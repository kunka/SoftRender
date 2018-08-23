//
// Created by huangkun on 2018/8/19.
//

#include "Mipmaps.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    Mipmaps::Mipmaps() {
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
//    texCoord = vec2(a_texCoord.x, 1.0 - a_texCoord.y); // flipY
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
        // GLSL’s built-in mix function takes two values as input and linearly interpolates between them based on its third argument.
        // If the third value is 0.0 it returns the first input; if it’s 1.0 it returns the second input value.
        // A value of 0.2 will return 80% of the first input color and 20% of the second input color, resulting in a mixture of both our textures.
        shader.loadStr(vert, frag);

        // Plane
        float vertices[] = {
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
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        texture = loadTexture("../res/wood.png");
        textureNoMM = loadTexture("../res/wood.png", false);
        shader.use();
        shader.setInt("ourTexture", 0);
        shader.setMat4("projection", projection);
    }

    void Mipmaps::draw(const mat4 &transform) {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        if (Input::getInstance()->isKeyPressed(GLFW_KEY_SPACE)) {
            glBindTexture(GL_TEXTURE_2D, textureNoMM);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        } else {
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);
        glBindVertexArray(VAO);

        std::vector<vec3> ps = {
                vec3(-1, -1, 1.5),
                vec3(-3, 3.5, -5),
                vec3(3, 3, -5),
                vec3(0, 0, -10),
                vec3(-10, 1, -15),
                vec3(5, 2, -15),
                vec3(8, -2, -12),
                vec3(3, -4, -8),
        };
//        shader.setMat4("view", glm::mat4());
//        shader.setMat4("projection", glm::mat4());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        for (int i = 0; i < ps.size(); i++) {
            model = glm::mat4();
//            model = glm::scale(model, vec3(1.9, 1.9, 1));
            model = glm::translate(model, ps[i]);
            shader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glDisable(GL_DEPTH_TEST);
    }

    Mipmaps::~Mipmaps() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
TEST_NODE_IMP_END