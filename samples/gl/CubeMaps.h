//
// Created by huangkun on 2018/4/9.
//

#ifndef GL_CUBEMAPS_H
#define GL_CUBEMAPS_H

#include "CustomDraw.h"

TEST_NODE_BEGIN(CubeMaps)

        unsigned int textureSkyBox;
        unsigned int skyBoxVAO;
        unsigned int skyBoxVBO;;
        Shader skyBoxShader;

TEST_NODE_END(CubeMaps)


#endif //GL_CUBEMAPS_H
