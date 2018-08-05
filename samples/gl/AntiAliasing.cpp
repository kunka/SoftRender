//
// Created by huangkun on 2018/4/11.
//

#include "AntiAliasing.h"

TEST_NODE_IMP_BEGIN

    AntiAliasing::AntiAliasing() {
    }

    void AntiAliasing::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);
        glBindVertexArray(VAO);

        // MSAA
        glEnable(GL_MULTISAMPLE);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-0.75f, 0.0f, 2.0f));
        model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // disable MSAA
        glDisable(GL_MULTISAMPLE);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.75f, 0.0f, 2.0f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    AntiAliasing::~AntiAliasing() {
    }
TEST_NODE_IMP_END