//
// Created by huangkun on 2018/8/14.
//

#include "LoadModel.h"

TEST_NODE_IMP_BEGIN

    LoadModel::LoadModel() {
        modelObject = new Model();
        modelObject->setCustomLoad(true);
        modelObject->load("../res/nanosuit/nanosuit.obj");
    }

    // pitch and yaw
    void LoadModel::draw(const mat4 &transform) {
        setDepthTest(true);
        setFaceCull(true);
        clearColor(50, 50, 50, 255);
        clearDepth();

        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0, -10, -10));
        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(Vector(cameraPos), Vector(target), Vector(cameraUp));
        Matrix m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);

        std::vector<Mesh *> &meshes = modelObject->getMeshes();
        for (unsigned int i = 0; i < meshes.size(); i++)
            drawMesh(*meshes[i], m);

        // draw light
//        drawPoint(lightPos, vec4(lightColor.x * 255, lightColor.y * 255, lightColor.z * 255, 255));

        SoftRender::draw(transform);
        setDepthTest(false);
        setFaceCull(false);
    }

    void LoadModel::setPixel(int x, int y, int z, float u, float v, vec3 varying[],
                             const std::vector<vec3> &uniforms) {
        Texture2D *diffuse = _bindTextures["texture_diffuse1"];
        if (diffuse) {
            const vec4 &textureColor = diffuse->sample(u, v);
            SoftRender::setPixel(x, y, z, textureColor);
        }
    }

    void LoadModel::bindTextures(const std::vector<Texture2D *> &textures) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++) {
            std::stringstream ss;
            std::string name = textures[i]->type;
            if (name == "texture_diffuse")
                ss << name << diffuseNr++; // transfer unsigned int to stream
            else if (name == "texture_specular")
                ss << name << specularNr++; // transfer unsigned int to stream
            auto it = _bindTextures.find(ss.str());
            if (it != _bindTextures.end())
                it->second = textures[i];
            else
                _bindTextures.insert(std::make_pair(ss.str(), textures[i]));
        }
    }

    void LoadModel::drawMesh(const Mesh &mesh, const Matrix &mvp) {
        const auto &vertices = mesh._vertices;
        const auto &indices = mesh._indices;
        bindTextures(mesh._texture2Ds);

        int num = indices.size() / 3;
        vec4 triangle[3];
        vec3 triangleWorld[3];
        vec3 normals[3];
        vec2 uv[3];
        for (int i = 0; i < num; i++) {
            for (int j = 0; j < 3; j++) {
                const Vertex &vertex = vertices[indices[i * 3 + j]];
                vec3 p = vertex.position;
                Vector v = mvp.applyPoint(p);
                triangle[j] = v.vec4();
                normals[j] = vertex.normal;
                uv[j] = vertex.texCoords;

                Vector v0 = modelMatrix.applyPoint(p);
                triangleWorld[j] = v0.vec3();
            }
            if (cvvCull(triangle)) {
                // 简单CVV裁剪，三角形3个点有一个不在cvv立方体内将被裁剪掉
                // log("cvv cull");
                continue;
            }

            if (faceCull(triangleWorld)) {
                // 背面剔除
//                log("backface cull");
                continue;
            }
            // 转换为屏幕坐标
            pointToScreen(triangle);

            // 光栅化
//            vec3 norms[] = {
//                    modelMatrix.applyVector(Vector(normals[0])).vec3(),
//                    modelMatrix.applyVector(Vector(normals[1])).vec3(),
//                    modelMatrix.applyVector(Vector(normals[2])).vec3(),
//            };
//
//            vec3 v1[] = {triangleWorld[0], norms[0]};
//            vec3 v2[] = {triangleWorld[1], norms[1]};
//            vec3 v3[] = {triangleWorld[2], norms[2]};
//            std::vector<VertexCoords> verts = {
//                    createVertexCoords(triangle[0], uv[0] * triangle[0].w, v1, 2),
//                    createVertexCoords(triangle[1], uv[1] * triangle[1].w, v2, 2),
//                    createVertexCoords(triangle[2], uv[2] * triangle[2].w, v3, 2),
//            };
            vec3 v1[] = {};
            vec3 v2[] = {};
            vec3 v3[] = {};
            std::vector<VertexCoords> verts = {
                    createVertexCoords(triangle[0], uv[0] * triangle[0].w, v1, 0),
                    createVertexCoords(triangle[1], uv[1] * triangle[1].w, v2, 0),
                    createVertexCoords(triangle[2], uv[2] * triangle[2].w, v3, 0),
            };
            fill(verts);
        }
    }

    LoadModel::~LoadModel() {
        delete modelObject;
    }
TEST_NODE_IMP_END