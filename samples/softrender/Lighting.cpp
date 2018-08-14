//
// Created by huangkun on 2018/8/8.
//

#include "Lighting.h"
#include "Input.h"

TEST_NODE_IMP_BEGIN

    Lighting::Lighting() {
        vertices = {
                // positions          // normals           // texture coords
                // back face
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, // bottom-left
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 1.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 1.0f, 1.0f, // top-right
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, // bottom-left
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 1.0f, // top-left
                // front face
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, // top-right
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 1.0f, // top-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, // bottom-left
                // left face
                -0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
                -0.5f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
                -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
                -0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
                // right face
                0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
                0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
                0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
                0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 1.0f, // top-left
                0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-right
                -0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, // top-right
                // top face
                -0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, // top-left
                0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f, // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
                -0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, // top-left
                -0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f  // bottom-left
        };
        texture2D.load("../res/container.jpg");

//        cameraPos = vec3(0.0f, 0.0f, 3.0f);
//        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

        lightPos = vec3(0.0, 1.0, 0.0);
        lightColor = vec3(1.0, 1.0, 1.0);
    }

    // pitch and yaw
    void Lighting::draw(const mat4 &transform) {
        setDepthTest(true);
        setFaceCull(true);
        clearColor(50, 50, 50, 255);
        clearDepth();

        modelMatrix.setIdentity();
        modelMatrix.translate(Vector(-0.5f, -1.0f, 0.0f));
//        modelMatrix.rotate(Vector(0, 1, 0), 2 * 3.14f * sin(glfwGetTime() / 4));
        modelMatrix.rotate(Vector(1, 1, 0), radians(45.0f));
        vec3 target = cameraPos + cameraDir;
        viewMatrix = Matrix::lookAt(Vector(cameraPos.x, cameraPos.y, cameraPos.z),
                                    Vector(target.x, target.y, target.z),
                                    Vector(cameraUp.x, cameraUp.y, cameraUp.z));
        Matrix m = modelMatrix;
        m.mult(viewMatrix);
        m.mult(projectMatrix);

        vec2 last;
        int column = 8;
        vec4 triangle[3];
        vec3 triangleWorld[3];
        Vector normals[3];
        vec2 uv[3];
        for (int i = 0; i < vertices.size(); i += column * 3) {
            for (int j = 0; j < 3; j++) {
                int row = i + j * column;
                vec4 p = vec4(vertices.at(row), vertices.at(row + 1),
                              vertices.at(row + 2), 1.0);
                // 模型 --> 世界 --> 相机空间 --> 齐次裁剪空间，xyz ~ [-w，w], w = Z(相机空间)
                Vector v = m.applyPoint(Vector(p.x, p.y, p.z));

                triangle[j] = vec4(v.x, v.y, v.z, v.w);
                normals[j] = Vector(vertices.at(row + 3), vertices.at(row + 4), vertices.at(row + 5));
                uv[j] = vec2(vertices.at(row + 6), vertices.at(row + 7));

                Vector v0 = modelMatrix.applyPoint(Vector(p.x, p.y, p.z));
                triangleWorld[j] = vec3(v0.x, v0.y, v0.z);
//                triangleWorld[j] = model * p;
            }
            if (cvvCull(triangle)) {
                // 简单CVV裁剪，三角形3个点有一个不在cvv立方体内将被裁剪掉
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
//            log("%f,%f,%f", triangleWorld[0].x, triangleWorld[0].y, triangleWorld[0].z);
//            mat4 matrix = mat4(modelMatrix.m[0][0], modelMatrix.m[1][0], modelMatrix.m[2][0], modelMatrix.m[3][0],
//                               modelMatrix.m[0][1], modelMatrix.m[1][1], modelMatrix.m[2][1], modelMatrix.m[3][1],
//                               modelMatrix.m[0][2], modelMatrix.m[1][2], modelMatrix.m[2][2], modelMatrix.m[3][2],
//                               modelMatrix.m[0][3], modelMatrix.m[1][3], modelMatrix.m[2][3], modelMatrix.m[3][3]);
//            matrix = glm::transpose(glm::inverse(matrix));
            vec3 norms[] = {
//                    matrix * vec4(normals[0].x, normals[0].y, normals[0].z, 0),
//                    matrix * vec4(normals[1].x, normals[1].y, normals[1].z, 0),
//                    matrix * vec4(normals[2].x, normals[2].y, normals[2].z, 0),
                    modelMatrix.applyVector(normals[0]).vec3(),
                    modelMatrix.applyVector(normals[1]).vec3(),
                    modelMatrix.applyVector(normals[2]).vec3(),
            };

            vec3 v1[] = {triangleWorld[0], norms[0]};
            vec3 v2[] = {triangleWorld[1], norms[1]};
            vec3 v3[] = {triangleWorld[2], norms[2]};
            std::vector<VertexCoords> verts = {
                    createVertexCoords(triangle[0], uv[0] * triangle[0].w, v1, 2),
                    createVertexCoords(triangle[1], uv[1] * triangle[1].w, v2, 2),
                    createVertexCoords(triangle[2], uv[2] * triangle[2].w, v3, 2),
            };
            fill(verts);
        }
        // draw light
        drawPoint(lightPos, vec4(lightColor.x * 255, lightColor.y * 255, lightColor.z * 255, 255));

        SoftRender::draw(transform);
        setDepthTest(false);
        setFaceCull(false);
    }

    void Lighting::setPixel(int x, int y, int z, float u, float v, vec3 varying[],
                            const std::vector<vec4> &uniforms) {
        const vec4 &textureColor = texture2D.sample(u, v);
        SoftRender::setPixel(x, y, z, textureColor);

//        vec3 fragPos = varying[0];
//        vec3 normal = glm::normalize(varying[1]);
//        vec3 lightDir = glm::normalize(lightPos - fragPos);
//        float diff = std::max(glm::dot(normal, lightDir), 0.0f);
//
//        // ambient
//        vec3 ambient = lightColor * vec3(0.2f, 0.2f, 0.2f);
//        // diffuse
//        vec3 diffuse = lightColor * diff;
//        // specular
//        vec3 viewDir = glm::normalize(cameraPos - fragPos);
//
//        // Phong
////        vec3 reflectDir = glm::reflect(-lightDir, normal);
////        float spec = pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), 32.0f);
//        // BlinnPhong
//        vec3 halfwayDir = glm::normalize(lightDir + viewDir);
//        float spec = pow(std::max(glm::dot(normal, halfwayDir), 0.0f), 16.0f);
//
//        vec3 specularColor = lightColor * vec3(0.5f, 0.5f, 0.5f);
//        vec3 specular = specularColor * spec;
//
//        vec4 color = vec4(vec3(textureColor) * (ambient + diffuse) + specular, 255);
//        SoftRender::setPixel(x, y, z, color);
    }

    Lighting::~Lighting() {
    }

TEST_NODE_IMP_END