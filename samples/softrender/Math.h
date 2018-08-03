//
// Created by huangkun on 2018/6/17.
//

#ifndef GL_MATH_H
#define GL_MATH_H

#include "stdio.h"

#define FLT_EQL(f1, f2) (std::abs(f1-f2)<FLT_EPSILON)

class Vector {
public:
    float x, y, z, w;

    Vector();

    Vector(float x, float y, float z, float w = 1.0);

    float length();

    void add(const Vector &other);

    void sub(const Vector &other);

    float dot(const Vector &other);

    Vector cross(const Vector &other);

    Vector interp(const Vector &v1, const Vector &v2, float t);

    void normalize();
};

class Matrix {
public:
    float m[4][4];

    Matrix();

    void setIdentity();

    void setZero();

    void add(const Matrix &other);

    void sub(const Matrix &other);

    void mult(const Matrix &other);

    Vector apply(const Vector &v);

    void scale(float scale);

    void scale(const Vector &scale);

    void translate(const Vector &offset);

    void rotate(const Vector &pivot, float theta);

    void lookAt(const Vector &eye, const Vector &target, const Vector &up);

    void perspective(float fovy, float aspect, float near, float far);
};

#endif //GL_MATH_H
