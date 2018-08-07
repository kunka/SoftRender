//
// Created by huangkun on 2018/6/16.
//

#include "Rast2DPoint.h"

TEST_NODE_IMP_BEGIN

    Rast2DPoint::Rast2DPoint() {

    }

    void Rast2DPoint::draw(const mat4 &transform) {
        memset(texData, 0, TEX_WIDTH * TEX_HEIGHT * 4);
        int i, j;

#define POINTS_NUM 9
        vec2 points[POINTS_NUM] = {vec2(TEX_WIDTH / 4, TEX_HEIGHT / 4),
                                   vec2(TEX_WIDTH / 2, TEX_HEIGHT / 4),
                                   vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4),
                                   vec2(TEX_WIDTH / 4, TEX_HEIGHT / 2),
                                   vec2(TEX_WIDTH / 2, TEX_HEIGHT / 2),
                                   vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 2),
                                   vec2(TEX_WIDTH / 4, TEX_HEIGHT / 4 * 3),
                                   vec2(TEX_WIDTH / 2, TEX_HEIGHT / 4 * 3),
                                   vec2(TEX_WIDTH / 4 * 3, TEX_HEIGHT / 4 * 3),};

        vec3 color(255, 0, 0);
        for (i = 0; i < POINTS_NUM; i++) {
            vec2 p = points[i];
            setPixel(p.x, p.y, 0, color);
        }

        SoftRender::draw(transform);
    }

    Rast2DPoint::~Rast2DPoint() {
    }

TEST_NODE_IMP_END