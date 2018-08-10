//
// Created by huangkun on 2018/6/16.
//

#include "Rast2DLine.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    Rast2DLine::Rast2DLine() {
        lines = {
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
        // random
        for (int i = 0; i < 20; i++) {
            lines.push_back(vec2(TEX_WIDTH * (rand() % 100 / 100.0f), TEX_HEIGHT * (rand() % 100 / 100.0f)));
            lines.push_back(vec2(TEX_WIDTH * (rand() % 100 / 100.0f), TEX_HEIGHT * (rand() % 100 / 100.0f)));
        }
    }

    void Rast2DLine::draw(const mat4 &transform) {
        clipRect.setRect(TEX_WIDTH / 4, TEX_WIDTH / 4, TEX_HEIGHT / 2, TEX_HEIGHT / 2);
        memset(texData, 0, TEX_WIDTH * TEX_HEIGHT * 4);

        isClipRect = Input::getInstance()->isKeyPressed(GLFW_KEY_C);
        for (int i = 0; i < lines.size(); i += 2) {
            vec2 p1 = lines[i];
            vec2 p2 = lines[i + 1];
            dda_line(p1, p2, vec3(255, 0, 0), vec3(0, 255, 0));
        }

        SoftRender::draw(transform);
    }

    bool Rast2DLine::clip_a_line(vec2 &p1, vec2 &p2, int minX, int maxX, int minY, int maxY) {
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
                code1 = encode(p1, minX, maxX, minY, maxY);
                if (code1 == 0 and code2 == 0) {
                    return true;
                } else if (code1 & code2) {
                    return false;
                } else {
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
    }

    /*
     * Cohen-Sutherland
     *
     *  8+1    8     8+2
     *      -------
     *     |       |
     *   1 |   0   |  2
     *     |       |
     *      -------
     *  4+1    4     4+2
     */
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

    void Rast2DLine::dda_line(const vec2 &pa, const vec2 &pb, const vec3 &color, const vec3 &color2) {
        vec2 p1 = pa;
        vec2 p2 = pb;

        if (isClipRect &&
            !clip_a_line(p1, p2, clipRect.getMinX(), clipRect.getMaxX(), clipRect.getMinY(), clipRect.getMaxY())) {
            return;
        }
        float dy = p2.y - p1.y;
        float dx = p2.x - p1.x;
        float stepX, stepY;
        int steps;
        if (abs(dy) > abs(dx)) {
            steps = abs((int) dy);
            stepY = dy > 0 ? 1 : -1;
            stepX = dx > 0 ? fabs(dx / dy) : -fabs(dx / dy);
        } else {
            steps = abs((int) dx);
            stepX = dx > 0 ? 1 : -1;
            if (dx == 0)
                stepY = 0;
            else
                stepY = dy > 0 ? fabs(dy / dx) : -fabs(dy / dx);
        }
        float x = p1.x, y = p1.y;
        setPixel(x, y, 0, color);
        for (int k = 1; k <= steps; k++) {
            x += stepX;
            y += stepY;
            if (k == steps) {
                setPixel(x, y, 0, color2);
            } else
                setPixel(x, y, 0, interp(color, color2, 1.0f * k / steps));
        }
        setPixel(p2.x, p2.y, 0, color2);
    }

    Rast2DLine::~Rast2DLine() {
    }

TEST_NODE_IMP_END