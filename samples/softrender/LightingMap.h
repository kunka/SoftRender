//
// Created by huangkun on 2018/8/10.
//

#ifndef GL_LIGHTINGMAP_H
#define GL_LIGHTINGMAP_H

#include "Lighting.h"

TEST_NODE_BEGIN2(LightingMap, Lighting)

    protected:
        virtual void setPixel(int x, int y, int z, float u, float v, vec3 varying[],
                              const std::vector<vec4> &uniforms = {}) override;

        Texture2D diffuseTexture;
        Texture2D specularTexture;

TEST_NODE_END(LightingMap)
#endif //GL_LIGHTINGMAP_H
