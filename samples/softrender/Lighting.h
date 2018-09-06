//
// Created by huangkun on 2018/8/8.
//

#ifndef GL_LIGHTING_H
#define GL_LIGHTING_H

#include "TextureCube.h"

TEST_NODE_BEGIN2(Lighting, TextureCube)

    public:
        virtual bool init() override;

    protected:

        virtual void setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms = {}) override;

        vec3 lightPos;
        vec3 lightColor;

TEST_NODE_END(Lighting)

#endif //GL_LIGHTING_H
