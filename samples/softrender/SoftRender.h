//
// Created by huangkun on 2018/6/16.
//

#ifndef GL_SOFTRENDER_H
#define GL_SOFTRENDER_H

#include "CustomDraw.h"
#include "MathUtil.h"
#include "Texture2D.h"
#include <unordered_map>

/*
 * -------[顶点处理]--------
 * 顶点坐标变换，O(模型空间) --> M（世界坐标） --> V（相机空间） --> P（投影坐标）
 *剔除
    - Depth Test: Occluded surfaces: hidden surface removal (visibility)
    - Backface Cull: Back faces: back face culling (CCW三角面剔除 or 计算三角面法线与视线夹角剔除)
    - CVV Cull: Faces outside view volume: viewing frustrum culling
 * 屏幕坐标映射（ViewPort变换，透视除法 --> NDC坐标 --> 屏幕坐标）
 *
 * -------[光栅化]--------
 * 三角形顶点插值，UV插值，自定义插值（顶点法向量插值等）
 * 扫面线填充2D三角形，2D/3D直线裁剪
 * 像素着色（颜色插值 or 纹理UV插值，纹理包裹/滤波/Mipmaps）
 * TODO:Alpha测试
 * 深度测试
 * TODO:模版测试
 * TODO:混合
 *
 * -------[其他]--------
 * Phong光照，ambient/diffuse/specular，材质/光照贴图
 * 模型加载，Model，Mesh
 * 深度测试，深度可视化
 * Blend
 */

#define MAX_VARYING_COUNT 5
#define MAX_TEX_SIZE 1024

struct VertexCoords {
    vec4 p;
    vec2 uv;
    vec3 varying[MAX_VARYING_COUNT];
    int varyingCount;

    inline void interp(vec3 ret[], vec3 v1[], vec3 v2[], float t) const {
        for (int i = 0; i < varyingCount; i++) {
            ret[i].x = v1[i].x + (v2[i].x - v1[i].x) * t;
            ret[i].y = v1[i].y + (v2[i].y - v1[i].y) * t;
            ret[i].z = v1[i].z + (v2[i].z - v1[i].z) * t;
        }
    }

    inline void divideW(vec3 ret[], float w) const {
        if (w != 0) {
            for (int i = 0; i < varyingCount; i++) {
                ret[i] /= w;
            }
        }
    }
};

static VertexCoords createVertexCoords(const vec4 &p, const vec2 &uv, const vec3 varying[], int varyingCount) {
    VertexCoords v;
    v.p = p;
    v.uv = uv;
//    if (varyingCount > 0) {
//        memcpy(v.varying, varying, sizeof(varying[0]) * varyingCount);
//    }
    for (int i = 0; i < varyingCount; i++) {
        v.varying[i] = varying[i];
    }
    v.varyingCount = varyingCount;
    return v;
}

TEST_NODE_BEGIN(SoftRender)

        inline float interp(float f1, float f2, float t) {
            return f1 + (f2 - f1) * t;
        }

        inline vec2 interp(const vec2 &v1, const vec2 &v2, float t) {
            return vec2(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t);
        }

        inline vec3 interp(const vec3 &v1, const vec3 &v2, float t) {
            return vec3(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t);
        }

        inline vec4 interp(const vec4 &v1, const vec4 &v2, float t) {
            return vec4(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t,
                        v1.w + (v2.w - v1.w) * t);
        }

        template<typename T>
        inline void swap(T &v1, T &v2) {
            T tmp = v1;
            v1 = v2;
            v2 = tmp;
        }

        virtual bool init() override;

    protected:

        unsigned int genTexture();

        virtual void setPixel(int x, int y, float z, float u, float v);

        void setPixel(int x, int y, float depth, const vec4 &color);

        void setPixel(int x, int y, float depth, const vec3 &color);

        void drawPoint(const vec3 &worldPos, const vec3 &color);

        void setDepthTest(bool depthTest);

        void setFaceCull(bool cull);

        void setFaceCullMode(int mode);

        void clearColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);

        void clearDepth();

        bool cvvCull(vec4 triangle[3]);

        bool faceCull(vec4 triangle[3]);

        bool depthTest(int x, int y, float depth);

        void pointToScreen(vec4 *triangle, int num = 3);

        int encode(const vec4 &p);

        int encodeScreen(const vec4 &p);

        bool clip_3D_line(vec4 &p1, vec4 &p2);

        void setBlendEnabled(bool blend) { _blend = blend; }

        void setBlendFunc(int src, int dst) {
            _blendFuncSrc = src;
            _blendFuncDst = dst;
        }

        void bindDepthBuffer(float *depthFBO) {
            _depthFBO = depthFBO;
        }

        int TEX_WIDTH;
        int TEX_HEIGHT;

        unsigned int texture = -1;

        GLubyte *texData;
        float *_depthFBO = nullptr; // ref
        bool _depthTest;
        bool faceCulling;
        int faceCullingMode;
        bool _blend;
        int _blendFuncSrc;
        int _blendFuncDst;
        float *depthBuff;
        Rect clipRect;
        bool isClipRect;
        Matrix modelMatrix;
        Matrix viewMatrix;
        Matrix projectMatrix;
        std::unordered_map<std::string, Texture2D *> _bindTextures;

TEST_NODE_END(SoftRender)


#endif //GL_SOFTRENDER_H
