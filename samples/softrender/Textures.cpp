//
// Created by huangkun on 2018/8/19.
//

#include "Textures.h"

TEST_NODE_IMP_BEGIN

    Textures::Textures() {
        TEX_WIDTH = 1024;
        TEX_HEIGHT = 1024;
    }

    bool Textures::init() {
        SoftRender::init();
        verticesPlane = {
                // postions                 // texture coords
                0.5f, 0.5f, 0.0f, 2.0f, 2.0f, // top right
                0.5f, -0.5f, 0.0f, 2.0f, -1.0f, // bottom right
                -0.5f, -0.5f, 0.0f, -1.0f, -1.0f, // bottom left
                -0.5f, 0.5f, 0.0f, -1.0f, 2.0f, // bottom top
        };
        indicesPlane = { // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
        };
        Mesh *mesh = new Mesh(createVertexs(verticesPlane, 3, 0, 2), indicesPlane);
        planeMeshes.push_back(mesh);

        texture2D.load("../res/smile.jpg");
        return true;
    }

    void Textures::draw(const mat4 &transform) {
        clearColor(25, 25, 25, 255);

        modelMatrix.setIdentity();
        viewMatrix.setIdentity();
        projectMatrix.setIdentity();

        // top left
        modelMatrix.translate(Vector(-0.5f, 0.5f, 0));
        Matrix m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        bindTextures({&texture2D});
        texture2D.setWrap(GL_REPEAT);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);


        SoftRender::draw(transform);
    }

    void Textures::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                            const std::vector<vec3> &uniforms) {
        Texture2D *texture = _bindTextures["texture0"];
        if (texture) {
            const vec3 &textureColor = texture->sample(u, v);
            vec4 color = vec4(vec3(textureColor), 255);
            SoftRender::setPixel(x, y, z, color);
        }
    }

    Textures::~Textures() {
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            delete planeMeshes[i];
    }

TEST_NODE_IMP_END