//
// Created by huangkun on 2018/6/17.
//

#include "Rast2DTriangle.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    Rast2DTriangle::Rast2DTriangle() {
        triangles = {
                vec2(TEX_WIDTH / 8, TEX_HEIGHT / 8),
                vec2(TEX_WIDTH / 2, TEX_HEIGHT / 4),
                vec2(TEX_WIDTH / 8 * 7, TEX_HEIGHT / 8 * 7),
        };
        // random
        for (int i = 0; i < 7; i++) {
            triangles.push_back(vec2(TEX_WIDTH * (rand() % 100 / 100.0f), TEX_HEIGHT * (rand() % 100 / 100.0f)));
            triangles.push_back(vec2(TEX_WIDTH * (rand() % 100 / 100.0f), TEX_HEIGHT * (rand() % 100 / 100.0f)));
            triangles.push_back(vec2(TEX_WIDTH * (rand() % 100 / 100.0f), TEX_HEIGHT * (rand() % 100 / 100.0f)));
        }
        clipRect.setRect(TEX_WIDTH / 4, TEX_WIDTH / 4, TEX_HEIGHT / 2, TEX_HEIGHT / 2);
    }

    void Rast2DTriangle::draw(const mat4 &transform) {
        memset(texData, 0, TEX_WIDTH * TEX_HEIGHT * 4);

        clipLine = Input::getInstance()->isKeyPressed(GLFW_KEY_C);

        for (int i = 0; i < triangles.size(); i += 3) {
            vec2 p1 = triangles[i];
            vec2 p2 = triangles[i + 1];
            vec2 p3 = triangles[i + 2];
            if (Input::getInstance()->isKeyPressed(GLFW_KEY_F)) {
                fill(p1, p2, p3, vec3(255, 0, 0), vec3(0, 255, 0), vec3(0, 0, 255));
            } else {
                dda_line(p1, p2, vec3(255, 0, 0), vec3(0, 255, 0));
                dda_line(p1, p3, vec3(255, 0, 0), vec3(0, 0, 255));
                dda_line(p2, p3, vec3(0, 255, 0), vec3(0, 0, 255));
            }
        }

        SoftRender::draw(transform);
    }

    struct VertexColor {
        vec2 p;
        vec3 color;
    };

    void Rast2DTriangle::fill(const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec3 &color, const vec3 &color2,
                              const vec3 &color3) {
        std::vector<VertexColor> ps = {{p1, color},
                                       {p2, color2},
                                       {p3, color3}};
        std::sort(ps.begin(), ps.end(), [](const VertexColor &a, const VertexColor &b) { return a.p.y > b.p.y; });
        vec2 max = ps[0].p;
        vec2 mid = ps[1].p;
        vec2 min = ps[2].p;
        vec3 maxColor = ps[0].color;
        vec3 midColor = ps[1].color;
        vec3 minColor = ps[2].color;

        if (mid.y == min.y || max.y == mid.y) {
            int dy = max.y - min.y;
            vec2 a, b;
            a.y = b.y = max.y;
            vec3 c1, c2;
            if (max.y == mid.y) {
                dda_line(mid, max, midColor, maxColor);
            } else {
                dda_line(min, mid, minColor, midColor);
            }
            for (int i = 1; i <= dy; i++) {
                float f = (float) i / dy;
                if (max.y == mid.y) {
                    a.x = mid.x + f * (min.x - mid.x);
                    c1 = interp(midColor, minColor, f);
                } else {
                    a.x = max.x + f * (mid.x - max.x);
                    c1 = interp(maxColor, midColor, f);
                }
                a.y -= 1;
                b.x = max.x + f * (min.x - max.x);
                c2 = interp(maxColor, minColor, f);
                b.y -= 1;
                dda_line(a, b, c1, c2);
            }
        } else {
            float dy = max.y - min.y;
            float dx = max.x - min.x;
            vec2 midP;
            midP.y = mid.y;
            vec3 c = interp(minColor, maxColor, midP.y / dy);
            if (dx == 0) {
                midP.x = max.x;
            } else {
                midP.x = max.x + (mid.y - max.y) * dx / dy;
            }
            fill(max, mid, midP, maxColor, midColor, c);
            fill(mid, midP, min, midColor, c, minColor);
        }
    }

    Rast2DTriangle::~Rast2DTriangle() {
    }

TEST_NODE_IMP_END