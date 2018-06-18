//
// Created by huangkun on 2018/6/17.
//

#include "Transform2D.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    Transform2D::Transform2D() {
        vertices = {
                // postions        // texture coords
                // Back face
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
                // Front face
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
                // Left face
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-left
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
                // Right face
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
                // Bottom face
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
                // Top face
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f  // bottom-left
        };
    }

    void Transform2D::draw(const mat4 &transform) {
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
        for (int i = 0; i < vertices.size(); i += 5 * 3) {
            Vector triangle[3];
            for (int j = 0; j < 3; j++) {
                Vector p = Vector(vertices.at(i + j * 5), vertices.at(i + j * 5 + 1), vertices.at(i + j * 5 + 2));
                p = m.apply(p);
                p.x /= -p.w;
                p.y /= -p.w;
                p.z /= -p.w;
                p.x = (p.x + 1.0f) / 2.0f * TEX_WIDTH;
                p.y = (p.y + 1.0f) / 2.0f * TEX_HEIGHT;
                triangle[j] = p;
            }
//            dda_line(vec2(triangle[0].x, triangle[0].y), vec2(triangle[1].x, triangle[1].y));
//            dda_line(vec2(triangle[1].x, triangle[1].y), vec2(triangle[2].x, triangle[2].y));
//            dda_line(vec2(triangle[2].x, triangle[2].y), vec2(triangle[0].x, triangle[0].y));

            fill(vec2(triangle[0].x, triangle[0].y), vec2(triangle[1].x, triangle[1].y),
                 vec2(triangle[2].x, triangle[2].y), vec3(100 + 15 * i, 100 + 15 * i, 100 + 15 * i));
//            fill(vec2(triangle[0].x, triangle[0].y), vec2(triangle[1].x, triangle[1].y),
//                 vec2(triangle[2].x, triangle[2].y));
        }
        SoftRender::draw(transform);
    }

    Transform2D::~Transform2D() {
    }

TEST_NODE_IMP_END