//
// Created by huangkun on 2018/8/24.
//

#ifndef GL_NORMALMAP_H
#define GL_NORMALMAP_H

#include "TextureCube.h"

TEST_NODE_BEGIN2(NormalMap, TextureCube)

    public:
        virtual bool init() override;

    protected:

        virtual void setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms, float dudx, float dvdy) override;

        std::vector<float> verticesPlane;
        std::vector<unsigned int> indicesPlane;

        Texture2D texture2DNormal;
        std::vector<Mesh *> planeMeshes;

        vec3 lightPos;

TEST_NODE_END(NormalMap)

#endif //GL_NORMALMAP_H
