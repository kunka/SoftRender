//
// Created by huangkun on 2018/8/30.
//

#include "DeferredShading.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    DeferredShading::DeferredShading() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    Normal = transpose(inverse(mat3(model))) * a_normal;
    TexCoord = a_texCoord;
    FragPos = vec3(model * vec4(a_position, 1.0));
}
)";

        const char *quad_vert = R"(
#version 330 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_texCoord;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
    texCoord = a_texCoord;
}
)";

        const char *frag = R"(
#version 330 core
   layout (location = 0) out vec3 gPosition;
   layout (location = 1) out vec3 gNormal;
   layout (location = 2) out vec4 gAlbedoSpec;

   uniform sampler2D texture_diffuse1;
   uniform sampler2D texture_specular1;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoord).rgb;
    gAlbedoSpec.a = texture(texture_specular1, TexCoord).r;
}
)";

        const char *visualize_gbuffer_frag = R"(
#version 330 core
in vec2 texCoord;
out vec4 FragColor;

   uniform sampler2D gPosition;
   uniform sampler2D gNormal;
   uniform sampler2D gAlbedoSpec;

   uniform float screenWidth;
   uniform float screenHeight;

void main()
{
    vec3 color;
    if(gl_FragCoord.y > screenHeight/2)
    {
        if(gl_FragCoord.x < screenWidth/2)
        {
            // position, (0-0.5, 0.5-1) --> (0-1,0-1)
            color = texture(gPosition, vec2(texCoord.x*2, texCoord.y*2-1)).rgb;
        }
        else
        {
            // normal, (0.5-1, 0.5-1) --> (0-1,0-1)
            color = texture(gNormal, vec2(texCoord.x*2-1, texCoord.y*2-1)).rgb;
        }
    }
    else
    {
        if(gl_FragCoord.x < screenWidth/2)
        {
            // albedo, (0-0.5, 0-0.5) --> (0-1,0-1)
            color = texture(gAlbedoSpec, vec2(texCoord.x*2, texCoord.y*2)).rgb;
        }
        else
        {
            // specular, (0.5-1, 0-0.5) --> (0-1,0-1)
            color = texture(gAlbedoSpec, vec2(texCoord.x*2-1, texCoord.y*2)).rgb;
            float specular = texture(gAlbedoSpec, vec2(texCoord.x*2-1, texCoord.y*2)).a;
            color *= 1.0 + normalize(vec3(specular,specular,specular)).r;
        }
    }

    FragColor = vec4(color, 1.0);
}
)";

        const char *deferred_frag = R"(
#version 330 core
   out vec4 FragColor;

   uniform sampler2D gPosition;
   uniform sampler2D gNormal;
   uniform sampler2D gAlbedoSpec;

   struct Light {
       vec3 Position;
       vec3 Color;
       float Linear;
       float Quadratic;
   };
   const int NR_LIGHTS = 32;
   uniform Light lights[NR_LIGHTS];
   uniform vec3 viewPos;

   in vec2 texCoord;

void main()
{
       // retrieve data from G-buffer
       vec3 FragPos = texture(gPosition, texCoord).rgb;
       vec3 Normal = texture(gNormal, texCoord).rgb;
       vec3 Albedo = texture(gAlbedoSpec, texCoord).rgb;
       float Specular = texture(gAlbedoSpec, texCoord).a;

       // then calculate lighting as usual
       vec3 lighting = Albedo * 0.1; // hard-coded ambient component
       vec3 viewDir = normalize(viewPos - FragPos);
       for(int i = 0; i < NR_LIGHTS; ++i)
       {
            // diffuse
           vec3 lightDir = normalize(lights[i].Position - FragPos);
           vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;

        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].Color * spec * Specular;

        // attenuation
        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
       }
       FragColor = vec4(lighting, 1.0);
}
)";

        const char *light_frag = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
