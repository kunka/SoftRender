//
// Created by huangkun on 2018/8/19.
//

#ifndef GL_TEXTUREFILTER_H
#define GL_TEXTUREFILTER_H

#include "TextureCube.h"

TEST_NODE_BEGIN2(TextureFilter, TextureCube)

    public:
        virtual bool init() override;

    protected:

        virtual void setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms, float dudx, float dvdy) override;

        std::vector<float> verticesPlane;
        std::vector<unsigned int> indicesPlane;
        std::vector<Mesh *> planeMeshes;
        Texture2D texture2D2;

TEST_NODE_END(TextureFilter)

#endif //GL_TEXTUREFILTER_H
