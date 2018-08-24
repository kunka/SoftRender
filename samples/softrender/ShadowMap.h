//
// Created by huangkun on 2018/8/18.
//

#ifndef GL_SHADOWMAP_H
#define GL_SHADOWMAP_H


#include "TextureCube.h"

TEST_NODE_BEGIN2(ShadowMap, TextureCube)

    public:
        virtual bool init() override;

    protected:

        virtual void setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms, float dudx, float dvdy) override;

        void renderScene(int type, bool faceCulling = false);

        float inShadow(vec4 &fragPosLightSpace, float bias);

        std::vector<float> verticesPlane;
        std::vector<float> verticesQuad;
        std::vector<unsigned int> indicesPlane;

        Texture2D texture2DBox;
        Texture2D texture2DPlane;

        std::vector<Mesh *> planeMeshes;
        std::vector<Mesh *> boxMeshes;
        std::vector<Mesh *> quadMeshes;

        vec3 lightPos;
        float *depthFBO;
        int renderType;
        Matrix lightSpaceMatrix;

TEST_NODE_END(ShadowMap)
#endif //GL_SHADOWMAP_H
