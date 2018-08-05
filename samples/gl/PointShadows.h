//
// Created by huangkun on 2018/5/2.
//

#ifndef GL_POINTSHADOWS_H
#define GL_POINTSHADOWS_H

#include "CustomDraw.h"

TEST_NODE_BEGIN(PointShadows)

        unsigned int texture2;
        unsigned int planeVAO;
        unsigned int planeVBO;
        unsigned int planeEBO;
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int depthMapFBO;
        Shader depthShader;
        Shader shadowShader;

        void renderScene(Shader &shader, bool faceCulling = false);

        unsigned int renderType;
        vec3 lightPos;
        unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

TEST_NODE_END(PointShadows)


#endif //GL_POINTSHADOWS_H
