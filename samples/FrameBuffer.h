//
// Created by huangkun on 09/04/2018.
//

#ifndef GL_FRAMEBUFFER_H
#define GL_FRAMEBUFFER_H


#include "DepthTest.h"

TEST_NODE_BEGIN2(FrameBuffer, DepthTest)

        unsigned int FBO;
        unsigned int textureColorBuffer;
        unsigned int quadVAO;
        unsigned int quadVBO;
        Shader frameShader;

TEST_NODE_END(FrameBuffer)


#endif //GL_FRAMEBUFFER_H