void main()
{
    FragColor = vec4(lightColor,1.0);
}
)";
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                // positions   // texCoords
                -1.0f, 1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 1.0f, 0.0f,

                -1.0f, 1.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f
        };
        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);

        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        {
            auto &size = Director::getInstance()->getFrameBufferSize();
            // create a color attachment texture
            glGenTextures(1, &gPosition);
            glBindTexture(GL_TEXTURE_2D, gPosition);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.width, size.height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

            glGenTextures(1, &gNormal);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.width, size.height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

            glGenTextures(1, &gAlbedoSpec);
            glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

            unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
            glDrawBuffers(3, attachments);

            // create and attach depth buffer (renderbuffer)
            unsigned int rboDepth;
            glGenRenderbuffers(1, &rboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
            // finally check if framebuffer is complete
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Framebuffer not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        modelObject = new Model();
        modelObject->load("../res/nanosuit/nanosuit.obj");

        shader.loadStr(vert, frag);
        visualizeGBufferShader.loadStr(quad_vert, visualize_gbuffer_frag);
        deferredShader.loadStr(quad_vert, deferred_frag);
        lightShader.loadStr(vert, light_frag);

        shader.use();
        shader.setInt("gPosition", 0);
        shader.setInt("gNormal", 1);
        shader.setInt("gAlbedoSpec", 2);
        shader.setMat4("projection", projection);
        deferredShader.use();
        deferredShader.setInt("gPosition", 0);
        deferredShader.setInt("gNormal", 1);
        deferredShader.setInt("gAlbedoSpec", 2);
        visualizeGBufferShader.use();
        visualizeGBufferShader.setInt("gPosition", 0);
        visualizeGBufferShader.setInt("gNormal", 1);
        visualizeGBufferShader.setInt("gAlbedoSpec", 2);

        cameraPos = vec3(0.0f, 0.0f, 10.0f);
        cameraDir = vec3(0.0f, 0.0f, 0.0f) - cameraPos;
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        glm::quat quat = glm::quat_cast(view);
        vec3 angles = eulerAngles(quat);
        pitch = -degrees(angles.x);

        // lighting info
        // -------------
        const unsigned int NR_LIGHTS = 32;
        srand(13);
        for (unsigned int i = 0; i < NR_LIGHTS; i++) {
            // calculate slightly random offsets
            float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
            float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
            float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
            lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
            // also calculate random color
            float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            lightColors.push_back(glm::vec3(rColor, gColor, bColor));
        }
    }

    void DeferredShading::draw(const mat4 &transform) {
        std::vector<glm::vec3> objectPositions;
        objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
        objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
        objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
        objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
        objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
        objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
        objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
        objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
        objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));
        auto &size = Director::getInstance()->getFrameBufferSize();


        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            shader.use();
            // use WSAD to control
            view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
            shader.setMat4("view", view);

            for (unsigned int i = 0; i < objectPositions.size(); i++) {
                model = glm::mat4();
                model = glm::translate(model, objectPositions[i]);
                model = glm::scale(model, glm::vec3(0.25f));
                shader.setMat4("model", model);
                modelObject->draw(shader);
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!Input::getInstance()->isKeyPressed(GLFW_KEY_SPACE)) {
            deferredShader.use();
            for (unsigned int i = 0; i < lightPositions.size(); i++) {
                deferredShader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
                deferredShader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
                // update attenuation parameters and calculate radius
                const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
                const float linear = 0.7;
                const float quadratic = 1.8;
                deferredShader.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
                deferredShader.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
            }
            deferredShader.setVec3("viewPos", cameraPos);
        } else {
            // visualize g-buffers
            visualizeGBufferShader.use();
            visualizeGBufferShader.setFloat("screenWidth", size.width);
            visualizeGBufferShader.setFloat("screenHeight", size.height);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
        // ----------------------------------------------------------------------------------
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
        // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
        // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
        // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
        glBlitFramebuffer(0, 0, size.width, size.height, 0, 0, size.width, size.height, GL_DEPTH_BUFFER_BIT,
                          GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // render light object
        if (!Input::getInstance()->isKeyPressed(GLFW_KEY_SPACE)) {
            lightShader.use();
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);
            for (unsigned int i = 0; i < lightPositions.size(); i++) {
                model = glm::mat4();
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(0.1f));
                lightShader.setMat4("model", model);
                lightShader.setVec3("lightColor", lightColors[i]);
                renderCube();
            }
        }
        glDisable(GL_DEPTH_TEST);
    }

    DeferredShading::~DeferredShading() {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        delete modelObject;
    }
TEST_NODE_IMP_END