//
// Created by huangkun on 02/04/2018.
//

#include "Cubes.h"
#include "stb_image.h"

TEST_NODE_IMP_BEGIN

    Cubes::Cubes() {
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
uniform sampler2D ourTexture2;

void main()
{
    FragColor = mix(texture(ourTexture, texCoord), texture(ourTexture2, texCoord), 0.2); // mix textures
}
)";
        shader.loadStr(vert, frag);

        stbi_set_flip_vertically_on_load(true); // flipY

        int width, height, nrChannels;
        unsigned char *data = stbi_load("../res/container.jpg", &width, &height, &nrChannels, 0);
        if (!data) {
            log("Failed to load texture");
            return;
        } else {
            log("Texture width = %d, height = %d", width, height);
        }

        int width2, height2, nrChannels2;
        unsigned char *data2 = stbi_load("../res/smile.png", &width2, &height2, &nrChannels2, 0);
        if (!data2) {
            log("Failed to load texture2");
            return;
        } else {
            log("Texture2 width = %d, height = %d", width2, height2);
        }

        float vertices[] = {
                // postions        // texture coords
                // Back face
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                // Front face
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
                // Left face
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
                // Right face
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
                // Bottom face
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
                // Top face
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
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

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data2);
        stbi_image_free(data);

        auto &size = Director::getInstance()->getWinSize();
        projection = glm::perspective(glm::radians(60.0f), (float) size.width / (float) size.height, 0.1f, 100.0f);

        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

        shader.use();
        shader.setInt("ourTexture", 0);
        shader.setInt("ourTexture2", 1);
        shader.setMat4("projection", projection);
        glEnable(GL_DEPTH_TEST);
    }

    void Cubes::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();
        // use WSAD to control
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);

        glBindVertexArray(VAO);

        // x center
        model = glm::mat4();
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x left
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x right
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // y top
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // y bottom
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
        model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
    }

    Cubes::~Cubes() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END