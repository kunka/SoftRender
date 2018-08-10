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
        clearColor(0, 0, 0, 0);

        Matrix model;
        model.rotate(Vector(0, 1, 0), radians(10.0f));
//        model.rotate(Vector(0, 1, 0), 2 * 3.14f * sin(glfwGetTime() / 4));
        vec3 target = cameraPos + cameraDir;
        viewMatrix.lookAt(Vector(cameraPos.x, cameraPos.y, cameraPos.z), Vector(target.x, target.y, target.z),
                          Vector(cameraUp.x, cameraUp.y, cameraUp.z));
        Matrix m = model;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        vec2 last;
        int column = 6;
        for (int i = 0; i < vertices.size(); i += column * 3) {
            vec4 triangle[3];
            vec3 color[3];
            for (int j = 0; j < 3; j++) {
                int row = i + j * column;
                vec4 p = vec4(vertices.at(row), vertices.at(row + 1),
                              vertices.at(row + 2), 1.0);
                // 模型 --> 世界 --> 相机空间 --> 齐次裁剪空间，xyz ~ [-w，w], w = Z(相机空间)
                Vector v = m.apply(Vector(p.x, p.y, p.z));
                p = vec4(v.x, v.y, v.z, v.w);
                triangle[j] = p;
                color[j] = vec3(vertices.at(row + 3) * 255, vertices.at(row + 4) * 255,
                                vertices.at(row + 5) * 255);
            }
            // CVV裁剪
            if (cvvCull(triangle)) {
                log("cvv cull");
                continue;
            }
            pointToScreen(triangle);

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

    RastCube::~RastCube() {
    }

TEST_NODE_IMP_END