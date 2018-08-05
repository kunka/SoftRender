//
// Created by huangkun on 2018/4/11.
//

#include "AdvancedGLSL.h"

TEST_NODE_IMP_BEGIN

    AdvancedGLSL::AdvancedGLSL() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    gl_PointSize = gl_Position.z;
}
)";

        const char *frag = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    if(gl_FragCoord.x < 400)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";
        shader.loadStr(vert, frag);


        float vertices[] = {
                0.0f, 0.0f, 0.0f,
        };
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
        glEnableVertexAttribArray(0);
    }

    void AdvancedGLSL::draw(const mat4 &transform) {
        glEnable(GL_PROGRAM_POINT_SIZE);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glm::vec3 positions[] = {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f, 3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f, 2.0f, -2.5f),
                glm::vec3(1.5f, 0.2f, -1.5f),
                glm::vec3(-1.3f, 1.0f, -1.5f)
        };
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++) {
            model = glm::mat4();
            model = glm::translate(model, positions[i]);
            shader.setMat4("model", model);
            glDrawArrays(GL_POINTS, 0, 1);
        }

        glDisable(GL_PROGRAM_POINT_SIZE);
    }

    AdvancedGLSL::~AdvancedGLSL() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END