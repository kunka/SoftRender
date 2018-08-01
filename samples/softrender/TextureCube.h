//
// Created by huangkun on 2018/6/25.
//

#ifndef GL_TEXTURECUBE_H
#define GL_TEXTURECUBE_H

#include "RastCube.h"

TEST_NODE_BEGIN2(TextureCube, RastCube)

    protected:
        vec3 sample(float u, float v);

        void fill(const vec4 &p1, const vec4 &p2, const vec4 &p3, const vec2 &uv,
                  const vec2 &uv2, const vec2 &uv3);

        void dda_line(const vec4 &pa, const vec4 &pb, const vec2 &uv, const vec2 &uv2);

        void setPixel(int x, int y, float depth, const vec4 &color);

        void setPixel(int x, int y, float depth, const vec3 &color);

        unsigned char *textureData;
        float *depthBuff;
        int textureWidth;
        int textureHeight;
TEST_NODE_END(TextureCube)


#endif //GL_TEXTURECUBE_H
