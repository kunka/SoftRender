//
// Created by huangkun on 08/04/2018.
//

#ifndef GL_STENCILTEST_H
#define GL_STENCILTEST_H


#include "CustomDraw.h"

TEST_NODE_BEGIN(StencilTest)

        Shader outlineShader;
        unsigned int texture2;
        unsigned int planeVAO;
        unsigned int planeVBO;
        unsigned int planeEBO;

TEST_NODE_END(StencilTest)


#endif //GL_STENCILTEST_H
