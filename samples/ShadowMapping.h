//
// Created by huangkun on 2018/4/13.
//

#ifndef GL_SHADOWMAPPING_H
#define GL_SHADOWMAPPING_H

#include "CustomDraw.h"

TEST_NODE_BEGIN(ShadowMapping)

        unsigned int texture2;
        unsigned int planeVAO;
        unsigned int planeVBO;
        unsigned int planeEBO;
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int depthMapFBO;
        Shader depthShader;
        Shader shadowShader;

        void renderScene(Shader &shader);

TEST_NODE_END(ShadowMapping)


#endif //GL_SHADOWMAPPING_H
