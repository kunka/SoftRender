//
// Created by huangkun on 2018/8/8.
//

#ifndef GL_LIGHTING_H
#define GL_LIGHTING_H

#include "TextureCube.h"

TEST_NODE_BEGIN2(Lighting, TextureCube)

    protected:
        virtual void setPixel(int x, int y, int z, float u, float v, const std::vector<vec4> &varying,
                              const std::vector<vec4> &uniforms = {}) override;

TEST_NODE_END(Lighting)

#endif //GL_LIGHTING_H
