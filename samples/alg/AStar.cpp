//
// Created by huangkun on 2018/4/24.
//

#include "AStar.h"
#include "Input.h"

using namespace std;
TEST_NODE_IMP_BEGIN

    AStar::AStar() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec2 a_position;
out vec2 pos;

void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
    pos = vec2(a_position);
}
)";

        const char *frag = R"(
#version 330 core
out vec4 FragColor;
in vec2 pos;

layout (std140) uniform Map
{
    vec4 map[1200];
    vec4 visitTimes[1200];
//    int map[1200];
//    float visitTimes[1200];
};

void main()
{
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    int width = 40;
    int height = 30;
    int x = int((pos.x/2 + 0.5) * width);
    int y = int((pos.y/2 + 0.5) * height);
    int index = x + y * width;

    int gap = 20;
    int x1 = int((pos.x/2 + 0.5) * width * gap);
    int y1 = int((pos.y/2 + 0.5) * height * gap);
    if (x1 % gap == 0 || y1 % gap == 0)
    {
        // grid line
        color = vec4(0.5, 0.5, 0.5, 0.5);
    }
    else
    {
        if (map[index].r == 1)
        {
            // block
            color = vec4(0.5, 0.5, 0.5, 1.0);
        }
        else if (visitTimes[index].r == -1)
        {
            // from
            color = vec4(1, 0.5, 0.5, 1.0);
        }
        else if (visitTimes[index].r == -2)
        {
            // to
            color = vec4(0, 1.0, 1.0, 1.0);
        }
        else if (visitTimes[index].r == -3)
        {
            // route history
            color = vec4(1.0, 1.0, 0, 1.0f);
        }
        else if (visitTimes[index].r == -4)
        {
            // route path
            color = vec4(1.0, 0, 1.0, 1.0);
        }
        else if (visitTimes[index].r > 0)
        {
            // route history
            color = vec4(0, visitTimes[index].r, 0, 1.0);
        }
        else if (map[index].r == 0)
        {
            // empty grid
            color = vec4(0.86, 0.86, 0.86, 1.0);
        }
        else
        {
            color = vec4(0, 0, 0, 1.0);
        }
    }

    FragColor = color;
}
)";
        shader.loadStr(vert, frag);

        float vertices[] = {
                1.0f, 1.0f,  // top right
                1.0f, -1.0f, // bottom right
                -1.0f, -1.0f,  // bottom left
                -1.0f, 1.0f,   // bottom top
        };
        unsigned int indices[] = { // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        width = 40;
        height = 30;

        shader.use();
        unsigned int bindingPoint = 1;
        unsigned int index = glGetUniformBlockIndex(shader.ID, "Map");
        glUniformBlockBinding(shader.ID, index, bindingPoint);
        int blockSize = 0;
        glGetActiveUniformBlockiv(shader.ID, index, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        log("blockSize = %d", blockSize);

        glGenBuffers(1, &UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        auto mapSize = 16 * width * height;
        auto visitTimesSize = 16 * width * height;
        log("mapSize = %d", mapSize);
        log("visitTimesSize = %d", visitTimesSize);
        glBufferData(GL_UNIFORM_BUFFER, mapSize + visitTimesSize, NULL, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // gen map
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                map.push_back(0);
            }
        }
        // add block
        for (int i = 18; i < 28; i++) {
            int j = 5;
            map[i + j * width] = 1;
            j = 20;
            map[i + j * width] = 1;
        }
        for (int j = 5; j <= 20; j++) {
            int i = 28;
            map[i + j * width] = 1;
        }

        from = vec2(8, 10);
        to = vec2(36, 15);

        resetVisitTimes();
//        thread = new std::thread(&AStar::doDJT, this);
//        thread = new std::thread(&AStar::doBFS, this);
        thread = new std::thread(&AStar::doAStar, this);
    }

    void AStar::resetVisitTimes() {
        visitTimes.clear();
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                visitTimes.push_back(0);
            }
        }
        // from
        visitTimes[from.x + from.y * width] = -1;
        // to
        visitTimes[to.x + to.y * width] = -2;
    }

    void AStar::doDJT() {
        slow = true;
        std::vector<int> path;
        unordered_set<int> open_set;
        unordered_set<int> close_set;
        unordered_map<int, int> came_from;
        unordered_map<int, int> g_score;
        unordered_map<int, float> h_score;
        std::map<int, float> f_score;
        auto indexOf = [&](vec2 &p) -> int {
            return int(p.x + p.y * width);
        };
        auto manhattanDis = [&](vec2 &from, vec2 &to) -> float {
//            return (abs(from.x - to.x) + abs(from.y - to.y));
            auto dp = from - to;
            return std::sqrt(dp.x * dp.x + dp.y * dp.y);
        };

        open_set.emplace(indexOf(from));
        float dis = 0;//manhattanDis(from, to);
        g_score.insert({indexOf(from), 0});
        h_score.insert({indexOf(from), dis});
        f_score.insert({indexOf(from), dis});
        while (!open_set.empty()) {
            float dis = INT_MAX;
            vec2 p;
            // 找出评估距离最小的节点
            for (auto iter = f_score.begin(); iter != f_score.end(); iter++) {
                if (close_set.find(iter->first) != close_set.end())
                    continue;
                if (iter->second < dis) {
                    dis = iter->second;
                    p.y = iter->first / width;
                    p.x = iter->first % width;
                }
            }
            mutex.lock();
            visitTimes[indexOf(p)] = -3;
            mutex.unlock();

            if (indexOf(p) == indexOf(to)) {
                reconstruct_path(path, came_from, indexOf(to));
                break;
            } else {
                // visualize the mini path from start pos we choose
                std::vector<int> tmpPath;
                reconstruct_path(tmpPath, came_from, indexOf(p));
                if (tmpPath.size() > 2) {
                    mutex.lock();
                    // route path
                    for (auto iter = tmpPath.rbegin(); iter != tmpPath.rend() - 1; iter++) {
                        int y = *iter / width;
                        int x = *iter % width;
                        visitTimes[*iter] = -4;
                    }
                    visitTimes[from.x + from.y * width] = -1;
                    visitTimes[to.x + to.y * width] = -2;
                    mutex.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(slow ? 1000 : 5));
                    // reset
                    mutex.lock();
                    // route path
                    for (auto iter = tmpPath.rbegin() + 1; iter != tmpPath.rend() - 1; iter++) {
                        int y = *iter / width;
                        int x = *iter % width;
                        visitTimes[*iter] = -3;
                    }
                    mutex.unlock();
                }
            }
            open_set.erase(indexOf(p));
            close_set.insert(indexOf(p));
            // 遍历相邻节点，8方向
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) {
                        // self;
                        continue;
                    }
                    vec2 y = vec2(p.x + i, p.y + j);
                    if (y.x < 0 || y.x >= width || y.y < 0 || y.y >= height)
                        // 边界
                        continue;
                    auto index = indexOf(y);
                    if (close_set.find(index) != close_set.end()) {
                        continue;
                    }
                    // block
                    if (map[index] > 0) {
                        continue;
                    }
                    auto dp = y - p;
