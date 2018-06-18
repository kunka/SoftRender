//
// Created by huangkun on 2018/6/16.
//

#ifndef GL_SOFTRENDER_H
#define GL_SOFTRENDER_H

#include "CustomDraw.h"

#define TEX_WIDTH   512
#define TEX_HEIGHT  512

TEST_NODE_BEGIN(SoftRender)

        vec3 interp(const vec3 &v1, const vec3 &v2, float t);

    protected:
        unsigned int genTexture();

        void setPixel(int x, int y, const vec3 &color);

        void setPixel(int x, int y, const vec4 &color);

        unsigned int texture = -1;

        GLubyte texData[TEX_WIDTH][TEX_HEIGHT][4];

TEST_NODE_END(SoftRender)


#endif //GL_SOFTRENDER_H
