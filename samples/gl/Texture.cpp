//
// Created by Administrator on 2018/4/1 0001.
//

#include "Texture.h"

TEST_NODE_IMP_BEGIN

    Texture::Texture() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_texCoord;
out vec4 ourColor;
out vec2 texCoord;
uniform mat4 model;

void main()
{
    gl_Position = model * vec4(a_position, 1.0);
    ourColor = vec4(a_color);
//    texCoord = vec2(a_texCoord.x, 1.0 - a_texCoord.y); // flipY
    texCoord = a_texCoord;
}
)";

        const char *frag = R"(
#version 330 core
in vec4 ourColor;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
//uniform sampler2D ourTexture2;

void main()
{
    FragColor = texture(ourTexture, texCoord);;
//    FragColor = texture(ourTexture, texCoord) * vec4(ourColor); // Color it
//    FragColor = mix(texture(ourTexture, texCoord), texture(ourTexture2, texCoord), 0.2); // mix textures
}
)";
        // GLSL’s built-in mix function takes two values as input and linearly interpolates between them based on its third argument.
        // If the third value is 0.0 it returns the first input; if it’s 1.0 it returns the second input value.
        // A value of 0.2 will return 80% of the first input color and 20% of the second input color, resulting in a mixture of both our textures.
        shader.loadStr(vert, frag);


        float vertices[] = {
                // postions       // colors              // texture coords
                0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 2.0f, 2.0f, // top right
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 2.0f, -1.0f, // bottom right
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f, // bottom left
                -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 2.0f, // bottom top
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        texture = loadTexture("../res/cat.jpg");
        shader.use();
//        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // set it manually
//        shader.setInt("texture2", 1); // or with shader class
        shader.setInt("ourTexture", 0);
    }

    void Texture::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        // top left
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        model = glm::mat4();
        model = glm::translate(model, vec3(-0.5, 0.5, 0));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // top right
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glBindVertexArray(VAO);
        model = glm::mat4();
        model = glm::translate(model, vec3(0.5, 0.5, 0));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // bottom left
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindVertexArray(VAO);
        model = glm::mat4();
        model = glm::translate(model, vec3(-0.5, -0.5, 0));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // bottom right
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        model = glm::mat4();
        model = glm::translate(model, vec3(0.5, -0.5, 0));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    Texture::~Texture() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
TEST_NODE_IMP_END