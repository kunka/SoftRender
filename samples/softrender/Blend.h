//
// Created by huangkun on 2018/8/15.
//

#ifndef GL_BLEND_H
#define GL_BLEND_H

#include "TextureCube.h"

TEST_NODE_BEGIN2(Blend, TextureCube)

    protected:

        virtual void setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms = {}) override;

        std::vector<float> verticesPlane;
        std::vector<unsigned int> indicesPlane;

        Texture2D texture2DBox;
        Texture2D texture2DPlane;
        Texture2D texture2DGrass;
        Texture2D texture2DWindow;

        std::vector<Mesh *> planeMeshes;
        std::vector<Mesh *> boxMeshes;
TEST_NODE_END(Blend)

#endif //GL_BLEND_H
