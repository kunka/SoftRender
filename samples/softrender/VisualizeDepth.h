//
// Created by huangkun on 2018/8/15.
//

#ifndef GL_VISUALIZEDEPTH_H
#define GL_VISUALIZEDEPTH_H

#include "TextureCube.h"

TEST_NODE_BEGIN2(VisualizeDepth, TextureCube)

    protected:

        virtual void setPixel(int x, int y, int z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms = {}) override;


TEST_NODE_END(VisualizeDepth)


#endif //GL_VISUALIZEDEPTH_H