//                    auto g_score_y = g_score.at(indexOf(p)) + std::sqrt(dp.x * dp.x + dp.y * dp.y);
                    auto g_score_y = g_score.at(indexOf(p)) + manhattanDis(y, p);

                    // visualize visit times
                    mutex.lock();
                    visitTimes[index] += 1;
                    mutex.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(slow ? 1000 : 5));

                    bool isBetter = false;
                    if (open_set.find(index) == open_set.end()) {
                        // 新节点，加入open集
                        open_set.insert(index);
                        isBetter = true;
                    } else if (g_score_y < g_score.at(index)) {
                        // 旧节点y，离起点距离小于实际距离
                        isBetter = true;
                    } else {
                        isBetter = false;
                    }
                    if (isBetter) {
                        came_from.insert({indexOf(y), indexOf(p)});
                        float dis = 0;//manhattanDis(y, to);
                        g_score.insert({index, g_score_y}); // 更新实际距离
                        h_score.insert({index, dis});
                        f_score.insert({index, dis + g_score_y});
                    }
                }
            }
        }

        mutex.lock();
        // route path
        for (auto iter = path.rbegin() + 1; iter != path.rend() - 1; iter++) {
            int y = *iter / width + 1;
            int x = *iter % width + 1;
            log("%d,%d", x, y);
            visitTimes[*iter] = -4;
        }
        visitTimes[from.x + from.y * width] = -1;
        visitTimes[to.x + to.y * width] = -2;
        mutex.unlock();
    }

    void AStar::doBFS() {
        std::vector<int> path;
        unordered_set<int> open_set;
        unordered_set<int> close_set;
        unordered_map<int, int> came_from;
//        unordered_map<int, int> g_score;
        unordered_map<int, float> h_score;
        std::map<int, float> f_score;
        auto indexOf = [&](vec2 &p) -> int {
            return int(p.x + p.y * width);
        };
        auto manhattanDis = [&](vec2 &from, vec2 &to) -> float {
            return (abs(from.x - to.x) + abs(from.y - to.y));
        };

        open_set.emplace(indexOf(from));
        float dis = manhattanDis(from, to);
        h_score.insert({indexOf(from), dis});
        f_score.insert({indexOf(from), dis});
        while (!open_set.empty()) {
            float dis = INT_MAX;
            vec2 p;
            for (auto iter = f_score.begin(); iter != f_score.end(); iter++) {
                if (close_set.find(iter->first) != close_set.end())
                    continue;
                if (iter->second < dis) {
                    dis = iter->second;
                    p.y = iter->first / width;
                    p.x = iter->first % width;
                }
            }
            mutex.lock();
            visitTimes[indexOf(p)] = -3;
            mutex.unlock();

            if (indexOf(p) == indexOf(to)) {
                reconstruct_path(path, came_from, indexOf(to));
                break;
            }
            open_set.erase(indexOf(p));
            close_set.insert(indexOf(p));
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) {
                        // self;
                        continue;
                    }
                    vec2 y = vec2(p.x + i, p.y + j);
                    if (y.x < 0 || y.x >= width || y.y < 0 || y.y >= height)
                        continue;
                    auto index = indexOf(y);
                    if (index < 0 || index >= width * height ||
                        close_set.find(index) != close_set.end()) {
                        continue;
                    }
                    // block
                    if (map[index] > 0) {
                        continue;
                    }
                    mutex.lock();
                    visitTimes[index] += 1;
                    mutex.unlock();
                    bool isBetter = false;
                    if (open_set.find(index) == open_set.end()) {
                        open_set.insert(index);
                        isBetter = true;
                    } else {
                        isBetter = false;
                    }
                    if (isBetter) {
                        came_from.insert({indexOf(y), indexOf(p)});
                        float dis = manhattanDis(y, to);
                        h_score.insert({index, dis});
                        f_score.insert({index, dis});
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(slow ? 1000 : 5));
                }
            }
        }

        mutex.lock();
        // route path
        for (auto iter = path.rbegin() + 1; iter != path.rend() - 1; iter++) {
            int y = *iter / width;
            int x = *iter % width;
            log("%d,%d", x, y);
            visitTimes[*iter] = -4;
        }
        // from
        visitTimes[from.x + from.y * width] = -1;
        // to
        visitTimes[to.x + to.y * width] = -2;
        mutex.unlock();
    }

    void AStar::doAStar() {
        slow = true;
        std::vector<int> path;
        unordered_set<int> open_set;
        unordered_set<int> close_set;
        unordered_map<int, int> came_from;
        unordered_map<int, int> g_score;
        unordered_map<int, float> h_score;
        std::map<int, float> f_score;
        auto indexOf = [&](vec2 &p) -> int {
            return int(p.x + p.y * width);
        };
        auto manhattanDis = [&](vec2 &from, vec2 &to) -> float {
//            return (abs(from.x - to.x) + abs(from.y - to.y));
            auto dp = from - to;
            return std::sqrt(dp.x * dp.x + dp.y * dp.y);
        };

        open_set.emplace(indexOf(from));
        float dis = manhattanDis(from, to);
        g_score.insert({indexOf(from), 0});
        h_score.insert({indexOf(from), dis});
        f_score.insert({indexOf(from), dis});
        while (!open_set.empty()) {
            float dis = INT_MAX;
            vec2 p;
            // 找出评估距离最小的节点
            for (auto iter = f_score.begin(); iter != f_score.end(); iter++) {
                if (close_set.find(iter->first) != close_set.end())
                    continue;
                if (iter->second < dis) {
                    dis = iter->second;
                    p.y = iter->first / width;
                    p.x = iter->first % width;
                }
            }
            mutex.lock();
            visitTimes[indexOf(p)] = -3;
            mutex.unlock();

            if (indexOf(p) == indexOf(to)) {
                reconstruct_path(path, came_from, indexOf(to));
                break;
            } else {
                std::vector<int> tmpPath;
                reconstruct_path(tmpPath, came_from, indexOf(p));
                if (tmpPath.size() > 2) {
                    mutex.lock();
                    // route path
                    for (auto iter = tmpPath.rbegin(); iter != tmpPath.rend() - 1; iter++) {
                        int y = *iter / width;
                        int x = *iter % width;
                        visitTimes[*iter] = -4;
                    }
                    // from
                    visitTimes[from.x + from.y * width] = -1;
                    // to
                    visitTimes[to.x + to.y * width] = -2;
                    mutex.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(slow ? 1000 : 5));
                    // reset
                    mutex.lock();
                    // route path
                    for (auto iter = tmpPath.rbegin() + 1; iter != tmpPath.rend() - 1; iter++) {
                        int y = *iter / width;
                        int x = *iter % width;
                        visitTimes[*iter] = -3;
                    }
                    mutex.unlock();
                }
            }
            open_set.erase(indexOf(p));
            close_set.insert(indexOf(p));
            // 遍历相邻节点
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) {// self;
                        continue;
                    }
                    vec2 y = vec2(p.x + i, p.y + j);
                    if (y.x < 0 || y.x >= width || y.y < 0 || y.y >= height)
                        continue;
                    auto index = indexOf(y);
                    if (close_set.find(index) != close_set.end()) {
                        continue;
                    }
                    // block
                    if (map[index] > 0) {
                        continue;
                    }
                    auto dp = y - p;
