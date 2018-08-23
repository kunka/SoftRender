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
//                -4 / 1.732, 0, 2, 0, 0,
//                4 / 1.732, 0, 2, 1, 0,
//                0, 10, 2, 0.5, 1,
        };

        texture2D.load("../res/net.jpg");
    }

    void TextureCube::draw(const mat4 &transform) {
        setDepthTest(true);
        setFaceCull(true);
        clearColor(50, 50, 50, 255);
        clearDepth();

        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0.5, -0.5, 0));
        modelMatrix.rotate(Vector(0, 1, 0), 2 * 3.14f * sin(glfwGetTime() / 4));
        modelMatrix.rotate(Vector(1, 0, 0), radians(30.0f));
        modelMatrix.scale(Vector(1.5f, 2, 0.5));
        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(Vector(cameraPos.x, cameraPos.y, cameraPos.z), Vector(target.x, target.y, target.z),
                                    Vector(cameraUp.x, cameraUp.y, cameraUp.z));
        projectMatrix = Matrix::perspective(radians(60.0f), (float) TEX_WIDTH / TEX_HEIGHT, 0.1, 100.0f);
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

    void TextureCube::fill(std::vector<VertexCoords> &verts, const std::vector<vec3> &uniforms) {
        std::sort(verts.begin(), verts.end(),
                  [](const VertexCoords &a, const VertexCoords &b) { return a.p.y > b.p.y; });
        const auto &max = verts[0].p;
        const auto &mid = verts[1].p;
        const auto &min = verts[2].p;
        const auto &maxUV = verts[0].uv;
        const auto &midUV = verts[1].uv;
        const auto &minUV = verts[2].uv;
        vec3 *maxVarying = verts[0].varying;
        vec3 *midVarying = verts[1].varying;
        vec3 *minVarying = verts[2].varying;
        int varyingCount = verts[0].varyingCount;

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
            float dvdy = abs(dy) < FLT_EPSILON ? 0 : (maxUV.y - minUV.y) / dy;

            vec3 varyingA[varyingCount];
            vec3 varyingB[varyingCount];
            float fdy = 1.0f / (max.y - min.y);
            for (int i = 0; i <= dy; i++) {
                float f = (float) i * fdy;
                if (max.y == mid.y) {
                    a.x = interp(mid.x, min.x, f);
                    a.z = interp(mid.z, min.z, f);
                    a.w = interp(mid.w, min.w, f);
                    uv1 = interp(midUV, minUV, f);
                    verts[0].interp(varyingA, midVarying, minVarying, f);
                } else {
                    a.x = interp(max.x, mid.x, f);
                    a.z = interp(max.z, mid.z, f);
                    a.w = interp(max.w, mid.w, f);
                    uv1 = interp(maxUV, midUV, f);
                    verts[0].interp(varyingA, maxVarying, midVarying, f);
                }
                b.x = interp(max.x, min.x, f);
                b.z = interp(max.z, min.z, f);
                b.w = interp(max.w, min.w, f);
                uv2 = interp(maxUV, minUV, f);
                verts[0].interp(varyingB, maxVarying, minVarying, f);

                if (a == b) {
                    // point
                    verts[0].divideW(varyingA, a.w);
                    setPixel(a.x, a.y, a.z, uv1.x / a.w, uv1.y / a.w, varyingA, uniforms, 0, dvdy / a.w);
                } else {
                    dda_line(createVertexCoords(a, uv1, varyingA, varyingCount),
                             createVertexCoords(b, uv2, varyingB, varyingCount), uniforms, dvdy);
                }
                a.y -= 1;
                b.y -= 1;
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

            vec3 interpVarying[varyingCount];
            verts[0].interp(interpVarying, minVarying, maxVarying, f);
            std::vector<VertexCoords> verts1 = {
                    createVertexCoords(max, maxUV, maxVarying, varyingCount),
                    createVertexCoords(mid, midUV, midVarying, varyingCount),
                    createVertexCoords(midP, uv, interpVarying, varyingCount)};
            fill(verts1, uniforms);
            std::vector<VertexCoords> verts2 = {
                    createVertexCoords(mid, midUV, midVarying, varyingCount),
                    createVertexCoords(midP, uv, interpVarying, varyingCount),
                    createVertexCoords(min, minUV, minVarying, varyingCount)};
            fill(verts2, uniforms);
        }
    }

    void TextureCube::dda_line(const VertexCoords &vert1, const VertexCoords &vert2,
                               const std::vector<vec3> &uniforms, float dvdy) {
        const vec4 &pa = vert1.p;
        const vec4 &pb = vert2.p;
        const vec2 &uv1 = vert1.uv;
        const vec2 &uv2 = vert2.uv;
        vec3 *varyingA = (vec3 *) vert1.varying;
        vec3 *varyingB = (vec3 *) vert2.varying;
        int varyingCount = vert1.varyingCount;
        vec4 p1 = pa;
        vec4 p2 = pb;
        vec4 p11 = pa;
        vec4 p22 = pb;
        if (clip_3D_line(p11, p22)) {
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
        vec3 varying1[varyingCount];
        vert1.interp(varying1, varyingA, varyingB, f1);
        vec3 varying2[varyingCount];
        vert1.interp(varying2, varyingA, varyingB, f2);

        // DDA
        float dy = p2.y - p1.y;
        float dx = p2.x - p1.x;
        float stepX, stepY;
        int steps;
        float fsteps;
        if (abs(dy) > abs(dx)) {
            steps = abs((int) dy);
            fsteps = 1.0f / abs(dy);
            stepY = dy > 0 ? 1 : -1;
            stepX = dx > 0 ? fabs(dx / dy) : -fabs(dx / dy);
        } else {
            steps = abs((int) dx);
            fsteps = 1.0f / abs(dx);
            stepX = dx > 0 ? 1 : -1;
            if (dx == 0)
                stepY = 0;
            else
                stepY = dy > 0 ? fabs(dy / dx) : -fabs(dy / dx);
        }
        float dudx = abs(dx) < FLT_EPSILON ? 0 : (uv12.x - uv11.x) / dx;
        float x = p1.x, y = p1.y;
        vec3 interpVarying[varyingCount];
        for (int k = 0; k <= steps; k++) {
            float f = k * fsteps;
            float z = interp(p1.z, p2.z, f);
            float w = interp(p1.w, p2.w, f);
            vec2 uv0 = interp(uv11, uv12, f);
            vert1.interp(interpVarying, varying1, varying2, f);
            vert1.divideW(interpVarying, w);
            setPixel(x, y, z, uv0.x / w, uv0.y / w, interpVarying, uniforms, dudx / w, dvdy / w);
            x += stepX;
            y += stepY;
        }

        // Bresenham
//        float x0 = p1.x, x1 = p2.x, y0 = p1.y, y1 = p2.y;
//        bool flip = abs(y1 - y0) > abs(x1 - x0);
//        if (flip) {
//            swap(x0, y0);
//            swap(x1, y1);
//        }
//        if (x0 > x1) {
//            swap(x0, x1);
//            swap(y0, y1);
//        }
//        int dx = x1 - x0;
//        int dy = abs(y1 - y0);
//        int error = dx / 2;
//        int ystep = y0 < y1 ? 1 : -1;
//        int y = y0;
//        for (int x = x0; x <= x1; x++) {
//            if (flip) {
//                float f = dx == 0 ? (y - y0) / (float) dy : (x - x0) / (float) dx;
//                float z = interp(p1.z, p2.z, f);
//                float w = interp(p1.w, p2.w, f);
//                vec2 uv0 = interp(uv11, uv12, f);
//                std::vector<vec4> interpVarying;
//                for (int m = 0; m < varyingA.size(); m++) {
//                    interpVarying.push_back(interp(varying1[m], varying2[m], f));
//                }
//                setPixel(y, x, p1.z, uv11.x / p1.w, uv11.y / p1.w, varying1, uniforms);
//            } else {
//                float f = dx == 0 ? (y - y0) / (float) dy : (x - x0) / (float) dx;
//                float z = interp(p1.z, p2.z, f);
//                float w = interp(p1.w, p2.w, f);
//                vec2 uv0 = interp(uv11, uv12, f);
//                std::vector<vec4> interpVarying;
//                for (int m = 0; m < varyingA.size(); m++) {
//                    interpVarying.push_back(interp(varying1[m], varying2[m], f));
//                }
//                setPixel(x, y, p1.z, uv11.x / p1.w, uv11.y / p1.w, varying1, uniforms);
//            }
//            error = error - dy;
//            if (error < 0) {
//                y += ystep;
//                error += dx;
//            }
//        }
    }

    void TextureCube::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                               const std::vector<vec3> &uniforms) {
        const vec4 &color = texture2D.sample(u, v);
        SoftRender::setPixel(x, y, z, color);
    }

    void TextureCube::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                               const std::vector<vec3> &uniforms, float dudx, float dudy) {
        setPixel(x, y, z, u, v, varying, uniforms);
    }

    TextureCube::~TextureCube() {
    }

    void TextureCube::drawMesh(const Mesh &mesh, const Matrix &mvp, int varyingCount) {
        const auto &vertices = mesh._vertices;
        const auto &indices = mesh._indices;
        bindTextures(mesh._texture2Ds);
        bool useIndices = indices.size() > 0;

        int num = useIndices ? indices.size() / 3 : vertices.size() / 3;
        vec4 triangle[3];
        vec3 triangleWorld[3];
        vec3 normals[3];
        vec2 uv[3];
        for (int i = 0; i < num; i++) {
            for (int j = 0; j < 3; j++) {
                const Vertex &vertex = useIndices ? vertices[indices[i * 3 + j]] : vertices[i * 3 + j];
                vec3 p = vertex.position;
                Vector v = mvp.applyPoint(p);
                triangle[j] = v.vec4();
                normals[j] = vertex.normal;
                uv[j] = vertex.texCoords;

                Vector v0 = modelMatrix.applyPoint(p);
                triangleWorld[j] = v0.vec3();
            }
            if (cvvCull(triangle)) {
                // log("cvv cull");
                continue;
            }

            if (faceCull(triangleWorld)) {
                // 背面剔除
                //  log("backface cull");
                continue;
            }
            // 转换为屏幕坐标
            pointToScreen(triangle);
//            vec3 norms[] = {
//                    modelMatrix.applyVector(normals[0]).vec3(),
//                    modelMatrix.applyVector(normals[1]).vec3(),
//                    modelMatrix.applyVector(normals[2]).vec3(),
//            };
            mat4 matrix = mat4(modelMatrix.m[0][0], modelMatrix.m[0][1], modelMatrix.m[0][2], modelMatrix.m[0][3],
                               modelMatrix.m[1][0], modelMatrix.m[1][1], modelMatrix.m[1][2], modelMatrix.m[1][3],
                               modelMatrix.m[2][0], modelMatrix.m[2][1], modelMatrix.m[2][2], modelMatrix.m[2][3],
                               modelMatrix.m[3][0], modelMatrix.m[3][1], modelMatrix.m[3][2], modelMatrix.m[3][3]);
            matrix = glm::transpose(glm::inverse(matrix));
            vec3 norms[] = {
                    matrix * vec4(normals[0].x, normals[0].y, normals[0].z, 0),
                    matrix * vec4(normals[1].x, normals[1].y, normals[1].z, 0),
                    matrix * vec4(normals[2].x, normals[2].y, normals[2].z, 0),
            };

            // 光栅化
            vec3 v1[3], v2[3], v3[3];
            if (varyingCount == 1) {
                v1[0] = triangleWorld[0] * triangle[0].w;
                v2[0] = triangleWorld[1] * triangle[1].w;
                v3[0] = triangleWorld[2] * triangle[2].w;
            } else if (varyingCount == 2) {
                v1[0] = triangleWorld[0] * triangle[0].w;
                v2[0] = triangleWorld[1] * triangle[1].w;
                v3[0] = triangleWorld[2] * triangle[2].w;
                v1[1] = norms[0] * triangle[0].w;
                v2[1] = norms[1] * triangle[1].w;
                v3[1] = norms[2] * triangle[2].w;
            }
            std::vector<VertexCoords> verts = {
                    createVertexCoords(triangle[0], uv[0] * triangle[0].w, v1, varyingCount),
                    createVertexCoords(triangle[1], uv[1] * triangle[1].w, v2, varyingCount),
                    createVertexCoords(triangle[2], uv[2] * triangle[2].w, v3, varyingCount),
            };
            fill(verts);
        }
    }

    void TextureCube::bindTextures(const std::vector<Texture2D *> &textures) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int defaultNr = 0;
        for (unsigned int i = 0; i < textures.size(); i++) {
            std::stringstream ss;
            std::string name = textures[i]->type;
            if (name == "texture_diffuse")
                ss << name << diffuseNr++; // transfer unsigned int to stream
            else if (name == "texture_specular")
                ss << name << specularNr++; // transfer unsigned int to stream
            else
                ss << "texture" << defaultNr++;
            auto it = _bindTextures.find(ss.str());
            if (it != _bindTextures.end())
                it->second = textures[i];
            else
                _bindTextures.insert(std::make_pair(ss.str(), textures[i]));
        }
    }
TEST_NODE_IMP_END