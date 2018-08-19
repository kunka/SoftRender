//
// Created by huangkun on 2018/6/25.
//

#ifndef GL_TEXTURECUBE_H
#define GL_TEXTURECUBE_H

#include "RastCube.h"
#include "Mesh.h"

TEST_NODE_BEGIN2(TextureCube, RastCube)

    protected:
        void fill(std::vector<VertexCoords> &verts, const std::vector<vec3> &uniforms = {});

//        void dda_line(const vec4 &pa, const vec4 &pb, const vec2 &uv, const vec2 &uv2);

        void dda_line(const VertexCoords &vert1, const VertexCoords &vert2, const std::vector<vec3> &uniforms = {});

        virtual void setPixel(int x, int y, float z, float u, float v, vec3 varying[],
                              const std::vector<vec3> &uniforms = {});

        void bindTextures(const std::vector<Texture2D *> &textures);

        void bindTexture(const Texture2D *texture);

        void drawMesh(const Mesh &mesh, const Matrix &mvp, int varyingCount = 0);

        Texture2D texture2D;
TEST_NODE_END(TextureCube)

#endif //GL_TEXTURECUBE_H
