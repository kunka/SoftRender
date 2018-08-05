//
// Created by huangkun on 2018/4/11.
//

#include "GeometryShader.h"

TEST_NODE_IMP_BEGIN

    GeometryShader::GeometryShader() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
out vec4 ourColor;

void main()
{
    gl_Position = vec4(a_position, 1.0);
    ourColor = vec4(a_color);
    gl_PointSize = 5;
}
)";

        const char *gem = R"(
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;
in vec4 ourColor[];
out vec4 color;

void main()
{
    color = ourColor[0];
    vec4 position = gl_in[0].gl_Position;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4( 0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4( 0.0, 0.4, 0.0, 0.0);
    color = vec4(1.0, 1.0, 1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
)";

        const char *frag = R"(
#version 330 core
in vec4 color;
out vec4 FragColor;

void main()
{
    FragColor = color;
}
)";
        shader.loadStr(vert, frag, gem);

        float vertices[] = {
                // postions       // colors
                -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom top
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom left
                0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom right
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        shader.use();
    }

    void GeometryShader::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_POINTS, 0, 4);
    }

    GeometryShader::~GeometryShader() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END