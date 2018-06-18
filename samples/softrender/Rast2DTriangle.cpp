//
// Created by huangkun on 2018/6/17.
//

#include "Rast2DTriangle.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    Rast2DTriangle::Rast2DTriangle() {
        triangles = {
                vec2(TEX_WIDTH / 4, TEX_HEIGHT / 4),
                vec2(TEX_WIDTH / 2, TEX_HEIGHT / 4),
                vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4 * 3),
        };
        // random
//        for (int i = 0; i < 7; i++) {
//            triangles.push_back(vec2(TEX_WIDTH * (rand() % 100 / 100.0f), TEX_HEIGHT * (rand() % 100 / 100.0f)));
//            triangles.push_back(vec2(TEX_WIDTH * (rand() % 100 / 100.0f), TEX_HEIGHT * (rand() % 100 / 100.0f)));
//            triangles.push_back(vec2(TEX_WIDTH * (rand() % 100 / 100.0f), TEX_HEIGHT * (rand() % 100 / 100.0f)));
//        }
    }

    void Rast2DTriangle::draw(const mat4 &transform) {
        memset(texData, 0, TEX_WIDTH * TEX_HEIGHT * 4);

        for (int i = 0; i < triangles.size(); i += 3) {
            vec2 p1 = triangles[i];
            vec2 p2 = triangles[i + 1];
            vec2 p3 = triangles[i + 2];
            if (Input::getInstance()->isKeyPressed(GLFW_KEY_F)) {
                fill(p1, p2, p3);
            } else {
                dda_line(p1, p2);
                dda_line(p1, p3);
                dda_line(p2, p3);
            }
        }

        SoftRender::draw(transform);
    }

    void Rast2DTriangle::fill(const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec3 &color) {
        std::vector<vec2> ps = {p1, p2, p3};
        std::sort(ps.begin(), ps.end(), [](const vec2 &a, const vec2 &b) { return a.y > b.y; });
        vec2 max = ps[0];
        vec2 mid = ps[1];
        vec2 min = ps[2];
        if (mid.y == min.y || max.y == mid.y) {
            int dy = max.y - min.y;
            vec2 a, b;
            a.y = b.y = max.y;
            for (int i = 1; i <= dy; i++) {
                float f = (float) i / dy;
                if (max.y == mid.y)
                    a.x = mid.x + f * (min.x - mid.x);
                else
                    a.x = max.x + f * (mid.x - max.x);
                a.y -= 1;
                b.x = max.x + f * (min.x - max.x);
                b.y -= 1;
                dda_line(a, b, color);
            }
        } else {
            float dy = max.y - min.y;
            float dx = max.x - min.x;
            vec2 midP;
            midP.y = mid.y;
            if (dx == 0) {
                midP.x = max.x;
            } else {
                midP.x = max.x + (mid.y - max.y) * dx / dy;
            }
            fill(max, mid, midP, color);
            fill(mid, midP, min, color);
        }
    }

    Rast2DTriangle::~Rast2DTriangle() {
    }

TEST_NODE_IMP_END