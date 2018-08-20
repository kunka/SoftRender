//
// Created by huangkun on 2018/8/19.
//

#include "TextureFilter.h"

TEST_NODE_IMP_BEGIN

    TextureFilter::TextureFilter() {
        TEX_WIDTH = 1024;
        TEX_HEIGHT = 1024;
    }

    bool TextureFilter::init() {
        SoftRender::init();
        verticesPlane = {
                // postions                 // texture coords
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

        texture2D.load("../res/cat_s.jpg");
        texture2D2.load("../res/cat.jpg");
        return true;
    }

    void TextureFilter::draw(const mat4 &transform) {
        clearColor(25, 50, 75, 255);

        modelMatrix.setIdentity();
        viewMatrix.setIdentity();
        projectMatrix.setIdentity();

        bindTextures({&texture2D});
        // top left
        modelMatrix.translate(Vector(-0.5f, 0.5f, 0));
        Matrix m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D.setMagFilter(GL_NEAREST);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);

        // top right
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0.5f, 0.5f, 0));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D.setMagFilter(GL_LINEAR);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);


        texture2D2.setWrap(GL_CLAMP_TO_EDGE);
        bindTextures({&texture2D2});
        // bottom left
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(-0.5f, -0.5f, 0));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D2.setMinFilter(GL_LINEAR);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);


        // bottom right up, min filter nearest
        float scale = 0.5f;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0.5 - scale / 2, -0.5 + scale / 2, 0));
        modelMatrix.scale(vec3(scale, scale, 1));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D2.setMinFilter(GL_NEAREST);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);

        scale = 0.25f;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0.5 + scale / 2, -0.5 + scale / 2, 0));
        modelMatrix.scale(vec3(scale, scale, 1));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D2.setMinFilter(GL_NEAREST);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);

        scale = 0.125f;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0.5 + scale * 2 + scale / 2, -0.5 + scale / 2, 0));
        modelMatrix.scale(vec3(scale, scale, 1));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D2.setMinFilter(GL_NEAREST);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);

        // bottom right down, min filter linear
        scale = 0.5;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0.5 - scale / 2, -0.5 - 0.5 + scale / 2, 0));
        modelMatrix.scale(vec3(scale, scale, 1));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D2.setMinFilter(GL_LINEAR);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);


        scale = 0.25f;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0.5 + scale / 2, -0.5 - 0.5 + scale / 2, 0));
        modelMatrix.scale(vec3(scale, scale, 1));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D2.setMinFilter(GL_LINEAR);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);

        scale = 0.125f;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0.5 + scale * 2 + scale / 2, -0.5 - 0.5 + scale / 2, 0));
        modelMatrix.scale(vec3(scale, scale, 1));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        texture2D2.setMinFilter(GL_LINEAR);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);

        SoftRender::draw(transform);
    }

    void TextureFilter::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                                 const std::vector<vec3> &uniforms, float dudx, float dvdy) {
        Texture2D *texture = _bindTextures["texture0"];
        if (texture) {
            const vec3 &textureColor = texture->sample(u, v, dudx, dvdy);
            vec4 color = vec4(vec3(textureColor), 255);
            SoftRender::setPixel(x, y, z, color);
        }
    }

    TextureFilter::~TextureFilter() {
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            delete planeMeshes[i];
    }

TEST_NODE_IMP_END