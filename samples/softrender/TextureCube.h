//
// Created by huangkun on 2018/6/25.
//

#ifndef GL_TEXTURECUBE_H
#define GL_TEXTURECUBE_H

#include "RastCube.h"

TEST_NODE_BEGIN2(TextureCube, RastCube)

    protected:
        vec3 sample(float u, float v);

        void fill(const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec2 &uv,
                  const vec2 &uv2, const vec2 &uv3);

        void dda_line(const vec2 &pa, const vec2 &pb, const vec2 &uv, const vec2 &uv2);

        unsigned char *textureData;
        int textureWidth;
        int textureHeight;
TEST_NODE_END(TextureCube)


#endif //GL_TEXTURECUBE_H
