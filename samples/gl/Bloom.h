//
// Created by huangkun on 2018/8/28.
//

#ifndef GL_BLOOM_H
#define GL_BLOOM_H


#include "CustomDraw.h"

TEST_NODE_BEGIN(Bloom)

        unsigned int FBO;
        unsigned int textureColorBuffer[2];
        unsigned int quadVAO;
        unsigned int quadVBO;
        Shader quadShader;
        Shader lightShader;
        Shader blurShader;
        Shader shaderBloomFinal;
        unsigned int texture2;

        unsigned int pingpongFBO[2];
        unsigned int pingpongColorbuffers[2];

TEST_NODE_END(Bloom)

#endif //GL_BLOOM_H
