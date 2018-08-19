//
// Created by huangkun on 2018/4/9.
//

#include "CubeMaps.h"

TEST_NODE_IMP_BEGIN

    CubeMaps::CubeMaps() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
//layout (location = 1) in vec2 a_texCoord;
//out vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 Position;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
//    texCoord = a_texCoord;

    Normal = mat3(transpose(inverse(model))) * a_normal;
    Position = vec3(model * vec4(a_position, 1.0));
}
)";

        const char *frag = R"(
#version 330 core
//in vec2 texCoord;
out vec4 FragColor;

//uniform sampler2D ourTexture;

uniform vec3 cameraPos;
uniform samplerCube skyBox;

in vec3 Normal;
in vec3 Position;

void main()
{
//    FragColor = texture(ourTexture, texCoord);

    // reflect
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skyBox, R).rgb, 1.0);
}
)";

        const char *skybox_vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
//    gl_Position = projection * view * vec4(a_position, 1.0);
    vec4 pos = projection * view * vec4(a_position, 1.0);
    gl_Position = pos.xyww;
    TexCoords = a_position;
}
)";

        const char *skybox_frag = R"(
#version 330 core
in vec3 Normal;
in vec3 Position;
in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skyBox;

void main()
{
    FragColor = texture(skyBox, TexCoords);
}
)";
        shader.loadStr(vert, frag);
        skyBoxShader.loadStr(skybox_vert, skybox_frag);

//        float vertices[] = {
//                // positions        // texture coords
//                // Back face
//                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
//                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
//                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right
//                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
//                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
//                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
//                // Front face
//                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
//                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
//                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
//                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
//                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
//                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
//                // Left face
//                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
//                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-left
//                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
//                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
//                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
//                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
//                // Right face
//                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
//                0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
//                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
//                0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
//                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
//                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
//                // Bottom face
//                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
//                0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
//                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
//                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
//                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
//                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
//                // Top face
//                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
//                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
//                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
//                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
//                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
//                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f  // bottom-left
//        };
        float vertices[] = {
                // positions          // normals
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
        };

        stbi_set_flip_vertically_on_load(false); // flipY
        int width, height, nrChannels;
        unsigned char *data = stbi_load("../res/container.jpg", &width, &height, &nrChannels, 0);
        if (!data) {
            log("Failed to load texture");
            return;
        } else {
            log("Texture width = %d, height = %d", width, height);
        }

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);

//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        float skyboxVertices[] = {
                // positions
                -1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,

                -1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f
        };

        glGenVertexArrays(1, &skyBoxVAO);
        glBindVertexArray(skyBoxVAO);

        glGenBuffers(1, &skyBoxVBO);
        glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

        std::vector<std::string> faces = {
                "right.jpg",
                "left.jpg",
                "top.jpg",
                "bottom.jpg",
                "back.jpg",
                "front.jpg",
        };
        glGenTextures(1, &textureSkyBox);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkyBox);
        for (unsigned int i = 0; i < 6; i++) {
            int width, height, nrChannels;
            std::string name = "../res/skybox/" + faces[i];
            unsigned char *data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
            if (!data) {
                log("Failed to load texture");
                return;
            } else {
                log("Texture width = %d, height = %d", width, height);
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            stbi_image_free(data);
        }

        shader.use();
//        shader.setInt("ourTexture", 0);
        shader.setInt("skyBox", 0);
        shader.setMat4("projection", projection);

        skyBoxShader.use();
        skyBoxShader.setInt("skyBox", 0);
        skyBoxShader.setMat4("projection", projection);
    }

    void CubeMaps::draw(const mat4 &transform) {
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);
        shader.setVec3("cameraPos", cameraPos);

        model = glm::mat4();
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        // draw cube
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


        // draw skybox as last
        // change depth function so depth test passes when values are equal to depth buffer's content
        glDepthFunc(GL_LEQUAL);
        skyBoxShader.use();
        view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
        skyBoxShader.setMat4("view", view);
        glBindVertexArray(skyBoxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkyBox);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        glDisable(GL_DEPTH_TEST);
    }

    CubeMaps::~CubeMaps() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &skyBoxVAO);
        glDeleteBuffers(1, &skyBoxVBO);
    }
TEST_NODE_IMP_END