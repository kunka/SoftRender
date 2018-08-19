//
// Created by huangkun on 2018/8/15.
//

#include "Blend.h"
#include <map>

TEST_NODE_IMP_BEGIN

    Blend::Blend() {
    }

    bool Blend::init() {
        SoftRender::init();
        texture2DBox.load("../res/container.jpg");
        texture2DPlane.load("../res/metal.png");
        texture2DWindow.load("../res/blending_transparent_window.png");

        vertices = {
                // postions        // texture coords
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
        };
        verticesPlane = {
                // postions       // texture coords
                0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
                -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // bottom top
        };
        indicesPlane = { // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
        };
        Mesh *mesh = new Mesh(createVertexs(verticesPlane, 3, 0, 2), indicesPlane);
        planeMeshes.push_back(mesh);

        mesh = new Mesh(createVertexs(vertices, 3, 0, 2));
        boxMeshes.push_back(mesh);

        cameraPos = vec3(0.0f, 1.5f, 3.0f);
        cameraDir = vec3(0.0f, 0.0f, 0.0f) - cameraPos;
        return true;
    }

    void Blend::draw(const mat4 &transform) {
        setDepthTest(true);
        clearColor(50, 50, 50, 255);
        clearDepth();
        setBlendEnabled(true);
        setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        setFaceCull(false);
        // draw plane
        bindTextures({&texture2DPlane});
        modelMatrix.setIdentity();
        modelMatrix.rotate(Vector(1, 0, 0), radians(-90.0f));
        modelMatrix.scale(Vector(8, 6, 1));
        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(Vector(cameraPos.x, cameraPos.y, cameraPos.z), Vector(target.x, target.y, target.z),
                                    Vector(cameraUp.x, cameraUp.y, cameraUp.z));
        Matrix m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m);

        // draw box
        bindTextures({&texture2DBox});
        modelMatrix.setIdentity();
        modelMatrix.translate(vec3(-1, 0.5, -1.0));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < boxMeshes.size(); i++)
            drawMesh(*boxMeshes[i], m);

        modelMatrix.setIdentity();
        modelMatrix.translate(vec3(1, 0.5, -1.0));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < boxMeshes.size(); i++)
            drawMesh(*boxMeshes[i], m);

        // draw windows
        std::vector<glm::vec3> vegetation;
        vegetation.push_back(glm::vec3(-1.5f, 0.5f, -0.48f));
        vegetation.push_back(glm::vec3(1.5f, 0.5f, 0.51f));
        vegetation.push_back(glm::vec3(0.0f, 0.5f, 0.7f));
        vegetation.push_back(glm::vec3(-0.3f, 0.5f, -2.3f));
        vegetation.push_back(glm::vec3(0.5f, 0.5f, -0.6f));

//        1. Draw all opaque objects first.
//        2. Sort all the transparent objects.
//        3. Draw all the transparent objects in sorted order.
        bindTextures({&texture2DWindow});
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < vegetation.size(); i++) {
            float distance = glm::distance(cameraPos, vegetation.at(i));
            sorted[distance] = vegetation[i];
        }
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.
                rend(); ++it) {
            modelMatrix.setIdentity();
            modelMatrix.translate(it->second);
            m = modelMatrix;
            m.mult(viewMatrix);
            m.mult(projectMatrix);
            for (unsigned int i = 0; i < planeMeshes.size(); i++)
                drawMesh(*planeMeshes[i], m);
        }

        SoftRender::draw(transform);
        setBlendEnabled(false);
        setDepthTest(false);
        setFaceCull(false);
    }

    void Blend::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                         const std::vector<vec3> &uniforms) {
        Texture2D *texture = _bindTextures["texture0"];
        if (texture) {
            const vec4 &color = texture->sample(u, v);
            SoftRender::setPixel(x, y, z, color);
        }
    }

    Blend::~Blend() {
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            delete planeMeshes[i];
        for (unsigned int i = 0; i < boxMeshes.size(); i++)
            delete boxMeshes[i];
    }

TEST_NODE_IMP_END