//
// Created by huangkun on 09/04/2018.
//

#include "FaceCulling.h"

TEST_NODE_IMP_BEGIN

    FaceCulling::FaceCulling() {
        cameraPos = vec3(0.0f, 2.0f, 2.0f);
        cameraDir = vec3(0.0f, 0.0f, 0.0f) - cameraPos;

        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        glm::quat quat = glm::quat_cast(view);
        vec3 angles = eulerAngles(quat);
        pitch = -degrees(angles.x);
    }

    void FaceCulling::draw(const mat4 &transform) {
        glEnable(GL_CULL_FACE);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();
        // use WSAD to control
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);
        glBindVertexArray(VAO);

        // x left
        glCullFace(GL_BACK); // default
        glFrontFace(GL_CCW); // default
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x right
        glCullFace(GL_FRONT);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDisable(GL_CULL_FACE);
    }


    FaceCulling::~FaceCulling() {
    }
TEST_NODE_IMP_END