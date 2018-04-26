//
// Created by huangkun on 2018/4/24.
//

#ifndef GL_ASTAR_H
#define GL_ASTAR_H

#include "CustomDraw.h"
#include <unordered_map>
#include <map>
#include <unordered_set>

#ifdef _WIN32

#include "mingw.thread.h"
#include "mingw.mutex.h"

#else

#include <thread>

#endif

TEST_NODE_BEGIN(AStar)

        unsigned int UBO;
        int width;
        int height;
        std::vector<int> map;
        std::vector<float> visitTimes;

        void drawMap();

        virtual void fixedUpdate(float delta) override;

    private:
        void resetVisitTimes();

        void doDJT();

        void doBFS();

        void doAStar();

        void reconstruct_path(std::vector<int> &path, std::unordered_map<int, int> &came_from, int current);

        vec2 from;
        vec2 to;
        std::thread *thread;
        std::mutex mutex;
        bool slow = false;
        bool slowDownPressed = false;

TEST_NODE_END(AStar)

#endif //GL_ASTAR_H
