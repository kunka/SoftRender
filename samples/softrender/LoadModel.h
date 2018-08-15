//
// Created by huangkun on 2018/8/14.
//

#ifndef GL_LOADMODEL_H
#define GL_LOADMODEL_H

#include "TextureCube.h"
#include "Model.h"

TEST_NODE_BEGIN2(LoadModel, TextureCube)

    protected:
        virtual void setPixel(int x, int y, int z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms = {}) override;

        Model *modelObject;
TEST_NODE_END(LoadModel)

#endif //GL_LOADMODEL_H
