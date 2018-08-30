//
// Created by huangkun on 2018/8/30.
//

#ifndef GL_DEFERREDSHADING_H
#define GL_DEFERREDSHADING_H

#include "CustomDraw.h"
#include "Model.h"

/*
 * The reality is that your GPU and GLSL are really bad at optimizing loops and branches.
 * The reason for this is that shader execution on the GPU is highly parallel and most architectures
 * have a requirement that for large collection of threads they need to run the exact same shader code
 * for it to be efficient. This often means that a shader is run that always executes all branches of
 * an if statement to ensure the shader runs are the same, making our previous radius check optimization
 * completely useless; we’d still calculate lighting for all light sources!
 */
TEST_NODE_BEGIN(DeferredShading)

        unsigned int gBuffer;
        unsigned int gPosition;
        unsigned int gNormal;
        unsigned int gAlbedoSpec; // color + specular intensity
        unsigned int quadVAO;
        unsigned int quadVBO;

        Shader visualizeGBufferShader;
        Shader deferredShader;
        Shader lightShader;

        Model *modelObject;
        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec3> lightColors;

TEST_NODE_END(DeferredShading)

#endif //GL_DEFERREDSHADING_H
