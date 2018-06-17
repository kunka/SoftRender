//
// Created by huangkun on 2018/6/16.
//

#include "Rast2DLine.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    Rast2DLine::Rast2DLine() {

    }

    void Rast2DLine::draw(const mat4 &transform) {
        memset(texData, 0, TEX_WIDTH * TEX_HEIGHT * 4);
        int i, j;

        std::vector<vec2> lines = {
                vec2(TEX_WIDTH / 4, TEX_HEIGHT / 4),
                vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4),
                vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4),
                vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4 * 3),
                vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4 * 3),
                vec2(TEX_WIDTH / 4, TEX_HEIGHT / 4 * 3),
                vec2(TEX_WIDTH / 4, TEX_HEIGHT / 4 * 3),
                vec2(TEX_WIDTH / 4, TEX_HEIGHT / 4),

                // test clip
                vec2(TEX_WIDTH / 4 - TEX_WIDTH / 8, TEX_HEIGHT / 4 - TEX_HEIGHT / 16),
                vec2(TEX_WIDTH / 4 * 3 + TEX_WIDTH / 8, TEX_HEIGHT / 2 + TEX_HEIGHT / 16),
                vec2(TEX_WIDTH / 4 - TEX_WIDTH / 8, TEX_HEIGHT / 4 * 3 + TEX_HEIGHT / 8),
                vec2(TEX_WIDTH / 4 * 3 + TEX_WIDTH / 8, TEX_HEIGHT / 4 - TEX_HEIGHT / 8),
                vec2(TEX_WIDTH / 8, TEX_HEIGHT / 8),
                vec2(TEX_WIDTH / 4 * 3 + TEX_HEIGHT / 8, TEX_HEIGHT / 8),
                vec2(TEX_WIDTH / 8, TEX_HEIGHT / 2),
                vec2(TEX_WIDTH / 4 * 3 + TEX_HEIGHT / 8, TEX_HEIGHT / 2),
                vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4 * 3),
                vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4),
                vec2(TEX_WIDTH / 8, TEX_HEIGHT / 4 * 3),
                vec2(TEX_WIDTH / 4 * 3 + TEX_WIDTH / 8, TEX_HEIGHT / 4 * 3),
        };
        for (i = 0; i < lines.size(); i += 2) {
            vec2 p1 = lines[i];
            vec2 p2 = lines[i + 1];
            if (clip_a_line(p1, p2, TEX_WIDTH / 4, TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4, TEX_HEIGHT / 4 * 3)) {
//            if (clip_a_line(p1, p2, TEX_WIDTH / 3, TEX_WIDTH / 3 * 2, TEX_HEIGHT / 3, TEX_HEIGHT / 3 * 2)) {
                dda_line(p1, p2);
            }
        }

        SoftRender::draw(transform);
    }

    bool Rast2DLine::clip_a_line(vec2 &p1, vec2 &p2, int minX, int maxX, int minY, int maxY) {
        if (!Input::getInstance()->isKeyPressed(GLFW_KEY_C)) {
            return true;
        }
        float dy = p2.y - p1.y;
        float dx = p2.x - p1.x;
        float m = dx == 0 ? 0 : dy / dx;
        while (true) {
            int code1 = encode(p1, minX, maxX, minY, maxY);
            int code2 = encode(p2, minX, maxX, minY, maxY);
            if (code1 == 0 and code2 == 0) {
                return true;
            } else if (code1 & code2) {
                return false;
            } else {
                if (code1 & 1) {
                    p1.y += m * (minX - p1.x);
                    p1.x = minX;
                } else if (code1 & 2) {
                    p1.y += m * (maxX - p1.x);
                    p1.x = maxX;
                } else if (code1 & 4) {
                    p1.x += (minY - p1.y) / m;
                    p1.y = minY;
                } else if (code1 & 8) {
                    p1.x += (maxY - p1.y) / m;
                    p1.y = maxY;
                }

                if (code2 & 1) {
                    p2.y += m * (minX - p2.x);
                    p2.x = minX;
                } else if (code2 & 2) {
                    p2.y += m * (maxX - p2.x);
                    p2.x = maxX;
                } else if (code2 & 4) {
                    p2.x += (minY - p2.y) / m;
                    p2.y = minY;
                } else if (code2 & 8) {
                    p2.x += (maxY - p2.y) / m;
                    p2.y = maxY;
                }
            }
        }
    }

    int Rast2DLine::encode(const vec2 &p, int minX, int maxX, int minY, int maxY) {
        int code = 0;
        if (p.x < minX)
            code |= 1;
        else if (p.x > maxX)
            code |= 2;
        if (p.y < minY)
            code |= 4;
        else if (p.y > maxY)
            code |= 8;
        return code;
    }

    void Rast2DLine::dda_line(const vec2 &p1, const vec2 &p2) {
        vec3 color(255, 0, 0);
        float dy = p2.y - p1.y;
        float dx = p2.x - p1.x;
        float stepX, stepY;
        int steps;
        if (abs(dy) > abs(dx)) {
            steps = abs((int) dy);
            stepY = p2.y > p1.y ? 1 : -1;
            stepX = dx == 0 ? 0 : dy / dx;
        } else {
            steps = abs((int) dx);
            stepX = p2.x > p1.x ? 1 : -1;
            stepY = dx == 0 ? 0 : dy / dx;
        }
        float x = p1.x, y = p1.y;
        setPixel(x, y, color);
        for (int k = 1; k <= steps; k++) {
            x += stepX;
            y += stepY;
            setPixel(x, y, color);
        }
    }

    Rast2DLine::~Rast2DLine() {
    }

TEST_NODE_IMP_END