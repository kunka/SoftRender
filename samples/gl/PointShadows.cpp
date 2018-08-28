//
// Created by huangkun on 2018/5/2.
//

#include "PointShadows.h"

TEST_NODE_IMP_BEGIN

    PointShadows::PointShadows() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform bool reverse_normals;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    vs_out.FragPos = vec3(model * vec4(a_position, 1.0));

    // a slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
    if(reverse_normals)
        vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * a_normal);
    else
        vs_out.Normal = transpose(inverse(mat3(model))) * a_normal;

    vs_out.TexCoords = a_texCoord;
}
)";
        const char *vert_depth = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
uniform mat4 model;

void main()
{
    gl_Position = model * vec4(a_position, 1.0);
}
)";

        const char *geom = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;
uniform mat4 shadowMatrices[6];
out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main() {
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle’s vertices
        {
           FragPos = gl_in[i].gl_Position;
           gl_Position = shadowMatrices[face] * FragPos;
           EmitVertex();
        }
        EndPrimitive();
    }
}
)";
        const char *frag = R"(
#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 ambient = 0.3 * color;
    vec3 lightColor = vec3(0.3);

    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    vec3 specular = lightColor * spec;

    FragColor = vec4(diffuse + ambient + specular, 1.0);
}
)";

        const char *depth_frag = R"(
#version 330 core
           uniform vec3 lightPos;
           uniform float far_plane;

in vec4 FragPos;

void main()
{

               // get distance between fragment and light source
               float lightDistance = length(FragPos.xyz - lightPos);
               // map to [0;1] range by dividing by far_plane
               lightDistance = lightDistance / far_plane;
               // write this as modified depth
               gl_FragDepth = lightDistance;
}
)";
        const char *shadow_frag = R"(
#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};
uniform Light light;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

   uniform vec3 lightPos;
   uniform vec3 viewPos;
   uniform float far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
(
vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float inShadow(vec3 fragPos)
{
    // PCF
   vec3 fragToLight = fragPos - lightPos;
   float currentDepth = length(fragToLight);

   float shadow = 0.0;
   float bias = 0.15;
   int samples = 20;
   float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
   for(int i = 0; i < samples; ++i)
   {
       float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] *
       diskRadius).r;
       closestDepth *= far_plane; // Undo mapping [0;1]
       if(currentDepth - bias > closestDepth)
           shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;

//    vec3 fragToLight = fragPos - lightPos;
//    float closestDepth = texture(depthMap, fragToLight).r;
//    closestDepth *= far_plane;
//    float currentDepth = length(fragToLight);
//    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
//    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 ambient = light.ambient * color;

    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    vec3 specular = light.specular * spec;

    float shadow = inShadow(fs_in.FragPos);
    FragColor = vec4(ambient + (1.0 - shadow) * (diffuse + specular), 1.0);

//    vec3 fragToLight = fs_in.FragPos - lightPos;
//    float closestDepth = texture(depthMap, fragToLight).r;
//    FragColor = vec4(vec3(closestDepth), 1.0);
}
)";

        shader.loadStr(vert, frag);
        depthShader.loadStr(vert_depth, depth_frag, geom);
        shadowShader.loadStr(vert, shadow_frag);

        float vertices[] = {
                // back face
                -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
                // front face
                -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
                1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
                1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
                -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
                -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
                // right face
                1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
                1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
                1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
                1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
                -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
                1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
                1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
                1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
                -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
                -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);


        // depth map fbo
        glGenTextures(1, &depthCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        for (unsigned int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                         SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                         NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // attach depth texture as FBO's depth buffer
        glGenFramebuffers(1, &depthMapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // visual depth quad
        float quadVertices[] = {
                // positions       // normals(uselsss) // texture Coords
                // to use the same vert shader
                -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Images
        texture = loadTexture("../res/wood.png");

        lightPos = vec3(0, 0, 0);
        shader.use();
        shader.setMat4("projection", projection);
        shader.setInt("diffuseTexture", 0);
        shader.setInt("depthMap", 1);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("light.position", lightPos);
        shader.setVec3("light.ambient", vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("light.diffuse", vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("light.specular", vec3(0.5f, 0.5f, 0.5f));
        shader.setFloat("far_plane", 25.0f);

        shadowShader.use();
        shadowShader.setMat4("projection", projection);
        shadowShader.setInt("diffuseTexture", 0);
        shadowShader.setInt("depthMap", 1);
        shadowShader.setVec3("lightPos", lightPos);
        shadowShader.setVec3("light.position", lightPos);
        shadowShader.setVec3("light.ambient", vec3(0.2f, 0.2f, 0.2f));
        shadowShader.setVec3("light.diffuse", vec3(1.0f, 1.0f, 1.0f));
        shadowShader.setVec3("light.specular", vec3(0.5f, 0.5f, 0.5f));
        shadowShader.setFloat("far_plane", 25.0f);

        cameraPos = vec3(3.0f, 0.0f, 4.0f);
        cameraDir = vec3(-2.0f, 1.5f, 0.0f) - cameraPos;
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        glm::quat quat = glm::quat_cast(view);
        vec3 angles = eulerAngles(quat);
        pitch = -degrees(angles.x);

        float aspect = (float) SHADOW_WIDTH / (float) SHADOW_HEIGHT;
        float near = 1.0f;
        float far = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f),
                                                            glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f),
                                                            glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f),
                                                            glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f),
                                                            glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f),
                                                            glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f),
                                                            glm::vec3(0.0f, -1.0f, 0.0f)));

        depthShader.use();
        depthShader.setVec3("lightPos", lightPos);
        depthShader.setMat4("shadowMatrices[0]", shadowTransforms[0]);
        depthShader.setMat4("shadowMatrices[1]", shadowTransforms[1]);
        depthShader.setMat4("shadowMatrices[2]", shadowTransforms[2]);
        depthShader.setMat4("shadowMatrices[3]", shadowTransforms[3]);
        depthShader.setMat4("shadowMatrices[4]", shadowTransforms[4]);
        depthShader.setMat4("shadowMatrices[5]", shadowTransforms[5]);
        depthShader.setFloat("far_plane", 25.0f);

    }

    void PointShadows::draw(const mat4 &transform) {
        glEnable(GL_DEPTH_TEST);

        renderType = 3;

        switch (renderType) {
            case 0 : {
                // render origin scene without shadow
                glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                shader.use();
                view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
                shader.setMat4("view", view);
                shader.setVec3("viewPos", cameraPos);
                renderScene(shader);
                break;
            }
            case 1: {
                // visual depth 1
                // light matrix
                glClear(GL_DEPTH_BUFFER_BIT);
                auto &size = Director::getInstance()->getWinSize();
//                mat4 pj = glm::perspective(glm::radians(60.0f), size.width / size.height, 0.1f, 100.0f);
                mat4 pj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
                mat4 vw = glm::lookAt(lightPos, vec3(0), vec3(0, 1, 0));
                depthShader.use();
                depthShader.setMat4("projection", pj);
                depthShader.setMat4("view", vw);
                depthShader.setInt("visual", 1);
                renderScene(depthShader);
                break;
            }
            case 2: {
                // visual depth by depth map
                // render to depth map
                auto &size = Director::getInstance()->getFrameBufferSize();
                glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
                glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);

//                mat4 pj = glm::perspective(glm::radians(60.0f), size.width / size.height, 0.1f, 100.0f);
                mat4 pj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
                mat4 vw = glm::lookAt(lightPos, vec3(0, 0, 0), vec3(0, 1, 0));
                depthShader.use();
                depthShader.setMat4("projection", pj);
                depthShader.setMat4("view", vw);
                depthShader.setInt("visual", 0);
                renderScene(depthShader);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // reset viewport
                glViewport(0, 0, size.width, size.height);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
                // reset transform
                depthShader.setMat4("projection", glm::mat4());
                depthShader.setMat4("view", glm::mat4());
                depthShader.setMat4("model", glm::mat4());
                depthShader.setInt("visual", 2);
                depthShader.setInt("depthMap", 0);
                glBindVertexArray(quadVAO);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);
                break;
            }
            case 3: {
                // render scene with shadow
                // render to depth map
                auto &size = Director::getInstance()->getFrameBufferSize();
                glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
                glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);

