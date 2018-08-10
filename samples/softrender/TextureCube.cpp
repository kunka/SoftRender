//
// Created by huangkun on 2018/6/25.
//

#include "TextureCube.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    TextureCube::TextureCube() {
        vertices = {
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

        texture2D.load("../res/net.jpg");
    }

    void TextureCube::draw(const mat4 &transform) {
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
            std::vector<VertexCoords> verts = {
                    {triangle[0], uv[0] * triangle[0].w},
                    {triangle[1], uv[1] * triangle[1].w},
                    {triangle[2], uv[2] * triangle[2].w},};
            fill(verts);
        }
        SoftRender::draw(transform);
        setDepthTest(false);
        setFaceCull(false);
    }

    void TextureCube::fill(std::vector<VertexCoords> &verts, const std::vector<vec4> &uniforms) {
        std::sort(verts.begin(), verts.end(),
                  [](const VertexCoords &a, const VertexCoords &b) { return a.p.y > b.p.y; });
        const auto &max = verts[0].p;
        const auto &mid = verts[1].p;
        const auto &min = verts[2].p;
        const auto &maxUV = verts[0].uv;
        const auto &midUV = verts[1].uv;
        const auto &minUV = verts[2].uv;
        const auto &maxVarying = verts[0].varying;
        const auto &midVarying = verts[1].varying;
        const auto &minVarying = verts[2].varying;

        /*
         * mid ---- max
         *     \  /
         *      \/
         *      min
         *
         *      max
         *      /\
         *     /  \
         * min ---- mid
         */
        if (mid.y == min.y || max.y == mid.y) {
            int dy = max.y - min.y;
            vec4 a, b;
            a.y = b.y = max.y;
            vec2 uv1, uv2;
            if (max.y == mid.y) {
//                dda_line(mid, max, midUV, maxUV);
                dda_line({mid, midUV, midVarying}, {max, maxUV, maxVarying}, uniforms);
            } else {
//                dda_line(min, mid, minUV, midUV);
                dda_line({min, minUV, minVarying}, {mid, midUV, midVarying}, uniforms);
            }
            for (int i = 1; i <= dy; i++) {
                float f = (float) i / dy;
                std::vector<vec4> varyingA;
                std::vector<vec4> varyingB;
                if (max.y == mid.y) {
                    a.x = interp(mid.x, min.x, f);
                    a.z = interp(mid.z, min.z, f);
                    a.w = interp(mid.w, min.w, f);
                    uv1 = interp(midUV, minUV, f);
                    for (int m = 0; m < maxVarying.size(); m++) {
                        varyingA.push_back(interp(midVarying[m], minVarying[m], f));
                    }
                } else {
                    a.x = interp(max.x, mid.x, f);
                    a.z = interp(max.z, mid.z, f);
                    a.w = interp(max.w, mid.w, f);
                    uv1 = interp(maxUV, midUV, f);
                    for (int m = 0; m < maxVarying.size(); m++) {
                        varyingA.push_back(interp(maxVarying[m], midVarying[m], f));
                    }
                }
                a.y -= 1;
                b.x = interp(max.x, min.x, f);
                b.z = interp(max.z, min.z, f);
                b.w = interp(max.w, min.w, f);
                uv2 = interp(maxUV, minUV, f);
                b.y -= 1;
                for (int m = 0; m < maxVarying.size(); m++) {
                    varyingB.push_back(interp(maxVarying[m], minVarying[m], f));
                }

                dda_line({a, uv1, varyingA}, {b, uv2, varyingB}, uniforms);
            }
        } else {
            /*
             *       max
             *       /|
             *      / |
             * mid /  |
             *     \  |
             *      \ |
             *       \|
             *       min
             */
            float dy = max.y - min.y;
            vec4 midP;
            midP.y = mid.y;
            float f = (midP.y - min.y) / dy;
            midP.w = interp(min.w, max.w, f);
            midP.z = interp(min.z, max.z, f);
            midP.x = interp(min.x, max.x, f);
            vec2 uv = interp(minUV, maxUV, f);

            std::vector<vec4> interpVarying;
            for (int m = 0; m < maxVarying.size(); m++) {
                interpVarying.push_back(interp(minVarying[m], maxVarying[m], f));
            }
            std::vector<VertexCoords> verts1 = {
                    {max,  maxUV, maxVarying},
                    {mid,  midUV, midVarying},
                    {midP, uv,    interpVarying}};
            fill(verts1, uniforms);
            std::vector<VertexCoords> verts2 = {
                    {mid,  midUV, midVarying},
                    {midP, uv,    interpVarying},
                    {min,  minUV, minVarying}};
            fill(verts2, uniforms);
        }
    }

    void TextureCube::dda_line(const VertexCoords &vert1, const VertexCoords &vert2,
                               const std::vector<vec4> &uniforms) {
        const vec4 &pa = vert1.p;
        const vec4 &pb = vert2.p;
        const vec2 &uv1 = vert1.uv;
        const vec2 &uv2 = vert2.uv;
        const auto &varyingA = vert1.varying;
        const auto &varyingB = vert2.varying;
        vec4 p1 = pa;
        vec4 p2 = pb;
        vec2 p11 = pa;
        vec2 p22 = pb;
        auto varying1 = varyingA;
        auto varying2 = varyingB;
        if (isClipRect &&
            !clip_a_line(p11, p22, clipRect.getMinX(), clipRect.getMaxX(), clipRect.getMinY(),
                         clipRect.getMaxY())) {
            return;
        }
        p1.x = p11.x;
        p1.y = p11.y;
        p2.x = p22.x;
        p2.y = p22.y;
        vec2 uv11 = uv1;
        vec2 uv12 = uv2;
        float f1 = 0, f2 = 0;
        if (pb.x != pa.x) {
            f1 = (p11.x - pa.x) / (pb.x - pa.x);
            f2 = (p22.x - pa.x) / (pb.x - pa.x);
        }
        if (pb.y != pa.y) {
            f1 = (p11.y - pa.y) / (pb.y - pa.y);
            f2 = (p22.y - pa.y) / (pb.y - pa.y);
        }
        p1.w = interp(pa.w, pb.w, f1);
        p2.w = interp(pa.w, pb.w, f2);
        p1.z = interp(pa.z, pb.z, f1);
        p2.z = interp(pa.z, pb.z, f2);
        uv11 = interp(uv1, uv2, f1);
        uv12 = interp(uv1, uv2, f2);
        for (int m = 0; m < varyingA.size(); m++) {
            varying1[m] = interp(varyingA[m], varyingB[m], f1);
        }
        for (int m = 0; m < varyingA.size(); m++) {
            varying2[m] = interp(varyingA[m], varyingB[m], f2);
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
        setPixel(x, y, p1.z, uv11.x / p1.w, uv11.y / p1.w, varying1, uniforms);
        for (int k = 1; k <= steps; k++) {
            x += stepX;
            y += stepY;
            float f = 1.0f * k / steps;
            float z = interp(p1.z, p2.z, f);
            float w = interp(p1.w, p2.w, f);
            vec2 uv0 = interp(uv11, uv12, f);
            std::vector<vec4> interpVarying;
            for (int m = 0; m < varyingA.size(); m++) {
                interpVarying.push_back(interp(varying1[m], varying2[m], f));
            }
            setPixel(x, y, z, uv0.x / w, uv0.y / w, interpVarying, uniforms);
        }
        setPixel(p2.x, p2.y, p2.z, uv12.x / p2.w, uv12.y / p2.w, varying2, uniforms);
    }

    void TextureCube::setPixel(int x, int y, int z, float u, float v, const std::vector<vec4> &varying,
                               const std::vector<vec4> &uniforms) {
        const vec4 &color = texture2D.sample(u, v);
        SoftRender::setPixel(x, y, z, color);
    }

    TextureCube::~TextureCube() {
    }

TEST_NODE_IMP_END