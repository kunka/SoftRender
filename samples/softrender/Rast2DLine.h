//
// Created by huangkun on 2018/6/16.
//

#ifndef GL_RASTLINE_H
#define GL_RASTLINE_H

#include "SoftRender.h"

TEST_NODE_BEGIN2(Rast2DLine, SoftRender)

    protected:
        void dda_line(const vec2 &p1, const vec2 &p2, const vec3 &color = vec3(255, 0, 0),
                      const vec3 &color2 = vec3(255, 0, 0));

        bool clip_a_line(vec2 &p1, vec2 &p2, int minX, int maxX, int minY, int maxY);

        int encode(const vec2 &p, int minX, int maxX, int minY, int maxY);

        std::vector<vec2> lines;
        Rect clipRect;
        bool clipLine = false;
TEST_NODE_END(Rast2DLine)

#endif //GL_RASTLINE_H
