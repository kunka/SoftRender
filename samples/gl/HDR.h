//
// Created by huangkun on 2018/8/27.
//

#ifndef GL_HDR_H
#define GL_HDR_H


#include "CustomDraw.h"

TEST_NODE_BEGIN(HDR)

        unsigned int FBO;
        unsigned int textureColorBuffer;
        unsigned int quadVAO;
        unsigned int quadVBO;
        Shader hdrShader;

TEST_NODE_END(HDR)


#endif //GL_HDR_H