//                    auto g_score_y = g_score.at(indexOf(p)) + std::sqrt(dp.x * dp.x + dp.y * dp.y);
                    auto g_score_y = g_score.at(indexOf(p)) + manhattanDis(y, p);

                    // visualize
                    mutex.lock();
                    visitTimes[index] += 1;
                    mutex.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(slow ? 1000 : 5));

                    bool isBetter = false;
                    if (open_set.find(index) == open_set.end()) {
                        // 新节点，加入open集
                        open_set.insert(index);
                        isBetter = true;
                    } else if (g_score_y < g_score.at(index)) {
                        // 旧节点y，离起点距离小于实际距离
                        isBetter = true;
                    } else {
                        isBetter = false;
                    }
                    if (isBetter) {
                        came_from.insert({indexOf(y), indexOf(p)});
                        float dis = manhattanDis(y, to);
                        g_score.insert({index, g_score_y}); // 更新实际距离
                        h_score.insert({index, dis});
                        f_score.insert({index, dis + g_score_y});
                    }
                }
            }
        }

        mutex.lock();
        // route path
        for (auto iter = path.rbegin() + 1; iter != path.rend() - 1; iter++) {
            int y = *iter / width;
            int x = *iter % width;
            log("%d,%d", x, y);
            visitTimes[*iter] = -4;
        }
        // from
        visitTimes[from.x + from.y * width] = -1;
        // to
        visitTimes[to.x + to.y * width] = -2;
        mutex.unlock();
    }

    void AStar::reconstruct_path(std::vector<int> &path, std::unordered_map<int, int> &came_from, int current) {
        path.push_back(current);
        if (came_from.find(current) != came_from.end()) {
            reconstruct_path(path, came_from, came_from.at(current));
        }
    }

    void AStar::drawMap() {
        // std140 require align memory
        mutex.lock();
        float max = 10.0f;
        for (auto iter = visitTimes.begin(); iter != visitTimes.end(); iter++) {
            if (*iter > max) {
                max = *iter;
            }
        }
        std::vector<vec4> map2;
        std::vector<vec4> visitTimes2;
        for (int i = 0; i < width * height; i++) {
            map2.push_back(vec4(map[i], 0, 0, 0));
            // normalize
            if (visitTimes[i] > 0) {
                visitTimes2.push_back(vec4(1.0f - visitTimes[i] / max, 0, 0, 0));
            } else {
                visitTimes2.push_back(vec4(visitTimes[i], 0, 0, 0));
            }
        }
        mutex.unlock();
        auto mapSize = 16 * width * height;
        auto visitTimesSize = 16 * width * height;

        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        // define the range of the buffer that links to a uniform binding point
//        glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, mapSize + visitTimesSize);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, mapSize, &map2[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, mapSize, visitTimesSize, &visitTimes2[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        CHECK_GL_ERROR_DEBUG();
    }

    void AStar::draw(const mat4 &transform) {
        drawMap();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        shader.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void AStar::fixedUpdate(float delta) {
        Input *input = Input::getInstance();
        if (input->isKeyPressed(GLFW_KEY_S) == GLFW_PRESS && !slowDownPressed) {
            slow = !slow;
            slowDownPressed = true;
        }
        if (input->isKeyPressed(GLFW_KEY_S) == GLFW_RELEASE) {
            slowDownPressed = false;
        }
    }

    AStar::~AStar() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        thread->join();
        delete thread;
    }

TEST_NODE_IMP_END