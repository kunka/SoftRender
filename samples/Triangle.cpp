//
// Created by Administrator on 2018/4/1 0001.
//

#include "Triangle.h"

TEST_NODE_IMP_BEGIN

    Triangle::Triangle() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
out vec4 outColor;

void main()
{
    gl_Position = vec4(a_position, 1.0);
    outColor = vec4(1.0, 1.0, 0, 1.0);
}
)";

        const char *frag = R"(
#version 330 core
in vec4 outColor;
out vec4 FragColor;

void main()
{
    FragColor = outColor;
}
)";
        shader.loadStr(vert, frag);

        float vertices[] = {
                0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f, // bottom left
                0.0f, 0.5f, 0.0f,  // top
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        // unbind
        glBindVertexArray(0);
    }

    void Triangle::draw(const mat4 &transform) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    Triangle::~Triangle() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END