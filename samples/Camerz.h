//
// Created by huangkun on 02/04/2018.
//

#ifndef GL_CAMERZ_H
#define GL_CAMERZ_H

#include "CustomDraw.h"

TEST_NODE_BEGIN(Camerz)

        virtual void fixedUpdate(float delta) override;

    protected:
        vec3 cameraPos;
        vec3 cameraDir;
        vec3 cameraUp;
        mat4 model;
        mat4 view;
        mat4 projection;
        unsigned int texture2;

TEST_NODE_END(Camerz)

#endif //GL_CAMERZ_H
