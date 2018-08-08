//
// Created by huangkun on 2018/8/8.
//

#include "Lighting.h"

TEST_NODE_IMP_BEGIN

    Lighting::Lighting() {
    }

    void Lighting::draw(const mat4 &transform) {
        setDepthTest(true);
        setFaceCull(true);
        clearColor(50, 50, 50, 255);
        clearDepth();

        Matrix model;
        model.rotate(Vector(0, 1, 0), 2 * 3.14f * sin(glfwGetTime() / 4));
        Matrix identity;
        identity.rotate(Vector(1, 0, 0), radians(-30.0f));
        model.mult(identity);
        vec3 target = cameraPos + cameraDir;
        viewMatrix.lookAt(Vector(cameraPos.x, cameraPos.y, cameraPos.z), Vector(target.x, target.y, target.z),
                          Vector(cameraUp.x, cameraUp.y, cameraUp.z));
        Matrix m = model;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        vec2 last;
        int column = 5;
        for (int i = 0; i < vertices.size(); i += column * 3) {
            vec4 triangle[3];
            vec3 triangleWorld[3];
            vec2 uv[3];
            for (int j = 0; j < 3; j++) {
                int row = i + j * column;
                vec4 p = vec4(vertices.at(row), vertices.at(row + 1),
                              vertices.at(row + 2), 1.0);
                // 模型 --> 世界 --> 相机空间 --> 齐次裁剪空间，xyz ~ [-w，w], w = Z(相机空间)
                Vector v = m.apply(Vector(p.x, p.y, p.z));
                triangle[j] = vec4(v.x, v.y, v.z, v.w);
                uv[j] = vec2(vertices.at(row + 3), vertices.at(row + 4));

                Vector v0 = model.apply(Vector(p.x, p.y, p.z));
                triangleWorld[j] = vec3(v0.x, v0.y, v0.z);
            }
            if (cvvCull(triangle)) {
                // 简单CVV裁剪，三角形3个点有一个不在cvv立方体内将被裁剪掉
                // log("cvv cull");
                continue;
            }

            if (faceCull(triangleWorld)) {
                // 背面剔除
                // log("backface cull");
                continue;
            }
            // 转换为屏幕坐标
            pointToScreen(triangle);

            // 光栅化
            fill(triangle[0], triangle[1], triangle[2], uv[0] / triangle[0].w, uv[1] / triangle[1].w,
                 uv[2] / triangle[2].w);
        }
        SoftRender::draw(transform);
        setDepthTest(false);
        setFaceCull(false);
    }

    Lighting::~Lighting() {
    }

TEST_NODE_IMP_END