//
// Created by huangkun on 2018/8/15.
//

#include "VisualizeDepth.h"

TEST_NODE_IMP_BEGIN

    VisualizeDepth::VisualizeDepth() {
    }

    void VisualizeDepth::draw(const mat4 &transform) {
        setDepthTest(true);
        setFaceCull(true);
        clearColor(50, 50, 50, 255);
        clearDepth();

        modelMatrix.setIdentity();
        modelMatrix.rotate(Vector(1, 0, 0), 2 * 3.14f * sin(glfwGetTime() / 4));
        modelMatrix.rotate(Vector(1, 0, 0), radians(90.0f));
        modelMatrix.scale(Vector(1.5f, 2, 0.5));
        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(Vector(cameraPos.x, cameraPos.y, cameraPos.z), Vector(target.x, target.y, target.z),
                                    Vector(cameraUp.x, cameraUp.y, cameraUp.z));
        Matrix m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        int column = 5;
        vec4 triangle[3];
        vec3 triangleWorld[3];
        vec2 uv[3];
        for (int i = 0; i < vertices.size(); i += column * 3) {
            for (int j = 0; j < 3; j++) {
                int row = i + j * column;
                vec4 p = vec4(vertices.at(row), vertices.at(row + 1),
                              vertices.at(row + 2), 1.0);
                // 模型 --> 世界 --> 相机空间 --> 齐次裁剪空间，xyz ~ [-w，w], w = Z(相机空间)
                Vector v = m.applyPoint(Vector(p.x, p.y, p.z));
                triangle[j] = vec4(v.x, v.y, v.z, v.w);
                uv[j] = vec2(vertices.at(row + 3), vertices.at(row + 4));

                Vector v0 = modelMatrix.applyPoint(Vector(p.x, p.y, p.z));
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
            vec3 v1[] = {triangle[0]};
            vec3 v2[] = {triangle[1]};
            vec3 v3[] = {triangle[2]};
            std::vector<VertexCoords> verts = {
                    createVertexCoords(triangle[0], uv[0] * triangle[0].w, v1, 1),
                    createVertexCoords(triangle[1], uv[1] * triangle[1].w, v2, 1),
                    createVertexCoords(triangle[2], uv[2] * triangle[2].w, v3, 1)
            };
            fill(verts);
        }
        SoftRender::draw(transform);
        setDepthTest(false);
        setFaceCull(false);
    }

    float LinearizeDepth(float depth) {
        float near = 0.1;
        float far = 5.0; // use a closer dis, the box isn't too far away.
        float z = depth * 2.0f - 1.0f; // back to NDC
        return (2.0f * near * far) / (far + near - z * (far - near));
    }

    void VisualizeDepth::setPixel(int x, int y, int z, float u, float v, vec3 varying[],
                                  const std::vector<vec3> &uniforms) {
        vec3 fragPos = varying[0];
        float depth = LinearizeDepth(fragPos.z) / 5.0f;
        vec4 color = vec4(depth, depth, depth, 1.0) * 255.0f;
        SoftRender::setPixel(x, y, z, color);
    }

    VisualizeDepth::~VisualizeDepth() {
    }

TEST_NODE_IMP_END