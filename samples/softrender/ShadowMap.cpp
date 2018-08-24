//
// Created by huangkun on 2018/8/18.
//

#include "ShadowMap.h"

TEST_NODE_IMP_BEGIN

    ShadowMap::ShadowMap() {
        TEX_WIDTH = 768;
        TEX_HEIGHT = 768;
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

        cameraPos = vec3(0.3f, 4.0f, 5.0f);
        cameraDir = vec3(0.0f, 2.0f, 0.0f) - cameraPos;
        lightPos = vec3(-2, 4, -3);

        depthFBO = new float[TEX_WIDTH * TEX_HEIGHT];
    }

    void ShadowMap::draw(const mat4 &transform) {
        setDepthTest(true);

        renderType = 2;
        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(cameraPos, target, cameraUp);

        switch (renderType) {
            case 0 : {
                // render origin scene without shadow
                clearColor(25, 50, 75, 255);
                clearDepth();
                renderScene(renderType);
                // draw light
                drawPoint(lightPos, vec4(255, 255, 255, 255));
                break;
            }
            case 1: {
                // visual depth 1
                clearColor(0, 0, 0, 0);
                clearDepth();
                projectMatrix = Matrix::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
                viewMatrix = Matrix::lookAt(lightPos, Vector(0, 0, 0), Vector(0, 1, 0));
//                renderScene(renderType, true);
                renderScene(renderType);
                break;
            }
            case 2: {
                projectMatrix = Matrix::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
                viewMatrix = Matrix::lookAt(lightPos, Vector(0, 0, 0), Vector(0, 1, 0));
                bindDepthBuffer(depthFBO);
                clearDepth();
//                renderScene(renderType, true);
                renderScene(renderType);
                bindDepthBuffer(nullptr);

                clearColor(0, 0, 0, 0);
                clearDepth();
                renderType = 3;
                viewMatrix = Matrix::lookAt(cameraPos, target, cameraUp);
                projectMatrix = Matrix::perspective(radians(60.0f), (float) TEX_WIDTH / TEX_HEIGHT, 0.1, 100.0f);
                Matrix pj = Matrix::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
                Matrix vm = Matrix::lookAt(lightPos, Vector(0, 0, 0), Vector(0, 1, 0));
                lightSpaceMatrix = vm;
                lightSpaceMatrix.mult(pj);
                bindDepthBuffer(depthFBO);
                renderScene(renderType);
                bindDepthBuffer(nullptr);
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
            setFaceCullMode(GL_FRONT);
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
            setFaceCullMode(GL_BACK);
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

    float near = 0.1;
    float far = 10.0; // use a closer dis, the box isn't too far away.
    float LinearizeDepth2(float depth) {
        float z = depth * 2.0 - 1.0; // back to NDC
        return (2.0 * near * far) / (far + near - z * (far - near));
    }

    float ShadowMap::inShadow(vec4 &fragPosLightSpace, float bias) {
        fragPosLightSpace.x /= fragPosLightSpace.w;
        fragPosLightSpace.y /= fragPosLightSpace.w;
        fragPosLightSpace.z /= fragPosLightSpace.w;
        // [-1, 1] --> [0, 1]
        fragPosLightSpace.x = (fragPosLightSpace.x + 1.0f) * 0.5f;
        fragPosLightSpace.y = (fragPosLightSpace.y + 1.0f) * 0.5f;
        fragPosLightSpace.z = (fragPosLightSpace.z + 1.0f) * 0.5f;
        if (fragPosLightSpace.z > 1.0f) // over sampling
            return 0;

//        int x = MathUtil::clamp(fragPosLightSpace.x * TEX_WIDTH, 0, TEX_WIDTH - 1);
//        int y = MathUtil::clamp(fragPosLightSpace.y * TEX_HEIGHT, 0, TEX_HEIGHT - 1);
//        float depthInDepthMap = _depthFBO[y * TEX_WIDTH + x];
//        return fragPosLightSpace.z - bias > depthInDepthMap ? 1.0f : 0.0f;

        // PCF
        float currentDepth = fragPosLightSpace.z;
        float shadow = 0.0;
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                int x0 = MathUtil::clamp(fragPosLightSpace.x * TEX_WIDTH + x, 0, TEX_WIDTH - 1);
                int y0 = MathUtil::clamp(fragPosLightSpace.y * TEX_HEIGHT + y, 0, TEX_HEIGHT - 1);
                float depthInDepthMap = _depthFBO[y0 * TEX_WIDTH + x0];
                shadow += currentDepth - bias > depthInDepthMap ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;
        return shadow;
    }

    void ShadowMap::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                             const std::vector<vec3> &uniforms, float dudx, float dvdy) {
        if (renderType == 2 && _depthFBO) {
            if (x >= 0 && y >= 0 && x < TEX_WIDTH && y < TEX_HEIGHT) {
                int index = y * TEX_WIDTH + x;
                if (z < _depthFBO[index]) {
                    _depthFBO[index] = z;
                }
            }
            return;
        }
        if (renderType == 1) {
            vec4 color = vec4(vec3(z) * 255.0f, 255.0);
            SoftRender::setPixel(x, y, z, color);
            return;
        }

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


            float shadow = 0;
            if (renderType == 3 && _depthFBO) {
                vec4 fragPosLightSpace = lightSpaceMatrix.applyPoint(Vector(fragPos)).vec4();
                float bias = max(0.05f * (1.0f - glm::dot(normal, lightDir)), 0.005f);
                shadow = inShadow(fragPosLightSpace, bias);
            }
            ambient *= vec3(textureColor);
            diffuse *= vec3(textureColor);
            vec4 color = vec4(ambient + (diffuse + specular) * (1.0f - shadow), 255);
            SoftRender::setPixel(x, y, z, color);
        }
    }

    ShadowMap::~ShadowMap() {
        for (unsigned int i = 0; i < planeMeshes.size(); i++)
            delete planeMeshes[i];
        for (unsigned int i = 0; i < boxMeshes.size(); i++)
            delete boxMeshes[i];

        delete[] depthFBO;
    }

TEST_NODE_IMP_END