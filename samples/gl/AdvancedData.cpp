//
// Created by huangkun on 2018/4/11.
//

#include "AdvancedData.h"

TEST_NODE_IMP_BEGIN

    AdvancedData::AdvancedData() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        float positions[] = {
                0.5f, 0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                -0.5f, -0.5f, 0.0f,
                -0.5f, 0.5f, 0.0f,
        };
        float colors[] = {
                1.0f, 0.0f, 0.0f, 0.5f,
                0.0f, 1.0f, 0.0f, 0.5f,
                0.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
        };
        float texCoords[] = {
                1.0f, 1.0f, // top right
                1.0f, 0.0f, // bottom right
                0.0f, 0.0f, // bottom left
                0.0f, 1.0f, // bottom top
        };
        unsigned int indices[] = { // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

//        1. copy whole data once
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) 0);
//        glEnableVertexAttribArray(0);
//
//        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (7 * sizeof(float)));
//        glEnableVertexAttribArray(2);


//         2. sub data
//        Instead of an interleaved layout 123123123123 we take a batched approach 111122223333
//         alloc memory
//        glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors) + sizeof(texCoords), 0, GL_STATIC_DRAW);
//        // fill buffer
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
//        glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), &colors);
//        glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), sizeof(texCoords), &texCoords);

        // 3. map buffer
        // alloc memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors) + sizeof(texCoords), 0, GL_STATIC_DRAW);
        char *ptr = (char *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, positions, sizeof(positions));
        memcpy(ptr + sizeof(positions), colors, sizeof(colors));
        memcpy(ptr + sizeof(positions) + sizeof(colors), texCoords, sizeof(texCoords));
        glUnmapBuffer(GL_ARRAY_BUFFER);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) sizeof(positions));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                              (void *) (sizeof(positions) + sizeof(colors)));
        glEnableVertexAttribArray(2);


        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void AdvancedData::draw(const mat4 &transform) {
        Texture::draw(transform);
    }

    AdvancedData::~AdvancedData() {
    }

TEST_NODE_IMP_END