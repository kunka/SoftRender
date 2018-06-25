//
// Created by huangkun on 2018/6/17.
//

#include "RastCube.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    RastCube::RastCube() {
        vertices = {
                // postions        // color
                // Back face
                -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // top-right
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom-left
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // top-left
                // Front face
                -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top-right
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// top-right
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// top-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,// bottom-left
                // Left face
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// top-right
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // top-left
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-left
                -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,// bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// bottom-right
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,// top-right
                // Right face
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// top-left
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// top-right
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // bottom-right
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top-left
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,// bottom-left
                // Bottom face
                -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,// top-right
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // top-left
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,// bottom-left
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// bottom-right
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // top-right
                // Top face
                -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,// top-left
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// top-right
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// bottom-right
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// top-left
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,// bottom-left
        };
    }

    void RastCube::draw(const mat4 &transform) {
        memset(texData, 0, TEX_WIDTH * TEX_HEIGHT * 4);

        Matrix model;
        model.rotate(Vector(0, 1, 0), radians(30.0f));
//        model.rotate(Vector(0, 1, 0), 2 * 3.14f * sin(glfwGetTime() / 4));
        Matrix view;
        vec3 target = cameraPos + cameraDir;
//        view.lookAt(Vector(0, 0, 4), Vector(0, 0, 0), Vector(0, 1, 0));
//        view.translate(Vector(0, 0, -4));
        view.lookAt(Vector(cameraPos.x, cameraPos.y, cameraPos.z), Vector(target.x, target.y, target.z),
                    Vector(cameraUp.x, cameraUp.y, cameraUp.z));
        Matrix projection;
        projection.perspective(radians(60.0f), (float) TEX_WIDTH / TEX_HEIGHT, 0.1, 100.0f);
        Matrix m = model;
        m.mult(view);
        m.mult(projection);
        vec2 last;
        for (int i = 0; i < vertices.size(); i += 6 * 3) {
            vec4 triangle[3];
            vec3 color[3];
            for (int j = 0; j < 3; j++) {
                vec4 p = vec4(vertices.at(i + j * 6), vertices.at(i + j * 6 + 1), vertices.at(i + j * 6 + 2), 1.0);
                // 模型 --> 世界 --> 相机空间 --> 齐次裁剪空间，xyz ~ [-w，w], w = Z(相机空间)
                Vector v = m.apply(Vector(p.x, p.y, p.z));
                p = vec4(v.x, v.y, v.z, v.w);
                triangle[j] = p;
                color[j] = vec3(vertices.at(i + j * 6 + 3) * 255, vertices.at(i + j * 6 + 4) * 255,
                                vertices.at(i + j * 6 + 5) * 255);
            }
            // CVV裁剪
            if (cvvCull(triangle)) {
                log("cvv cull");
                continue;
            }
            for (int j = 0; j < 3; j++) {
                vec4 p = triangle[j];
                // CVV裁剪
//                if (cvvCull(triangle)) {
//                    log("cvv cull");
//                    continue;
//                }
                // （透视除法） --> NDC空间
                p.x /= p.w;
                p.y /= p.w;
                p.z /= p.w;
                p.w = 1.0;
                // NDC空间 --> 窗口坐标（视口变换）
                p.x = (p.x + 1.0f) / 2.0f * TEX_WIDTH;
                p.y = (p.y + 1.0f) / 2.0f * TEX_HEIGHT;
                // 背面剔除
                // 光栅化
                // 光照，深度测试
                triangle[j] = p;
            }
            if (!Input::getInstance()->isKeyPressed(GLFW_KEY_F)) {
                dda_line(vec2(triangle[0]), vec2(triangle[1]));
                dda_line(vec2(triangle[1]), vec2(triangle[2]));
                dda_line(vec2(triangle[2]), vec2(triangle[0]));
            } else {
                fill(vec2(triangle[0]), vec2(triangle[1]), vec2(triangle[2]),
                     color[0], color[1], color[2]);
            }
        }
        SoftRender::draw(transform);
    }

    bool RastCube::cvvCull(vec4 triangle[3]) {
        return !inCvv(triangle[0]) && !inCvv(triangle[1]) && !inCvv(triangle[2]);
    }

    bool RastCube::inCvv(const vec4 &vector) {
        float w = fabs(vector.w);
        return fabs(vector.x) < w || fabs(vector.y) < w || fabs(vector.z) < w;
    }

    RastCube::~RastCube() {
    }

TEST_NODE_IMP_END