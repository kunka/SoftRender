//
// Created by huangkun on 2018/6/17.
//

#ifndef GL_MATHUTIL_H
#define GL_MATHUTIL_H

#include "stdio.h"
#include <glm/glm.hpp>

#define FLT_EQL(f1, f2) (std::abs(f1-f2)<FLT_EPSILON)

class MathUtil {
public:
    static float clamp(float x, float min, float max);
};

class Vector {
public:
    float x, y, z, w;

    Vector();

    Vector(float x, float y, float z, float w = 1.0);

    Vector(const glm::vec3 &v);

    Vector(const glm::vec4 &v);

    float length();

    void add(const Vector &other);

    void sub(const Vector &other);

    float dot(const Vector &other);

    Vector cross(const Vector &other);

    void normalize();

    glm::vec2 vec2();

    glm::vec3 vec3();

    glm::vec4 vec4();
};

class Matrix {
public:
    /*
     * OPENGL右手坐标系，左乘矩阵，右乘列向量，列主序存储
     * | 00 10 20 30 |
     * | 01 11 21 31 |
     * | 02 12 22 32 |
     * | 03 13 23 33 |
     */
    float m[4][4];

    Matrix();

    Matrix(const Matrix &copy);

    void setIdentity();

    void setZero();

    void add(const Matrix &other);

    void sub(const Matrix &other);

    void mult(const Matrix &other);

    Vector applyVector(const Vector &v) const;

    Vector applyPoint(const Vector &v) const;

    void scale(float scale);

    void scale(const Vector &scale);

    void translate(const Vector &offset);

    void rotate(const Vector &pivot, float theta);

    static Matrix lookAt(const Vector &eye, const Vector &target, const Vector &up);

    static Matrix perspective(float fovy, float aspect, float near, float far);

    static Matrix ortho(float left, float right, float bottom, float top, float near, float far);
};

#endif //GL_MATHUTIL_H
