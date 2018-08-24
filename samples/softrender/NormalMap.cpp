//
// Created by huangkun on 2018/8/24.
//

#include "NormalMap.h"

TEST_NODE_IMP_BEGIN

    NormalMap::NormalMap() {
        TEX_WIDTH = 768;
        TEX_HEIGHT = 768;
    }

    bool NormalMap::init() {
        SoftRender::init();
        verticesPlane = {
                // postions       // colors              // texture coords
                0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
                -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // bottom top
        };
        indicesPlane = { // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
        };

        texture2D.load("../res/brickwall.jpg");
        texture2D.setMagFilter(GL_LINEAR);
        texture2D.setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
        texture2D.genMipmaps();

        texture2DNormal.load("../res/brickwall_normal.jpg");
        texture2DNormal.setMagFilter(GL_LINEAR);
        texture2DNormal.setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
        texture2DNormal.genMipmaps();

        Mesh *mesh = new Mesh(createVertexs(verticesPlane, 3, 4, 2), indicesPlane);
        planeMeshes.push_back(mesh);

        cameraPos = vec3(-0.9f, 0.2f, 0.8f);
        cameraDir = vec3(0.0f, 0.0f, 0.0f) - cameraPos;
        lightPos = vec3(0.3f, -0.1f, 0.3f);
    }

    void NormalMap::draw(const mat4 &transform) {
        setDepthTest(true);
        setFaceCull(false);
        clearColor(25, 25, 25, 255);
        clearDepth();

        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(Vector(cameraPos), Vector(target), Vector(cameraUp));

        // draw plane
        bindTextures({&texture2D, &texture2DNormal});
        modelMatrix.setIdentity();
        Matrix m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 1);

        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(0, -0.5, 0.5));
        modelMatrix.rotate(Vector(1, 0, 0), radians(90.0f));
        m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            drawMesh(*planeMeshes[i], m, 1);


        // draw light
        drawPoint(lightPos, vec4(255, 255, 255, 255));
        SoftRender::draw(transform);
        setDepthTest(false);
    }

    void NormalMap::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                             const std::vector<vec3> &uniforms, float dudx, float dvdy) {
        Texture2D *texture = _bindTextures["texture0"];
        Texture2D *texture1 = _bindTextures["texture1"];
        if (texture && texture1) {
            const vec3 &textureColor = texture->sample(u, v, dudx, dvdy);
//            vec4 color = vec4(vec3(textureColor), 255);
//            SoftRender::setPixel(x, y, z, color);


            vec3 fragPos = varying[0];
            float distance = glm::length(lightPos - fragPos);
            float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

//            vec3 normal = glm::normalize(varying[1]);
            vec3 normal = texture1->sample(u, v, dudx, dvdy);
            normal /= 255.0f;
            normal = glm::normalize(normal * 2.0f - 1.0f);

            vec3 lightDir = glm::normalize(lightPos - fragPos);
            float diff = std::max(glm::dot(normal, lightDir), 0.0f);

            // ambient
            vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
            // diffuse
            vec3 diffuse = vec3(0.5f, 0.5f, 0.5f) * diff;
            // specular
            vec3 viewDir = glm::normalize(cameraPos - fragPos);
            // BlinnPhong
            vec3 halfwayDir = glm::normalize(lightDir + viewDir);
            float spec = pow(std::max(glm::dot(normal, halfwayDir), 0.0f), 16.0f);

            spec *= 255.0f;
            vec3 specular = vec3(0.2f, 0.2f, 0.2f) * spec;

            ambient *= vec3(textureColor);
            diffuse *= vec3(textureColor);
            vec4 color = vec4((ambient + diffuse + specular) * attenuation, 255);
            SoftRender::setPixel(x, y, z, color);
        }
    }

    NormalMap::~NormalMap() {
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            delete planeMeshes[i];
    }

TEST_NODE_IMP_END