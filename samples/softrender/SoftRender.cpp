//
// Created by huangkun on 2018/6/16.
//

#include "SoftRender.h"

TEST_NODE_IMP_BEGIN

    SoftRender::SoftRender() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(a_position, 1.0);
    texCoord = a_texCoord;
}
)";

        const char *frag = R"(
#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, texCoord);;
}
)";
        shader.loadStr(vert, frag);

        float ratio = 512.0f / 512.0f;
        float vertices[] = {
                // postions       // texture coords
                ratio, ratio, 0.0f, 1.0f, 1.0f, // top right
                ratio, -ratio, 0.0f, 1.0f, 0.0f, // bottom right
                -ratio, -ratio, 0.0f, 0.0f, 0.0f, // bottom left
                -ratio, ratio, 0.0f, 0.0f, 1.0f, // bottom top
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        shader.use();
        shader.setInt("ourTexture", 0);

        clipRect.setRect(0, 0, TEX_WIDTH, TEX_HEIGHT);
        isClipRect = true;
        depthBuff = new float[TEX_WIDTH * TEX_HEIGHT];
        depthTest = false;
    }

    void SoftRender::draw(const mat4 &transform) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        texture = genTexture();

        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    SoftRender::~SoftRender() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        delete[] depthBuff;
    }

    unsigned int SoftRender::genTexture() {
        unsigned int textureID = texture;
        if (textureID == -1) {
            glGenTextures(1, &textureID);
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (textureID == 0) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        return textureID;
    }

    void SoftRender::setPixel(int x, int y, float depth, const vec4 &color) {
        if (x >= 0 && y >= 0 && x < TEX_WIDTH && y < TEX_HEIGHT) {
            int index = y * TEX_WIDTH + x;
            if (!depthTest or depth < depthBuff[index]) {
                depthBuff[index] = depth;
                texData[y][x][0] = (GLubyte) color.r;
                texData[y][x][1] = (GLubyte) color.g;
                texData[y][x][2] = (GLubyte) color.b;
                texData[y][x][3] = (GLubyte) color.a;
            }
        }
    }

    void SoftRender::setPixel(int x, int y, float depth, const vec3 &color) {
        setPixel(x, y, depth, vec4(color, 255));
    }

    float SoftRender::interp(float f1, float f2, float t) {
        return f1 + (f2 - f1) * t;
    }

    vec2 SoftRender::interp(const vec2 &v1, const vec2 &v2, float t) {
        return vec2(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t);
    }

    vec3 SoftRender::interp(const vec3 &v1, const vec3 &v2, float t) {
        return vec3(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t);
    }

    void SoftRender::setDepthTest(bool depthTest) {
        this->depthTest = depthTest;
    }

    void SoftRender::clearColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
        int color = a << 24 | r << 16 | g << 8 | b;
        memset(texData, color, TEX_WIDTH * TEX_HEIGHT * 4);
    }

    void SoftRender::clearDepth() {
        for (int i = 0; i < TEX_WIDTH * TEX_HEIGHT; i++)
            depthBuff[i] = INT_MAX;
    }

    bool SoftRender::cvvCull(vec4 triangle[3]) {
        return !inCvv(triangle[0]) && !inCvv(triangle[1]) && !inCvv(triangle[2]);
//        return !inCvv(triangle[0]) || !inCvv(triangle[1]) || !inCvv(triangle[2]);
    }

    bool SoftRender::inCvv(const vec4 &vector) {
        float w = fabs(vector.w);
        return w > 0.01 && fabs(vector.x) < w && fabs(vector.y) < w && fabs(vector.z) < w;
    }

TEST_NODE_IMP_END