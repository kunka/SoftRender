//
// Created by huangkun on 08/04/2018.
//

#include "Blending.h"
#include <map>

TEST_NODE_IMP_BEGIN

    Blending::Blending() {
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
        const char *grass_frag = R"(
#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, texCoord);
//    if(texColor.a < 0.05)
//        discard;
    FragColor = texColor;
}
)";
        shader.loadStr(vert, frag);
        grassShader.loadStr(vert, grass_frag);



        int width, height, nrChannels;
        unsigned char *data = stbi_load("../res/container.jpg", &width, &height, &nrChannels, 0);
        if (!data) {
            log("Failed to load texture");
            return;
        } else {
            log("Texture width = %d, height = %d", width, height);
        }

        int width2, height2, nrChannels2;
        unsigned char *data2 = stbi_load("../res/metal.png", &width2, &height2, &nrChannels2, 0);
        if (!data2) {
            log("Failed to load texture2");
            return;
        } else {
            log("Texture2 width = %d, height = %d", width2, height2);
        }

        int width3, height3, nrChannels3;
        unsigned char *data3 = stbi_load("../res/grass.png", &width3, &height3, &nrChannels3, 0);
        if (!data3) {
            log("Failed to load textureGrass");
            return;
        } else {
            log("textureGrass width = %d, height = %d", width3, height3);
        }

        int width4, height4, nrChannels4;
        unsigned char *data4 = stbi_load("../res/blending_transparent_window.png", &width4, &height4, &nrChannels4, 0);
        if (!data4) {
            log("Failed to load texture4");
            return;
        } else {
            log("texture4 width = %d, height = %d", width4, height4);
        }

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


        // Images
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenTextures(1, &textureGrass);
        glBindTexture(GL_TEXTURE_2D, textureGrass);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width3, height3, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenTextures(1, &textureWindow);
        glBindTexture(GL_TEXTURE_2D, textureWindow);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width4, height4, 0, GL_RGBA, GL_UNSIGNED_BYTE, data4);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data4);
        stbi_image_free(data3);
        stbi_image_free(data2);
        stbi_image_free(data);

        auto &size = Director::getInstance()->getWinSize();
        projection = glm::perspective(glm::radians(60.0f), (float) size.width / (float) size.height, 0.1f, 100.0f);
        shader.use();
        shader.setInt("ourTexture", 0);
        shader.setMat4("projection", projection);
        grassShader.use();
        grassShader.setInt("ourTexture", 0);
        grassShader.setMat4("projection", projection);

        cameraPos = vec3(0.0f, 1.5f, 3.0f);
        cameraDir = vec3(0.0f, 0.0f, 0.0f) - cameraPos;

        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        glm::quat quat = glm::quat_cast(view);
        vec3 angles = eulerAngles(quat);
        pitch = -degrees(angles.x);

    }

    void Blending::draw(const mat4 &transform) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();
        // use WSAD to control
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);

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

        // plane
        shader.setInt("ourTexture", 0);
        model = glm::mat4();
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(8, 6, 1));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(planeVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // grass
        grassShader.use();
        grassShader.setMat4("view", view);
        grassShader.setInt("ourTexture", 0);
        std::vector<glm::vec3> vegetation;
        vegetation.push_back(glm::vec3(-1.5f, 0.5f, -0.48f));
        vegetation.push_back(glm::vec3(1.5f, 0.5f, 0.51f));
        vegetation.push_back(glm::vec3(0.0f, 0.5f, 0.7f));
        vegetation.push_back(glm::vec3(-0.3f, 0.5f, -2.3f));
        vegetation.push_back(glm::vec3(0.5f, 0.5f, -0.6f));

//        glBindTexture(GL_TEXTURE_2D, textureGrass);
//        for (unsigned int i = 0; i < vegetation.size(); i++) {
//            model = glm::mat4();
//            model = glm::translate(model, vegetation[i]);
//            grassShader.setMat4("model", model);
//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        }

//        glBindTexture(GL_TEXTURE_2D, textureWindow);
//        for (unsigned int i = 0; i < vegetation.size(); i++) {
//            model = glm::mat4();
//            model = glm::translate(model, vegetation[i]);
//            grassShader.setMat4("model", model);
//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        }

//        1. Draw all opaque objects first.
//        2. Sort all the transparent objects.
//        3. Draw all the transparent objects in sorted order.
        glBindTexture(GL_TEXTURE_2D, textureWindow);
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < vegetation.size(); i++) {
            float distance = glm::distance(cameraPos, vegetation.at(i));
            sorted[distance] = vegetation[i];
        }
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.
                rend(); ++it) {
            model = glm::mat4();
            model = glm::translate(model, it->second);
            shader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // reset
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }

    Blending::~Blending() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteBuffers(1, &planeVBO);
        glDeleteBuffers(1, &planeEBO);

    }
TEST_NODE_IMP_END