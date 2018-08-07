//
// Created by huangkun on 2018/6/16.
//

#ifndef GL_SOFTRENDER_H
#define GL_SOFTRENDER_H

#include "CustomDraw.h"

#define TEX_WIDTH   512
#define TEX_HEIGHT  512

/*
 * 顶点着色，MV变换
 * P投影
 * cvv裁剪
 * 屏幕坐标映射
 *
 *
 * 三角形数据设置
 * 像素着色（纹理采样）
 * Alpha测试
 * 深度测试
 * 模版测试
 * 混合
 */
TEST_NODE_BEGIN(SoftRender)

        vec3 interp(const vec3 &v1, const vec3 &v2, float t);

        vec2 interp(const vec2 &v1, const vec2 &v2, float t);

        float interp(float f1, float f2, float t);

    protected:
        unsigned int genTexture();

        void setPixel(int x, int y, float depth, const vec4 &color);

        void setPixel(int x, int y, float depth, const vec3 &color);

        void setDepthTest(bool depthTest);

        void clearColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);

        void clearDepth();

        bool cvvCull(vec4 triangle[3]);

        bool inCvv(const vec4 &vector);

        unsigned int texture = -1;

        GLubyte texData[TEX_WIDTH][TEX_HEIGHT][4];
        bool depthTest;
        float *depthBuff;
        Rect clipRect;
        bool isClipRect;

TEST_NODE_END(SoftRender)


#endif //GL_SOFTRENDER_H
