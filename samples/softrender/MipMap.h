//
// Created by huangkun on 2018/8/19.
//

#ifndef GL_MIPMAP_H
#define GL_MIPMAP_H

#include "TextureCube.h"

TEST_NODE_BEGIN2(MipMap, TextureCube)

    public:
        virtual bool init() override;

    protected:

        virtual void setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms, float dudx, float dudy) override;

        std::vector<float> verticesPlane;
        std::vector<unsigned int> indicesPlane;
        std::vector<Mesh *> planeMeshes;
        Texture2D texture2DMM;


TEST_NODE_END(TextureFilter)
#endif //MipMap
