//
// Created by huangkun on 2018/4/24.
//

#include "AStar.h"

TEST_NODE_IMP_BEGIN

    AStar::AStar() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec2 a_position;
out vec2 pos;

void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
    pos = vec2(a_position);
}
)";

        const char *frag = R"(
#version 330 core
out vec4 FragColor;
in vec2 pos;

layout (std140) uniform Map
{
    vec4 status[1200];
    vec4 times[1200];
//    int status[1200];
//    float times[1200];
};

void main()
{
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    int width = 40;
    int height = 30;
    int x = int((pos.x/2 + 0.5) * width);
    int y = int((pos.y/2 + 0.5) * height);
    int index = x + y * width;

    color.b = times[index].r;
    color.g = color.b;
    color.r = color.b;

    FragColor = color;
}
)";
        shader.loadStr(vert, frag);

        float vertices[] = {
                1.0f, 1.0f,  // top right
                1.0f, -1.0f, // bottom right
                -1.0f, -1.0f,  // bottom left
                -1.0f, 1.0f,   // bottom top
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

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        width = 40;
        height = 30;
        int max = 100;
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++) {
                visitTimes.push_back(std::rand() % max);
            }

        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++) {
                map.push_back(0);
            }

        shader.use();
        unsigned int bindingPoint = 1;
        unsigned int index = glGetUniformBlockIndex(shader.ID, "Map");
        glUniformBlockBinding(shader.ID, index, bindingPoint);
        int blockSize = 0;
        glGetActiveUniformBlockiv(shader.ID, index, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        log("blockSize = %d", blockSize);

        glGenBuffers(1, &UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        auto mapSize = 16 * map.size();
        auto visitTimesSize = 16 * visitTimes.size();
        log("mapSize = %d", mapSize);
        log("visitTimesSize = %d", visitTimesSize);
        glBufferData(GL_UNIFORM_BUFFER, mapSize + visitTimesSize, NULL, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        drawMap();
    }

    void AStar::drawMap() {
        int max = 100;
        // std140 require align memory
        std::vector<vec4> map2;
        std::vector<vec4> visitTimes2;
        for (int i = 0; i < width * height; i++) {
            map2.push_back(vec4(map[i], 0, 0, 0));
            // normalize
            visitTimes2.push_back(vec4(visitTimes[i] / max, 0, 0, 0));
        }
        auto mapSize = 16 * map.size();
        auto visitTimesSize = 16 * visitTimes.size();

        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        // define the range of the buffer that links to a uniform binding point
//        glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, mapSize + visitTimesSize);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, mapSize, &map2[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, mapSize, visitTimesSize, &visitTimes2[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        CHECK_GL_ERROR_DEBUG();
    }

    void AStar::draw(const mat4 &transform) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        shader.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    AStar::~AStar() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

TEST_NODE_IMP_END