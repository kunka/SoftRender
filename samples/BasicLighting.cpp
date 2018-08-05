//
// Created by huangkun on 03/04/2018.
//

#include "BasicLighting.h"

TEST_NODE_IMP_BEGIN

    BasicLighting::BasicLighting() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    FragPos = vec3(model * vec4(a_position, 1.0));
    Normal = mat3(transpose(inverse(model))) * a_normal; // should cal on CPU
//    Normal = a_normal;
}
)";

        const char *frag = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.31f, 1.0f);
}
)";

        const char *light_frag = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0f);
}
)";

        const char *ambient_frag = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;

void main()
{
    vec3 ambient = ambientStrength * lightColor;
    FragColor = vec4(objectColor  * ambient, 1.0f);
}
)";

        const char *diffuse_frag = R"(
#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 lightPos;

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    FragColor = vec4(objectColor * (diffuse + ambient), 1.0);
}
)";

        const char *specular_frag = R"(
#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularStrength;

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor * specularStrength;

    FragColor = vec4(objectColor * (diffuse + ambient + specular), 1.0);
}
)";
        shader.loadStr(vert, frag);
        lightShader.loadStr(vert, light_frag);
        ambientShader.loadStr(vert, ambient_frag);
        diffuseShader.loadStr(vert, diffuse_frag);
        specularShader.loadStr(vert, specular_frag);

        float vertices[] = {
                // postions         // normals
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

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // light obj
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);


        auto &size = Director::getInstance()->getWinSize();
        projection = glm::perspective(glm::radians(60.0f), (float) size.width / (float) size.height, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("projection", projection);

        vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
        vec3 objectColor = vec3(1.0f, 0.5f, 0.31f);
        float ambientStrength = 0.1f;
        float specularStrength = 0.5f;

        lightShader.use();
        lightShader.setMat4("projection", projection);
        ambientShader.setVec3("lightColor", lightColor);

        ambientShader.use();
        ambientShader.setMat4("projection", projection);
        ambientShader.setVec3("objectColor", objectColor);
        ambientShader.setVec3("lightColor", lightColor);
        ambientShader.setFloat("ambientStrength", ambientStrength);

        diffuseShader.use();
        diffuseShader.setMat4("projection", projection);
        diffuseShader.setVec3("objectColor", objectColor);
        diffuseShader.setVec3("lightColor", lightColor);
        diffuseShader.setFloat("ambientStrength", ambientStrength);

        specularShader.use();
        specularShader.setMat4("projection", projection);
        specularShader.setVec3("objectColor", objectColor);
        specularShader.setVec3("lightColor", lightColor);
        specularShader.setFloat("ambientStrength", ambientStrength);
        specularShader.setFloat("specularStrength", specularStrength);
    }

    void BasicLighting::draw(const mat4 &transform) {
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);

        // use WSAD to control
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

        // light obj
        vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
        vec3 lightPos = glm::vec3(0.0f, 2.0f, 2.0f);
        lightShader.use();
        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2, 0.2f, 0.2f));
        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        lightShader.setVec3("lightColor", lightColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // y top, original
        shader.use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x left, ambient
        ambientShader.use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.8f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ambientShader.setMat4("model", model);
        ambientShader.setMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x center, diffuse
        diffuseShader.use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        diffuseShader.setMat4("model", model);
        diffuseShader.setMat4("view", view);
        diffuseShader.setVec3("lightPos", lightPos);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // specular light obj
        lightColor = vec3(0.0f, 1.0f, 0.0f);
        lightPos = glm::vec3(2.0f, 0.0f, -1.0f);
        lightShader.use();
        lightShader.setVec3("lightColor", lightColor);
        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2, 0.2f, 0.2f));
        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x right, specular
        specularShader.use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.8f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        specularShader.setVec3("lightColor", lightColor);
        specularShader.setVec3("lightPos", lightPos);
        specularShader.setMat4("model", model);
        specularShader.setMat4("view", view);
        specularShader.setVec3("viewPos", cameraPos);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glBindVertexArray(0);
        glDisable(GL_DEPTH_TEST);
    }

    BasicLighting::~BasicLighting() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteVertexArrays(1, &lightVAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END