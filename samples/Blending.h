//
// Created by huangkun on 08/04/2018.
//

#ifndef GL_BLENDING_H
#define GL_BLENDING_H

#include "CustomDraw.h"

TEST_NODE_BEGIN(Blending)

        unsigned int texture2;
        unsigned int textureGrass;
        unsigned int textureWindow;
        Shader grassShader;
        unsigned int planeVAO;
        unsigned int planeVBO;
        unsigned int planeEBO;

TEST_NODE_END(Blending)


#endif //GL_BLENDING_H
