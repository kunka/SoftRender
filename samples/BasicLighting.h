//
// Created by huangkun on 03/04/2018.
//

#ifndef GL_BASICLIGHTING_H
#define GL_BASICLIGHTING_H


#include "CustomDraw.h"

TEST_NODE_BEGIN(BasicLighting)

        Shader lightShader;
        Shader ambientShader;
        Shader diffuseShader;
        Shader specularShader;

TEST_NODE_END(BasicLighting)

#endif //GL_BASICLIGHTING_H
