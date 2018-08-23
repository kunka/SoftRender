//
// Created by huangkun on 2018/8/18.
//

#include "ShadowMap.h"

TEST_NODE_IMP_BEGIN

    ShadowMap::ShadowMap() {
//        TEX_WIDTH = 1024;
//        TEX_HEIGHT = 1024;
    }

    bool ShadowMap::init() {
        SoftRender::init();
        vertices = {
                // back face
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, // bottom-left
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, // top-right
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, // bottom-left
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.5f, // top-left
                // front face
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, // bottom-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, // top-right
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, // top-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, // bottom-left
                // left face
                -0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, // top-right
                -0.5f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.5f, // top-left
                -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, // bottom-left
                -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, // bottom-left
                -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
                -0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, // top-right
                // right face
                0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, // top-left
                0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, // top-right
                0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, // bottom-right
                0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, // top-left
                0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.5f, // top-left
                0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.0f, // bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-right
                -0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f, // top-right
                // top face
                -0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, // top-left
                0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.0f, // bottom-right
                -0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, // top-left
                -0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f  // bottom-left
        };
        verticesPlane = {
                // postions               // normal          // texture coords
                0.5f * 10, 0.5f * 10, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f * 10, 1.0f * 10, // top right
                0.5f * 10, -0.5f * 10, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f * 10, 0.0f, // bottom right
                -0.5f * 10, -0.5f * 10, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
                -0.5f * 10, 0.5f * 10, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f * 10, // bottom top
        };
        indicesPlane = { // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
        };
        verticesQuad = {
                // positions       // normals(uselsss) // texture Coords
                // to use the same vert shader
                -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        };

        texture2DBox.load("../res/container.jpg");
        texture2DPlane.load("../res/net.jpg");
        texture2DPlane.setWrap(GL_REPEAT);
        texture2DPlane.setMagFilter(GL_LINEAR);
        texture2DPlane.setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
        texture2DPlane.genMipmaps();

        Mesh *mesh = new Mesh(createVertexs(verticesPlane, 3, 3, 2), indicesPlane);
        planeMeshes.push_back(mesh);

        mesh = new Mesh(createVertexs(vertices, 3, 3, 2));
        boxMeshes.push_back(mesh);

        mesh = new Mesh(createVertexs(verticesQuad, 3, 3, 2));
        quadMeshes.push_back(mesh);

        cameraPos = vec3(0.3f, 4.0f, 5.0f);
        cameraDir = vec3(0.0f, 2.0f, 0.0f) - cameraPos;
        lightPos = vec3(-2, 4, -3);

        depthFBO = new float[TEX_WIDTH * TEX_HEIGHT];
    }

    void ShadowMap::draw(const mat4 &transform) {
        setDepthTest(true);
        clearDepth();

        renderType = 0;
        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(cameraPos, target, cameraUp);

        switch (renderType) {
            case 0 : {
                // render origin scene without shadow
                clearColor(25, 50, 75, 255);
                renderScene(renderType);
                // draw light
                drawPoint(lightPos, vec4(255, 255, 255, 255));
                break;
            }
            case 1: {
                // visual depth 1

                break;
            }
            case 2: {
                break;
            }
            case 3: {

                break;
            }
            default:
                break;
        }


        SoftRender::draw(transform);
        setDepthTest(false);
    }

    void ShadowMap::renderScene(int type, bool faceCulling) {
        if (faceCulling) {
            setFaceCull(true);
        }
        // draw box
        bindTextures({&texture2DBox});
        modelMatrix.setIdentity();
        modelMatrix.translate(vec3(-1, 2, -1.0));
        Matrix m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < boxMeshes.size(); i++)
            drawMesh(*boxMeshes[i], m, 2);

        modelMatrix.setIdentity();
        modelMatrix.translate(vec3(1, 0.5, -1.0));
        modelMatrix.rotate(Vector(0, 1, 0), radians(-30.0f));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < boxMeshes.size(); i++)
            drawMesh(*boxMeshes[i], m, 2);

        modelMatrix.setIdentity();
        modelMatrix.translate(vec3(-2, 0.5, 0.0));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < boxMeshes.size(); i++)
            drawMesh(*boxMeshes[i], m, 2);

        if (faceCulling) {
            setFaceCull(false);
        }

        // draw plane
        bindTextures({&texture2DPlane});
        modelMatrix.setIdentity();
        modelMatrix.rotate(Vector(1, 0, 0), radians(-90.0f));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 2);
    }

    void ShadowMap::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                             const std::vector<vec3> &uniforms, float dudx, float dvdy) {
        Texture2D *texture = _bindTextures["texture0"];
        if (texture) {
            const vec3 &textureColor = texture->sample(u, v, dudx, dvdy);
//            vec4 color = vec4(vec3(textureColor), 255);
//            SoftRender::setPixel(x, y, z, color);

            vec3 fragPos = varying[0];
            vec3 normal = glm::normalize(varying[1]);
            vec3 lightDir = glm::normalize(lightPos - fragPos);
            float diff = std::max(glm::dot(normal, lightDir), 0.0f);

            // ambient
            vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
            // diffuse
            vec3 diffuse = vec3(1.0f, 1.0f, 1.0f) * diff;
            // specular
            vec3 viewDir = glm::normalize(cameraPos - fragPos);
            // BlinnPhong
            vec3 halfwayDir = glm::normalize(lightDir + viewDir);
            float spec = pow(std::max(glm::dot(normal, halfwayDir), 0.0f), 16.0f);

            spec *= 255.0f;
            vec3 specular = vec3(0.5f, 0.5f, 0.5f) * spec;

            vec4 color = vec4(vec3(textureColor) * (ambient + diffuse) + specular, 255);
            SoftRender::setPixel(x, y, z, color);
        }
    }

    ShadowMap::~ShadowMap() {
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            delete planeMeshes[i];
        for (unsigned int i = 0; i < boxMeshes.size(); i++)
            delete boxMeshes[i];
        for (unsigned int i = 0; i < quadMeshes.size(); i++)
            delete quadMeshes[i];

        delete[] depthFBO;
    }

TEST_NODE_IMP_END