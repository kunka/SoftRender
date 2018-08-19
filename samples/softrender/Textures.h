//
// Created by huangkun on 2018/8/19.
//

#ifndef GL_TEXTURES_H
#define GL_TEXTURES_H

#include "TextureCube.h"

TEST_NODE_BEGIN2(Textures, TextureCube)

    public:
        virtual bool init() override;

    protected:

        virtual void setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms = {}) override;

        std::vector<float> verticesPlane;
        std::vector<unsigned int> indicesPlane;
        std::vector<Mesh *> planeMeshes;

TEST_NODE_END(Textures)

#endif //GL_TEXTURES_H
