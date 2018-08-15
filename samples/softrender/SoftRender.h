//
// Created by huangkun on 2018/6/16.
//

#ifndef GL_SOFTRENDER_H
#define GL_SOFTRENDER_H

#include "CustomDraw.h"
#include "MathUtil.h"
#include "Texture2D.h"
#include <unordered_map>

#define TEX_WIDTH   768
#define TEX_HEIGHT  768

/*
 * -------[顶点处理]--------
 * 顶点坐标变换，M(模型空间) --> W（世界坐标） --> V（相机空间） --> P（投影坐标）
 * cvv裁剪
 * 背面剔除(CCW三角面剔除 or 计算三角面法线与视线夹角剔除)
 * 屏幕坐标映射（透视除法 --> NDC坐标 --> 屏幕坐标）
 *
 * -------[光栅化]--------
 * 投影2D三角形数据设置，扫面线填充
 * 像素着色（颜色插值 or 纹理UV插值）
 * TODO:Alpha测试
 * 深度测试
 * TODO:模版测试
 * TODO:混合
 */

#define MAX_VARYING_COUNT 5
#define MAX_BIND_TEXTURES 10

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
};

static VertexCoords createVertexCoords(const vec4 &p, const vec2 &uv, const vec3 varying[], int varyingCount) {
    VertexCoords v;
    v.p = p;
    v.uv = uv;
    if (varyingCount > 0) {
        memcpy(v.varying, varying, sizeof(varying[0]) * varyingCount);
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

    protected:
        unsigned int genTexture();

        virtual void setPixel(int x, int y, int z, float u, float v);

        void setPixel(int x, int y, float depth, const vec4 &color);

        void setPixel(int x, int y, float depth, const vec3 &color);

        void drawPoint(const vec3 &worldPos, const vec3 &color);

        void setDepthTest(bool depthTest);

        void setFaceCull(bool cull);

        void clearColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);

        void clearDepth();

        bool cvvCull(vec4 triangle[3]);

        bool faceCull(vec3 triangle[3]);

        bool faceCull(const vec3 &triangle1, const vec3 &normal);

        bool depthTest(int x, int y, float depth);

        void pointToScreen(vec4 *triangle, int num = 3);

        bool inCvv(const vec4 &vector);

        unsigned int texture = -1;

        GLubyte texData[TEX_WIDTH][TEX_HEIGHT][4];
        bool _depthTest;
        bool faceCulling;
        float *depthBuff;
        Rect clipRect;
        bool isClipRect;
        Matrix modelMatrix;
        Matrix viewMatrix;
        Matrix projectMatrix;
        std::unordered_map<std::string, Texture2D *> _bindTextures;

TEST_NODE_END(SoftRender)


#endif //GL_SOFTRENDER_H
