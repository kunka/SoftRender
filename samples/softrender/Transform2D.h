//
// Created by huangkun on 2018/6/17.
//

#ifndef GL_TRANSFORM2D_H
#define GL_TRANSFORM2D_H

#include "Rast2DTriangle.h"
#include "Math.h"

TEST_NODE_BEGIN2(Transform2D, Rast2DTriangle)

    protected:
        std::vector<float> vertices;

TEST_NODE_END(Transform2D)


#endif //GL_TRANSFORM2D_H
