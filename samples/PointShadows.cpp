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

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    vs_out.FragPos = vec3(model * vec4(a_position, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * a_normal;
    vs_out.TexCoords = a_texCoord;
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

struct Material {
sampler2D diffuse;
};
uniform Material material;

struct Light {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};
uniform Light light;

uniform vec3 viewPos;

void main()
{
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

    vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;
    vec3 ambient = light.ambient * color;

    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    vec3 specular = light.specular * spec;

    FragColor = vec4(diffuse + ambient + specular, 1.0);
}
)";


        shader.loadStr(vert, frag);

        float vertices[] = {
                // back face
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, // bottom-left
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, // top-right
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, // bottom-left
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.5f, // top-left
                // front face
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, // bottom-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, // top-right
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, // top-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, // bottom-left
                // left face
                -0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, // top-right
                -0.5f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.5f, // top-left
                -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, // bottom-left
                -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, // bottom-left
                -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
                -0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, // top-right
                // right face
                0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, // top-left
                0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, // top-right
                0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, // bottom-right
                0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, // top-left
                0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.5f, // top-left
                0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.0f, // bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-right
                -0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f, // top-right
                // top face
                -0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, // top-left
                0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.0f, // bottom-right
                -0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, // top-left
                -0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f  // bottom-left
        };
        float scale = 20;
        for (int i = 0; i < 36 * 8; i += 8) {
            for (int j = 0; j < 3; j++) {
                vertices[i + j] *= scale;
            }
        }

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

        // Images
        texture = loadTexture("../res/wood.png");

        // depth map fbo
        glGenFramebuffers(1, &depthMapFBO);
        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
                         GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setInt("material.diffuse", 0);
        shader.setVec3("light.ambient", vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("light.diffuse", vec3(0.5f, 0.5f, 0.5f)); // darken the light a bit to fit the scene
        shader.setVec3("light.specular", vec3(1.0f, 1.0f, 1.0f));

        glEnable(GL_DEPTH_TEST);
    }

    void PointShadows::draw(const mat4 &transform) {
        renderType = 0;

        switch (renderType) {
            case 0 : {
                // render origin scene without shadow
                glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                shader.use();
                view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
                shader.setMat4("view", view);
                renderScene(shader);
                break;
            }
            default:
                break;
        }
    }

    void PointShadows::renderScene(Shader &shader, bool faceCulling) {
        if (faceCulling) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT); // fix peter panning
        }

//        glEnable(GL_CULL_FACE);
//        glCullFace(GL_FRONT);
        // cube
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        model = glm::mat4();
//        model = glm::scale(model, vec3(10));
//        model = glm::translate(model, vec3(0, 0, 1.5));
        shader.setMat4("model", model);
        shader.setVec3("light.position", vec3(0, 0, 0));
        glDrawArrays(GL_TRIANGLES, 0, 36);
//        glDisable(GL_CULL_FACE);
    }

    PointShadows::~PointShadows() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END