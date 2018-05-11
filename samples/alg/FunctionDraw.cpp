//
// Created by huangkun on 2018/5/4.
//

#include "FunctionDraw.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    FunctionDraw::FunctionDraw() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    color = a_color;
}
)";
        const char *frag = R"(
#version 330 core
out vec4 FragColor;
in vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}
)";
        shader.loadStr(vert, frag);

        float vertices[] = {
                // postions
                0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
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


//        cameraPos = vec3(3.0f, 3.0f, 3.0f);
//        cameraDir = vec3(0.0f) - cameraPos;
//
//        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
//        glm::quat quat = glm::quat_cast(view);
//        vec3 angles = eulerAngles(quat);
//        log("%f,%f,%f", degrees(angles.x), degrees(angles.y), degrees(angles.z));
////        pitch = -degrees(angles.x);

        shader.use();
        shader.setVec3("ourColor", vec3(1.0, 1.0, 1.0));

        elapsed = 0;
        cameraPos = vec3(2.0f, 2.0f, 2.0f);
        linear(yaw, yaw, -120, 5);
        linear(pitch, pitch, -20, 5);
    }

    void FunctionDraw::setCamera(const glm::vec3 &position, const glm::vec3 &rotation) {
        cameraPos = position;
        yaw += rotation.x;
        pitch += rotation.y;
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraDir = glm::normalize(front);
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
    }

    void FunctionDraw::linear(float &var, float from, float to, float dur) {
        tickers.push_back([=, &var](float delta) mutable {
            elapsed += delta;
            if (elapsed > 0) {
                if (elapsed < dur) {
                    var = from + (to - from) * elapsed / dur;
                } else {
                    var = to;
                    elapsed = INT_MIN;
                }
                glm::vec3 front;
                front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
                front.y = sin(glm::radians(pitch));
                front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
                cameraDir = glm::normalize(front);
            } else {
                elapsed = INT_MIN;
            }
        });
    }

    void FunctionDraw::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        model = glm::mat4();
        model = glm::scale(model, vec3(1.5));
        shader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 6);
    }


    void FunctionDraw::fixedUpdate(float delta) {
        CustomDraw::fixedUpdate(delta);
        Input *input = Input::getInstance();
        // pitch and yaw
        vec2 deltaP = input->getMousePosDelta();
        if (deltaP.x != 0 && deltaP.y != 0) {
            float sensitivity = delta * 5;
            yaw += deltaP.x * sensitivity;
            pitch += deltaP.y * sensitivity;

            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch< - 89.0f) pitch = -89.0f;
            log("yaw(%.1f), pitch(%.1f)", yaw, pitch);

            glm::vec3 front;
            front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
            front.y = sin(glm::radians(pitch));
            front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
            cameraDir = glm::normalize(front);
        }

        for (auto function  : tickers) {
            function(delta);
        }
    }

    FunctionDraw::~FunctionDraw() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END