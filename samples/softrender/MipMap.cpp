//
// Created by huangkun on 2018/8/19.
//

#include "MipMap.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    MipMap::MipMap() {
        TEX_WIDTH = 1024;
        TEX_HEIGHT = 1024;
    }

    bool MipMap::init() {
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

        texture2D.load("../res/wood.png");
        texture2D.setWrap(GL_CLAMP_TO_EDGE);
        texture2D.setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
        texture2D.genMipmaps();
        texture2DNoMM.load("../res/wood.png");
        texture2DNoMM.setWrap(GL_CLAMP_TO_EDGE);
        texture2DNoMM.setMinFilter(GL_NEAREST);

        return true;
    }

    void MipMap::draw(const mat4 &transform) {
        clearColor(25, 50, 75, 255);

        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(Vector(cameraPos.x, cameraPos.y, cameraPos.z), Vector(target.x, target.y, target.z),
                                    Vector(cameraUp.x, cameraUp.y, cameraUp.z));

        if (Input::getInstance()->isKeyPressed(GLFW_KEY_SPACE)) {
            bindTextures({&texture2DNoMM});
        } else {
            bindTextures({&texture2D});
        }

//        modelMatrix.setIdentity();
//        viewMatrix.setIdentity();
//        projectMatrix.setIdentity();

        std::vector<vec3> ps = {
                vec3(-1, -1, 1.5),
                vec3(-3, 3.5, -5),
                vec3(3, 3, -5),
                vec3(0, 0, -10),
                vec3(-10, 1, -15),
                vec3(5, 2, -15),
                vec3(8, -2, -12),
                vec3(3, -4, -8),
        };
        for (int i = 0; i < ps.size(); i++) {
            modelMatrix.setIdentity();
//            modelMatrix.scale(Vector(1.9, 1.9, 1));
            modelMatrix.translate(ps[i]);
            Matrix m = modelMatrix;
            m.mult(viewMatrix);
            m.mult(projectMatrix);
            for (unsigned int i = 0; i < planeMeshes.size(); i++)
                drawMesh(*planeMeshes[i], m, 2);
        }

        SoftRender::draw(transform);
    }

    void MipMap::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                          const std::vector<vec3> &uniforms, float dudx, float dvdy) {
        Texture2D *texture = _bindTextures["texture0"];
        if (texture) {
            const vec3 &textureColor = texture->sample(u, v, dudx, dvdy);
            vec4 color = vec4(vec3(textureColor), 255);
            SoftRender::setPixel(x, y, z, color);
        }
    }

    MipMap::~MipMap() {
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            delete planeMeshes[i];
    }

TEST_NODE_IMP_END