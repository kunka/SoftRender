//
// Created by huangkun on 2018/4/24.
//

#ifndef GL_ASTAR_H
#define GL_ASTAR_H

#include "CustomDraw.h"
#include <unordered_map>
#include <unordered_set>
#include <thread>

TEST_NODE_BEGIN(AStar)

        unsigned int UBO;
        int width;
        int height;
        std::vector<int> map;
        std::vector<float> visitTimes;

        void drawMap();

    private:
        void resetVisitTimes();

        void doAStar();

        void reconstruct_path(std::vector<int> &path, std::unordered_map<int, int> &came_from, int current);

        vec2 from;
        vec2 to;
        std::thread *thread;
        std::mutex mutex;

TEST_NODE_END(AStar)

#endif //GL_ASTAR_H
