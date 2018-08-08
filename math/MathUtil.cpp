//
// Created by huangkun on 2018/6/17.
//

#include "MathUtil.h"
#include <math.h>
#include <string.h>

int MathUtil::clamp(int x, int min, int max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

Vector::Vector() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

Vector::Vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
}

float Vector::length() {
    return sqrt(x * x + y * y + z * z);
}

void Vector::add(const Vector &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w = 1.0;
}

void Vector::sub(const Vector &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w = 1.0;
}

float Vector::dot(const Vector &other) {
    return x * other.x + y * other.y + z * other.z;
}

Vector Vector::cross(const Vector &other) {
    return Vector(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x, 1.0);
}

Vector Vector::interp(const Vector &v1, const Vector &v2, float t) {
    return Vector(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t, w);
}

void Vector::normalize() {
    float len = sqrt(x * x + y * y + z * z);
    if (len > 0) {
        float inv = 1.0f / len;
        x *= inv;
        y *= inv;
        z *= inv;
    }
}

Matrix::Matrix() {
    setIdentity();
}

void Matrix::setIdentity() {
    memset(m, 0, 16 * sizeof(float));
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
}

void Matrix::setZero() {
    memset(m, 0, 16 * sizeof(float));
}

void Matrix::add(const Matrix &other) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            m[i][j] += other.m[i][j];
}

void Matrix::sub(const Matrix &other) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            m[i][j] -= other.m[i][j];
}

void Matrix::mult(const Matrix &other) {
    Matrix ret;
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            ret.m[i][j] = m[i][0] * other.m[0][j] +
                          m[i][1] * other.m[1][j] +
                          m[i][2] * other.m[2][j] +
                          m[i][3] * other.m[3][j];
    memcpy(m, ret.m, 16 * sizeof(float));
}

Vector Matrix::apply(const Vector &v) {
    Vector ret;
    ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
    ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
    ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
    ret.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
    return ret;
}

/*
 * s 0 0 0
 * 0 s 0 0
 * 0 0 s 0
 * 0 0 0 1
 */
void Matrix::scale(float scale) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            m[i][j] *= scale;
}

/*
 * s 0 0 0
 * 0 s 0 0
 * 0 0 s 0
 * 0 0 0 1
 */
void Matrix::scale(const Vector &scale) {
    m[0][0] *= scale.x;
    m[1][1] *= scale.y;
    m[2][2] *= scale.z;
}

/*
 * 1 0 0 x
 * 0 1 0 y
 * 0 0 1 z
 * 0 0 0 1
 */
void Matrix::translate(const Vector &offset) {
    setIdentity();
//    Matrix t;
//    t.m[3][0] = offset.x;
//    t.m[3][1] = offset.y;
//    t.m[3][2] = offset.z;
//    mult(t);
    m[3][0] = offset.x;
    m[3][1] = offset.y;
    m[3][2] = offset.z;
}

/*
 * cos(theta)+(1-cos(theta))xx  (1-cos(theta))xy-sin(theta)z  (1-cos(theta))xz+sin(theta)y  0
 * (1-cos(theta)yx+sin(theta)z  cos(theta) +(1-cos(theta))yy  (1-cos(theta))yz-sin(theta)x  0
 * (1-cos(theta)zx-sin(theta)y  (1-cos(theta))zy+sin(theta)x  cos(theta)+(1-cos(theta))zz   0
 *            0                              0                               0              1
*/
void Matrix::rotate(const Vector &pivot, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    Vector n = pivot;
    n.normalize();
    float x = n.x;
    float y = n.y;
    float z = n.z;
    setIdentity();
    m[0][0] = c + (1 - c) * x * x;
    m[0][1] = (1 - c) * x * y - s * z;
    m[0][2] = (1 - c) * x * z + s * y;
    m[1][0] = (1 - c) * y * x + s * z;
    m[1][1] = c + (1 - c) * y * y;
    m[1][2] = (1 - c) * y * z - s * x;
    m[2][0] = (1 - c) * z * x - s * y;
    m[2][1] = (1 - c) * z * y + s * x;

    m[2][2] = c + (1 - c) * z * z;
}

/*
 *  u.x  u.y  u.z  -u.dot(eye)
 *  v.x  v.y  v.z  -v.dot(eye)
 *  n.x  n.y  n.z  -n.dot(eye)
 *   0    0    0        1
 */
void Matrix::lookAt(const Vector &eye, const Vector &target, const Vector &up) {
    setZero();
    Vector n = target;
    n.sub(eye);
    n.normalize();
    Vector v = up;
    Vector u = v.cross(n);
    u.normalize();
    v = n.cross(u);

    m[0][0] = u.x;
    m[1][0] = u.y;
    m[2][0] = u.z;
    m[3][0] = -u.dot(eye);

    m[0][1] = v.x;
    m[1][1] = v.y;
    m[2][1] = v.z;
    m[3][1] = -v.dot(eye);

    m[0][2] = n.x;
    m[1][2] = n.y;
    m[2][2] = n.z;
    m[3][2] = -n.dot(eye);

    m[0][3] = m[1][3] = m[2][3] = 0.0f;
    m[3][3] = 1.0f;
}

/*
 * 1/(asp * tan(fovy/2))       0            0         0
 *          0             1/tan(fovy/2)     0         0
 *          0                  0          n+f/n-f  2nf/n-f
 *          0                  0            1        0
 */
void Matrix::perspective(float fovy, float aspect, float near, float far) {
    setZero();
    float t = 1.0f / tan(fovy / 2);
    m[0][0] = t / aspect;
    m[1][1] = t;
    m[2][2] = (near + far) / (far - near);
    m[3][2] = 2 * near * far / (near - far);
    m[2][3] = 1.0f;
}
