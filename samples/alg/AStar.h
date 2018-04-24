//
// Created by huangkun on 2018/4/24.
//

#ifndef GL_ASTAR_H
#define GL_ASTAR_H

#include "CustomDraw.h"

TEST_NODE_BEGIN(AStar)

        unsigned int UBO;
        int width;
        int height;
        std::vector<int> map;
        std::vector<float> visitTimes;

        void drawMap();

TEST_NODE_END(AStar)

#endif //GL_ASTAR_H
