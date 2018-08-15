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

    LoadModel::~LoadModel() {
        delete modelObject;
    }
TEST_NODE_IMP_END