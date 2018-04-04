//
// Created by huangkun on 04/04/2018.
//

#include "ModelLoad.h"

TEST_NODE_IMP_BEGIN

    ModelLoad::ModelLoad() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    TexCoords = a_texCoords;
}
)";

        const char *frag = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}
)";
        shader.loadStr(vert, frag);

        modelObject = new Model("../res/nanosuit/nanosuit.obj");

        glEnable(GL_DEPTH_TEST);
    }

    void ModelLoad::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        model = glm::mat4();
        model = glm::translate(model, vec3(0, -10, -15));
        shader.setMat4("model", model);
        modelObject->draw(shader);
    }

    ModelLoad::~ModelLoad() {
        delete modelObject;
    }
TEST_NODE_IMP_END