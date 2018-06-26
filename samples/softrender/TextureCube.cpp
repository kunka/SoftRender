//
// Created by huangkun on 2018/6/25.
//

#include "TextureCube.h"
#include "Input.h"
#include "stb_image.h"

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

        int nrComponents;
        textureData = stbi_load("../res/container.jpg", &textureWidth, &textureHeight, &nrComponents, 0);
        log("%d", nrComponents);
    }

    void TextureCube::draw(const mat4 &transform) {
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
        int column = 5;
        for (int i = 0; i < vertices.size(); i += column * 3) {
            vec4 triangle[3];
            vec2 uv[3];
            for (int j = 0; j < 3; j++) {
                vec4 p = vec4(vertices.at(i + j * column), vertices.at(i + j * column + 1),
                              vertices.at(i + j * column + 2), 1.0);
                // 模型 --> 世界 --> 相机空间 --> 齐次裁剪空间，xyz ~ [-w，w], w = Z(相机空间)
                Vector v = m.apply(Vector(p.x, p.y, p.z));
                p = vec4(v.x, v.y, v.z, v.w);
                triangle[j] = p;
                uv[j] = vec2(vertices.at(i + j * column + 3), vertices.at(i + j * column + 4));
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
            fill(vec2(triangle[0]), vec2(triangle[1]), vec2(triangle[2]), uv[0], uv[1], uv[2]);
        }
        SoftRender::draw(transform);
    }

    int clamp_(int x, int min, int max) {
        return (x < min) ? min : ((x > max) ? max : x);
    }

    vec3 TextureCube::sample(float u, float v) {
        int x = clamp_(textureWidth * u, 0, textureWidth - 1);
        int y = clamp_(textureHeight * v, 0, textureHeight - 1);
        unsigned char *pixel = textureData + (textureHeight * y + x) * 3;
        unsigned int r = (unsigned int) pixel[0];
        unsigned int g = (unsigned int) pixel[1];
        unsigned int b = pixel[2];
        return vec3(r, g, b);
    }

    struct VertexCoords {
        vec2 p;
        vec2 uv;
    };

    void TextureCube::fill(const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec2 &uv,
                           const vec2 &uv2, const vec2 &uv3) {
        std::vector<VertexCoords> ps = {{p1, uv},
                                        {p2, uv2},
                                        {p3, uv3}};
        std::sort(ps.begin(), ps.end(), [](const VertexCoords &a, const VertexCoords &b) { return a.p.y > b.p.y; });
        vec2 max = ps[0].p;
        vec2 mid = ps[1].p;
        vec2 min = ps[2].p;
        vec2 maxUV = ps[0].uv;
        vec2 midUV = ps[1].uv;
        vec2 minUV = ps[2].uv;

        if (mid.y == min.y || max.y == mid.y) {
            int dy = max.y - min.y;
            vec2 a, b;
            a.y = b.y = max.y;
            vec2 uv1, uv2;
            if (max.y == mid.y) {
                dda_line(mid, max, midUV, maxUV);
            } else {
                dda_line(min, mid, minUV, midUV);
            }
            for (int i = 1; i <= dy; i++) {
                float f = (float) i / dy;
                if (max.y == mid.y) {
                    a.x = mid.x + f * (min.x - mid.x);
                    uv1 = interp(midUV, minUV, f);
                } else {
                    a.x = max.x + f * (mid.x - max.x);
                    uv1 = interp(maxUV, midUV, f);
                }
                a.y -= 1;
                b.x = max.x + f * (min.x - max.x);
                uv2 = interp(maxUV, minUV, f);
                b.y -= 1;
                dda_line(a, b, uv1, uv2);
            }
        } else {
            float dy = max.y - min.y;
            float dx = max.x - min.x;
            vec2 midP;
            midP.y = mid.y;
            vec2 uv = interp(minUV, maxUV, (midP.y - min.y) / dy);
            if (dx == 0) {
                midP.x = max.x;
            } else {
                midP.x = max.x + (mid.y - max.y) * dx / dy;
            }
            fill(max, mid, midP, maxUV, midUV, uv);
            fill(mid, midP, min, midUV, uv, minUV);
        }
    }

    void TextureCube::dda_line(const vec2 &pa, const vec2 &pb, const vec2 &uv, const vec2 &uv2) {
        vec2 p1 = pa;
        vec2 p2 = pb;

        if (clipLine &&
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
        setPixel(x, y, sample(uv.x, uv.y));
        for (int k = 1; k <= steps; k++) {
            x += stepX;
            y += stepY;
            if (k == steps) {
                setPixel(x, y, sample(uv2.x, uv2.y));
            } else {
                vec2 uv0 = interp(uv, uv2, 1.0f * k / steps);
                setPixel(x, y, sample(uv0.x, uv0.y));
            }
        }
    }

    TextureCube::~TextureCube() {
        stbi_image_free(textureData);
    }

TEST_NODE_IMP_END