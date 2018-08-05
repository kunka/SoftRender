//
// Created by huangkun on 30/03/2018.
//

#include "CustomDraw.h"
#include "Input.h"
#include "stb_image.h"

int CustomDraw::currentSceneIndex;
std::vector<std::pair<std::string, Scene *(*)()>> CustomDraw::testScenes;

CustomDraw::CustomDraw() {
    stbi_set_flip_vertically_on_load(true); // flipY

    cameraPos = vec3(0.0f, 0.0f, 4.0f);
    cameraDir = vec3(0.0f, 0.0f, -1.0f);
    cameraUp = vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
    pitch = 0;
    yaw = -90;

    auto &size = Director::getInstance()->getWinSize();
    projection = glm::perspective(glm::radians(60.0f), size.width / size.height, 0.1f, 100.0f);
}

CustomDraw::~CustomDraw() {
}

void CustomDraw::fixedUpdate(float delta) {
    Input *input = Input::getInstance();
    float cameraSpeed = delta * 5;
    // move
    if (input->isKeyPressed(GLFW_KEY_W))
        cameraPos += cameraSpeed * cameraDir;
    if (input->isKeyPressed(GLFW_KEY_S))
        cameraPos -= cameraSpeed * cameraDir;
    if (input->isKeyPressed(GLFW_KEY_A))
        cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;
    if (input->isKeyPressed(GLFW_KEY_D))
        cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;

    // pitch and yaw
    vec2 deltaP = input->getMousePosDelta();
    if (deltaP.x != 0 && deltaP.y != 0) {
        float sensitivity = delta * 5;
        yaw += deltaP.x * sensitivity;
        pitch += deltaP.y * sensitivity;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraDir = glm::normalize(front);
    }

    if (input->isKeyPressed(GLFW_KEY_P))
        log("cameraPos(%.1f,%.1f,%.1f), cameraDir(%.02f,%.02f,%.02f), cameraUp(%.1f,%.1f,%.1f)", cameraPos.x,
            cameraPos.y,
            cameraPos.z, cameraDir.x, cameraDir.y, cameraDir.z, cameraUp.x, cameraUp.y, cameraUp.z);

//    if (input->isKeyPressed(GLFW_KEY_LEFT)) {
//        CustomDraw::currentSceneIndex--;
//        if (CustomDraw::currentSceneIndex < 0) {
//            CustomDraw::currentSceneIndex = CustomDraw::testScenes.size() - 1;
//        }
//        auto pair = CustomDraw::testScenes[CustomDraw::currentSceneIndex];
//        Director::getInstance()->replaceScene(pair.second());
//        log("switch to scene: %s", pair.first.c_str());
//    } else if (input->isKeyPressed(GLFW_KEY_RIGHT)) {
//        CustomDraw::currentSceneIndex++;
//        if (CustomDraw::currentSceneIndex >= CustomDraw::testScenes.size()) {
//            CustomDraw::currentSceneIndex = 0;
//        }
//        auto pair = CustomDraw::testScenes[CustomDraw::currentSceneIndex];
//        Director::getInstance()->replaceScene(pair.second());
//        log("switch to scene: %s", pair.first.c_str());
//    }
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int CustomDraw::loadTexture(const std::string &path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        log("Texture failed to load at path: %s", path.c_str());
        stbi_image_free(data);
    }

    return textureID;
}