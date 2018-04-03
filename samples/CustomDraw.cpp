//
// Created by huangkun on 30/03/2018.
//

#include "CustomDraw.h"
#include "Input.h"

CustomDraw::CustomDraw() {
    cameraPos = vec3(0.0f, 0.0f, 4.0f);
    cameraDir = vec3(0.0f, 0.0f, -1.0f);
    cameraUp = vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
    pitch = 0;
    yaw = -90;
}

CustomDraw::~CustomDraw() {
}


void CustomDraw::fixedUpdate(float delta) {
    Input *input = Input::getInstance();
    float cameraSpeed = delta * 3;
    // move
    if (input->isKeyPressed(GLFW_KEY_W))
        cameraPos += cameraSpeed * cameraDir;
    if (input->isKeyPressed(GLFW_KEY_S))
        cameraPos -= cameraSpeed * cameraDir;
    if (input->isKeyPressed(GLFW_KEY_A))
        cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;
    if (input->isKeyPressed(GLFW_KEY_D))
        cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;

    // pitch and yaw
    vec2 deltaP = input->getMousePosDelta();
    if (deltaP.x != 0 && deltaP.y != 0) {
        float sensitivity = delta * 3;
        yaw += deltaP.x * sensitivity;
        pitch += deltaP.y * sensitivity;

        if (pitch > 45.0f) pitch = 45.0f;
        if (pitch < -45.0f) pitch = -45.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraDir = glm::normalize(front);
    }
}