//
// Created by huangkun on 08/04/2018.
//

#include "StencilTest.h"

TEST_NODE_IMP_BEGIN

    StencilTest::StencilTest() {
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

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, texCoord);
}
)";

        const char *color_frag = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0, 0.0, 1.0);
}
)";
        shader.loadStr(vert, frag);
        outlineShader.loadStr(vert, color_frag);

        float vertices[] = {
                // postions        // texture coords
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
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

        // Plane
        float vertices2[] = {
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
        glGenVertexArrays(1, &planeVAO);
        glBindVertexArray(planeVAO);

        glGenBuffers(1, &planeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

        glGenBuffers(1, &planeEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);


        // Images
        texture = loadTexture("../res/container.jpg");
        texture2 = loadTexture("../res/metal.png");


        auto &size = Director::getInstance()->getWinSize();
        projection = glm::perspective(glm::radians(60.0f), size.width / size.height, 0.1f, 100.0f);
        shader.use();
        shader.setMat4("projection", projection);
        cameraPos = vec3(0.0f, 2.0f, 4.0f);
        cameraDir = vec3(0.0f, 0.0f, 0.0f) - cameraPos;

        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        glm::quat quat = glm::quat_cast(view);
        vec3 angles = eulerAngles(quat);
        pitch = -degrees(angles.x);

        outlineShader.use();
        outlineShader.setMat4("projection", projection);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    void StencilTest::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glStencilMask(0x00); // make sure we don’t update the stencil buffer while drawing the floor
        // plane
        shader.use();
        shader.setInt("ourTexture", 0);
        model = glm::mat4();
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(8, 6, 1));
        shader.setMat4("model", model);
        // use WSAD to control
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);

        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();
        shader.setInt("ourTexture", 0);

        // left behind
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.0f, 0.5f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // right behind
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0f, 0.5f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x center
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 1.0f));
        model = glm::scale(model, vec3(2, 1, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // outline
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        outlineShader.use();
        outlineShader.setMat4("view", view);

        // left behind
        float scale = 1.1;
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.0f, 0.5f, -1.0f));
        model = glm::scale(model, vec3(scale, scale, scale));
        outlineShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // right behind
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0f, 0.5f, -1.0f));
        model = glm::scale(model, vec3(scale, scale, scale));
        outlineShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x center
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 1.0f));
        model = glm::scale(model, vec3(2, 1, 1));
        model = glm::scale(model, vec3(scale, scale, scale));
        outlineShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // reset
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
    }

    StencilTest::~StencilTest() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteBuffers(1, &planeVBO);
        glDeleteBuffers(1, &planeEBO);
    }
TEST_NODE_IMP_END