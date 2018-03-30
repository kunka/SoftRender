//
// Created by huangkun on 26/03/2018.
//

#include "Cube.h"
#include <glad/glad.h>

static float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

Cube::Cube() {
}

Cube *Cube::create() {
    Cube *ret = new Cube();
    if (ret && ret->init()) {
    }
    return ret;
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

bool Cube::init() {
    const char *vs = "#version 330 core\
    layout (location = 0) in vec3 aPos;\
    layout (location = 1) in vec2 aTexCoord;\
    out vec2 TexCoord;\
    uniform mat4 model; \
    uniform mat4 view; \
    uniform mat4 projection; \
    void main()\
    {\
    gl_Position = projection * view * model * vec4(aPos, 1.0);\
    TexCoord = vec2(aTexCoord.x, 1.0-aTexCoord.y);\
    }";
    const char *fs = "#version 330 core\
    out vec4 FragColor;\
    \
    in vec2 TexCoord;\
    uniform sampler2D texture1;\
    uniform sampler2D texture2;\
    void main()\
    {\
    vec2 coord = TexCoord;\
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, coord) , 0.5);\
    }";

    shader.loadStr(vs, fs);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

//void Cube::draw() {
//
//}
