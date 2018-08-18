//
// Created by huangkun on 2018/8/10.
//

#include "LightingMap.h"

TEST_NODE_IMP_BEGIN

    LightingMap::LightingMap() {
        diffuseTexture.load("../res/container2.png");
        specularTexture.load("../res/container2_specular.png");
        diffuseTexture.setFilter(TF_LINEAR);
        specularTexture.setFilter(TF_LINEAR);
    }

    void LightingMap::draw(const mat4 &transform) {
        Lighting::draw(transform);
    }

    void LightingMap::setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                               const std::vector<vec3> &uniforms) {
        const vec4 &textureColor = diffuseTexture.sample(u, v);

        vec3 fragPos = varying[0];
        vec3 normal = glm::normalize(varying[1]);
        vec3 lightDir = glm::normalize(lightPos - fragPos);
        float diff = std::max(glm::dot(normal, lightDir), 0.0f);

        // ambient
        vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
        // diffuse
        vec3 diffuse = vec3(0.5, 0.5, 0.5) * diff;
        // specular
        vec3 viewDir = glm::normalize(cameraPos - fragPos);

        // Phong
        vec3 reflectDir = glm::reflect(-lightDir, normal);
        float spec = pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), 32.0f);
        // BlinnPhong
//        vec3 halfwayDir = glm::normalize(lightDir + viewDir);
//        float spec = pow(std::max(glm::dot(normal, halfwayDir), 0.0f), 32.0f);

        const vec4 &specularColor = specularTexture.sample(u, v);
        vec3 specular = specularColor * spec;

        vec4 color = vec4(vec3(textureColor) * (diffuse + ambient) + specular, 255);
        SoftRender::setPixel(x, y, z, color);
    }

    LightingMap::~LightingMap() {
    }

TEST_NODE_IMP_END