//
// Created by huangkun on 2018/6/16.
//

#ifndef GL_SOFTRENDER_H
#define GL_SOFTRENDER_H

#include "CustomDraw.h"
#include "MathUtil.h"
#include "Texture2D.h"

#define TEX_WIDTH   1024
#define TEX_HEIGHT  1024

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

struct VertexCoords {
    vec4 p;
    vec2 uv;
    std::vector<vec4> varying;
};

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

        void pointToScreen(vec4 *triangle, int num = 3);

        bool inCvv(const vec4 &vector);

        unsigned int texture = -1;

        GLubyte texData[TEX_WIDTH][TEX_HEIGHT][4];
        bool depthTest;
        bool faceCulling;
        float *depthBuff;
        Rect clipRect;
        bool isClipRect;
        Matrix modelMatrix;
        Matrix viewMatrix;
        Matrix projectMatrix;

TEST_NODE_END(SoftRender)


#endif //GL_SOFTRENDER_H
