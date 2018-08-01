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
//        textureData = stbi_load("../res/net.jpg", &textureWidth, &textureHeight, &nrComponents, 0);
        stbi_set_flip_vertically_on_load(true); // flipY
        textureData = stbi_load("../res/smile.jpg", &textureWidth, &textureHeight, &nrComponents, 0);
        log("width=%d,height=%d,channel=%d", textureWidth, textureHeight, nrComponents);
        depthBuff = new float[textureWidth * textureHeight];

//        clipRect.setRect(TEX_WIDTH / 4, TEX_WIDTH / 4, TEX_HEIGHT / 2, TEX_HEIGHT / 2);
        clipRect.setRect(0, 0, TEX_WIDTH, TEX_HEIGHT);
        clipLine = true;
    }

    void TextureCube::draw(const mat4 &transform) {
        memset(texData, 0, TEX_WIDTH * TEX_HEIGHT * 4);
        for (int i = 0; i < textureWidth * textureHeight; i++)
            depthBuff[i] = INT_MAX;

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
            vec3 triangleOrigin[3];
            vec2 uv[3];
            for (int j = 0; j < 3; j++) {
                vec4 p = vec4(vertices.at(i + j * column), vertices.at(i + j * column + 1),
                              vertices.at(i + j * column + 2), 1.0);
                // 模型 --> 世界 --> 相机空间 --> 齐次裁剪空间，xyz ~ [-w，w], w = Z(相机空间)
                Vector v = m.apply(Vector(p.x, p.y, p.z));
                triangle[j] = vec4(v.x, v.y, v.z, v.w);
                uv[j] = vec2(vertices.at(i + j * column + 3), vertices.at(i + j * column + 4));

                Vector v0 = model.apply(Vector(p.x, p.y, p.z));
                triangleOrigin[j] = vec3(v0.x, v0.y, v0.z);
            }
            // CVV裁剪
            if (cvvCull(triangle)) {
                log("cvv cull");
                continue;
            }
            // 背面剔除
            vec3 cameraP = vec3(cameraPos.x, cameraPos.y, cameraPos.z);
            vec3 v1 = triangleOrigin[1] - triangleOrigin[0];
            vec3 v2 = triangleOrigin[2] - triangleOrigin[0];
            vec3 normal = glm::cross(v1, v2);
            vec3 v0 = vec3(triangleOrigin[0]) - cameraP;
            if (glm::dot(v0, normal) >= 0) {
//                log("backface cull");
//                continue;
            }

            for (int j = 0; j < 3; j++) {
                vec4 p = triangle[j];
                // CVV裁剪
//                if (cvvCull(triangle)) {
//                    log("cvv cull");
//                    continue;
//                }
                // （透视除法） --> NDC空间
                p.x /= p.w; // [-1,1]
                p.y /= p.w; // [-1,1]
                p.z /= p.w; // [-1,1]
//                p.w = 1.0;
                // NDC空间 --> 窗口坐标（视口变换）
                p.x = (p.x + 1.0f) / 2.0f * TEX_WIDTH;
                p.y = (p.y + 1.0f) / 2.0f * TEX_HEIGHT;
                // 背面剔除
                // 光栅化
                // 光照，深度测试
                triangle[j] = p;
            }
            fill(triangle[0], triangle[1], triangle[2], uv[0], uv[1], uv[2]);
//            fill(triangle[0], triangle[1], triangle[2], uv[0] / triangle[0].w, uv[1] / triangle[1].w,
//                 uv[2] / triangle[2].w);
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
        vec4 p;
        vec2 uv;
    };

    void TextureCube::fill(const vec4 &p1, const vec4 &p2, const vec4 &p3, const vec2 &uv,
                           const vec2 &uv2, const vec2 &uv3) {
        std::vector<VertexCoords> ps = {{p1, uv},
                                        {p2, uv2},
                                        {p3, uv3}};
        std::sort(ps.begin(), ps.end(), [](const VertexCoords &a, const VertexCoords &b) { return a.p.y > b.p.y; });
        vec4 max = ps[0].p;
        vec4 mid = ps[1].p;
        vec4 min = ps[2].p;
        vec2 maxUV = ps[0].uv;
        vec2 midUV = ps[1].uv;
        vec2 minUV = ps[2].uv;

        if (mid.y == min.y || max.y == mid.y) {
            int dy = max.y - min.y;
            vec4 a, b;
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
            float dz = max.z - min.z;
            vec4 midP;
            midP.y = mid.y;
            vec2 uv = interp(minUV, maxUV, (midP.y - min.y) / dy);
            midP.z = interp(min.z, max.z, (midP.y - min.y) / dy);
            if (dx == 0) {
                midP.x = max.x;
            } else {
                midP.x = max.x + (mid.y - max.y) * dx / dy;
            }
            fill(max, mid, midP, maxUV, midUV, uv);
            fill(mid, midP, min, midUV, uv, minUV);
        }
    }

    void TextureCube::dda_line(const vec4 &pa, const vec4 &pb, const vec2 &uv1, const vec2 &uv2) {
        vec4 p1 = pa;
        vec4 p2 = pb;
        vec2 p11 = pa;
        vec2 p22 = pb;
        if (clipLine &&
            !clip_a_line(p11, p22, clipRect.getMinX(), clipRect.getMaxX(), clipRect.getMinY(), clipRect.getMaxY())) {
            return;
        }
        p1.x = p11.x;
        p1.y = p11.y;
        p2.x = p22.x;
        p2.y = p22.y;
        vec2 uv11 = uv1;
        vec2 uv12 = uv2;
        if (pb.x != pa.x) {
            uv11.x = interp(uv1.x, uv2.x, (p11.x - pa.x) / (pb.x - pa.x));
            uv12.x = interp(uv1.x, uv2.x, (p22.x - pa.x) / (pb.x - pa.x));
        }
        if (pb.y != pa.y) {
            uv11.y = interp(uv1.y, uv2.y, (p11.y - pa.y) / (pb.y - pa.y));
            uv12.y = interp(uv1.y, uv2.y, (p22.y - pa.y) / (pb.y - pa.y));
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
//        setPixel(x, y, 1.0f / p1.z, sample(uv.x, uv.y));
        setPixel(x, y, p1.z, sample(uv11.x, uv11.y));
        for (int k = 1; k <= steps; k++) {
            x += stepX;
            y += stepY;
            float f = 1.0f * k / steps;
            float z = interp(p1.z, p2.z, f);
            vec2 uv0 = interp(uv11, uv12, f);
//            setPixel(x, y, 1.0f / z, sample(uv0.x, uv0.y));
            setPixel(x, y, z, sample(uv0.x, uv0.y));
        }
    }

    void TextureCube::setPixel(int x, int y, float depth, const vec4 &color) {
        if (x >= 0 && y >= 0 && x < TEX_WIDTH && y < TEX_HEIGHT) {
            int index = y * textureWidth + x;
            if (depth < depthBuff[index]) {
                depthBuff[index] = depth;
                texData[y][x][0] = (GLubyte) color.r;
                texData[y][x][1] = (GLubyte) color.g;
                texData[y][x][2] = (GLubyte) color.b;
                texData[y][x][3] = (GLubyte) color.a;
//            log("%f",depth);
            }
        }
    }

    void TextureCube::setPixel(int x, int y, float depth, const vec3 &color) {
        setPixel(x, y, depth, vec4(color, 255));
    }

    TextureCube::~TextureCube() {
        stbi_image_free(textureData);
        delete[] depthBuff;
    }

TEST_NODE_IMP_END