//                mat4 pj = glm::perspective(glm::radians(60.0f), size.width / size.height, 0.1f, 100.0f);
//                mat4 pj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
//                mat4 vw = glm::lookAt(lightPos, vec3(0, 0, 0), vec3(0, 1, 0));
                depthShader.use();
//                depthShader.setMat4("projection", projection);
//                depthShader.setMat4("view", view);
//                depthShader.setInt("visual", 0);
//                glEnable(GL_CULL_FACE);
//                glCullFace(GL_FRONT); // fix peter panning
//                renderScene(depthShader, true);
                renderScene(depthShader);
//                glDisable(GL_CULL_FACE);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // reset viewport
                glViewport(0, 0, size.width, size.height);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
                shadowShader.use();
                shadowShader.setMat4("projection", projection);
                shadowShader.setInt("depthMap", 1);
//                shadowShader.setMat4("lightSpaceMatrix", pj * vw);
                view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
                shadowShader.setMat4("view", view);
                shadowShader.setVec3("viewPos", cameraPos);
                renderScene(shadowShader);
                break;
            }
            default:
                break;
        }

        glDisable(GL_DEPTH_TEST);
    }

    void PointShadows::renderScene(Shader &shader, bool faceCulling) {
        if (faceCulling) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT); // fix peter panning
        }

        // room cube
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        model = glm::mat4();
        model = glm::scale(model, glm::vec3(5.0f));
        shader.setMat4("model", model);
        // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
        glDisable(GL_CULL_FACE);
        // A small little hack to invert normals when drawing cube from the inside so lighting still works.
        shader.setInt("reverse_normals", 1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // A small little hack to invert normals when drawing cube from the inside so lighting still works.
        shader.setInt("reverse_normals", 0);
        glEnable(GL_CULL_FACE);

        // cubes
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.75f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(0.75f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    PointShadows::~PointShadows() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
    }
TEST_NODE_IMP_END