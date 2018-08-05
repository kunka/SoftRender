//
// Created by huangkun on 2018/4/11.
//

#ifndef GL_BLINNPHONGLIGHTING_H
#define GL_BLINNPHONGLIGHTING_H


#include "CustomDraw.h"

TEST_NODE_BEGIN(BlinnPhongLighting)

        Shader bpShader;
        bool blinn = false;
        bool blinnKeyPressed = false;

        virtual void fixedUpdate(float delta) override;

TEST_NODE_END(BlinnPhongLighting)

#endif //GL_BLINNPHONGLIGHTING_H
