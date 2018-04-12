//
// Created by huangkun on 2018/4/12.
//

#include "GammaCorrection.h"

TEST_NODE_IMP_BEGIN

    GammaCorrection::GammaCorrection() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
out vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    texCoord = a_texCoord;
}
)";

        const char *frag = R"(
#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform vec4 color;
uniform float gamma;

void main()
{
    FragColor = color;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}
)";
        shader.loadStr(vert, frag);

        // Plane
        float vertices[] = {
                // postions       // texture coords
                0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
                -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // bottom top
        };
        unsigned int indices[] = { // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
        };
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        shader.use();
        shader.setMat4("projection", projection);
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);

        glEnable(GL_DEPTH_TEST);

//      Textures used for coloring objects like diffuse textures are almost always in sRGB space.
//      Textures used for retrieving lighting parameters like specular maps and normal maps are almost
//      always in linear space so if you were to configure these as sRGB textures as well, the lighting will break down.
    }

    void GammaCorrection::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        shader.use();

        // Default with gamma correction
        for (int i = 0; i <= 10; i++) {
            model = glm::mat4();
            model = glm::translate(model, vec3(0.5 * (i - 5), 0.25, 0));
            model = glm::scale(model, vec3(0.5, 0.5, 1));
            shader.setMat4("model", model);
            shader.setFloat("gamma", 1.0f);
            shader.setVec4("color", vec4(i / 10.0, i / 10.0, i / 10.0, 1.0));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        // Convert to linear color space
        for (int i = 0; i <= 10; i++) {
            model = glm::mat4();
            model = glm::translate(model, vec3(0.5 * (i - 5), -0.25, 0));
            model = glm::scale(model, vec3(0.5, 0.5, 1));
            shader.setMat4("model", model);
            shader.setFloat("gamma", 2.2f);
            shader.setVec4("color", vec4(i / 10.0, i / 10.0, i / 10.0, 1.0));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    GammaCorrection::~GammaCorrection() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
TEST_NODE_IMP_END