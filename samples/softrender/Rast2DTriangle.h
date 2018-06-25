//
// Created by huangkun on 2018/6/17.
//

#ifndef GL_RAST2DTRIANGLE_H
#define GL_RAST2DTRIANGLE_H


#include "Rast2DLine.h"

TEST_NODE_BEGIN2(Rast2DTriangle, Rast2DLine)

    protected:

        void fill(const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec3 &color = vec3(255, 0, 0),
                  const vec3 &color2 = vec3(255, 0, 0), const vec3 &color3 = vec3(255, 0, 0));

        std::vector<vec2> triangles;
TEST_NODE_END(Rast2DTriangle)

#endif //GL_RAST2DTRIANGLE_H
