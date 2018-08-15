//
// Created by huangkun on 2018/6/17.
//

#include "MathUtil.h"
#include <math.h>
#include <string.h>

float MathUtil::clamp(float x, float min, float max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

Vector::Vector() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

Vector::Vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
}

Vector::Vector(const glm::vec3 &v) : x(v.x), y(v.y), z(v.z) {
}

Vector::Vector(const glm::vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {
}

glm::vec2 Vector::vec2() {
    return glm::vec2(x, y);
}

glm::vec3 Vector::vec3() {
    return glm::vec3(x, y, z);
}

glm::vec4 Vector::vec4() {
    return glm::vec4(x, y, z, w);
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

/*
 *          | 00 10 20 30 |   | 00 10 20 30 |
 *          | 01 11 21 31 |   | 01 11 21 31 |
 *  other * | 02 12 22 32 | * | 02 12 22 32 |
 *          | 03 13 23 33 |   | 03 13 23 33 |
 */
void Matrix::mult(const Matrix &other) {
    Matrix ret;
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
//            ret.m[i][j] = m[0][i] * other.m[j][0] +
//                          m[1][i] * other.m[j][1] +
//                          m[2][i] * other.m[j][2] +
//                          m[3][i] * other.m[j][3];
            ret.m[i][j] = other.m[0][j] * m[i][0] +
                          other.m[1][j] * m[i][1] +
                          other.m[2][j] * m[i][2] +
                          other.m[3][j] * m[i][3];
    memcpy(m, ret.m, 16 * sizeof(float));
}
//
//void Matrix::transpose() {
//}
//void Matrix::inverse() {
//}
//
/*
 *  | 00 10 20 30 |   | x |
 *  | 01 11 21 31 |   | y |
 *  | 02 12 22 32 | * | z |
 *  | 03 13 23 33 |   | 1 |
 */
Vector Matrix::applyPoint(const Vector &v) const {
    Vector ret;
    ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
    ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
    ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
    ret.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3];
    return ret;
}

/*
 *  | 00 10 20 30 |   | x |
 *  | 01 11 21 31 |   | y |
 *  | 02 12 22 32 | * | z |
 *  | 03 13 23 33 |   | 0 |
 */
Vector Matrix::applyVector(const Vector &v) const {
    Vector ret;
    ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
    ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
    ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
    ret.w = 0;
    return ret;
}

/*
 *  | 00 10 20 30 |   | s 0 0 0 |
 *  | 01 11 21 31 |   | 0 s 0 0 |
 *  | 02 12 22 32 | * | 0 0 s 0 |
 *  | 03 13 23 33 |   | 0 0 0 1 |
 */
void Matrix::scale(float scale) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            m[i][j] *= scale;
}

/*
 *  | 00 10 20 30 |   | x 0 0 0 |
 *  | 01 11 21 31 |   | 0 y 0 0 |
 *  | 02 12 22 32 | * | 0 0 z 0 |
 *  | 03 13 23 33 |   | 0 0 0 1 |
 */
void Matrix::scale(const Vector &scale) {
    for (int i = 0; i < 3; i++) {
        m[0][i] *= scale.x;
        m[1][i] *= scale.y;
        m[2][i] *= scale.z;
    }
}

/*
 *  | 00 10 20 30 |   |1 0 0 x|  00 10 20 00x+10y+20z+30
 *  | 01 11 21 31 |   |0 1 0 y|  01 11 21 01x+11y+21z+31
 *  | 02 12 22 32 | * |0 0 1 z|  02 12 22 02x+12y+22z+32
 *  | 03 13 23 33 |   |0 0 0 1|  03 13 23 03x+13y+23z+33
 */
void Matrix::translate(const Vector &offset) {
    m[3][0] += m[0][0] * offset.x + m[1][0] * offset.y + m[2][0] * offset.z;
    m[3][1] += m[0][1] * offset.x + m[1][1] * offset.y + m[2][1] * offset.z;
    m[3][2] += m[0][2] * offset.x + m[1][2] * offset.y + m[2][2] * offset.z;
    m[3][3] += m[0][3] * offset.x + m[1][3] * offset.y + m[2][3] * offset.z;
}

/*
 * | 00 10 20 30 |   | cos(theta)+(1-cos(theta))xx  (1-cos(theta))xy-sin(theta)z  (1-cos(theta))xz+sin(theta)y  0
 * | 01 11 21 31 |   | (1-cos(theta)yx+sin(theta)z  cos(theta) +(1-cos(theta))yy  (1-cos(theta))yz-sin(theta)x  0
 * | 02 12 22 32 | * | (1-cos(theta)zx-sin(theta)y  (1-cos(theta))zy+sin(theta)x  cos(theta)+(1-cos(theta))zz   0
 * | 03 13 23 33 |   |           0                              0                               0              1
*/
void Matrix::rotate(const Vector &pivot, float theta) {
    Matrix ret;
    float c = cos(theta);
    float s = sin(theta);
    Vector n = pivot;
    n.normalize();
    float x = n.x;
    float y = n.y;
    float z = n.z;
    ret.m[0][0] = c + (1 - c) * x * x;
    ret.m[1][0] = (1 - c) * x * y - s * z;
    ret.m[2][0] = (1 - c) * x * z + s * y;
    ret.m[0][1] = (1 - c) * y * x + s * z;
    ret.m[1][1] = c + (1 - c) * y * y;
    ret.m[2][1] = (1 - c) * y * z - s * x;
    ret.m[0][2] = (1 - c) * z * x - s * y;
    ret.m[1][2] = (1 - c) * z * y + s * x;
    ret.m[2][2] = c + (1 - c) * z * z;
    ret.mult(*this);
    memcpy(m, ret.m, 16 * sizeof(float));
}

/*
 * | right.x right.y  right.z  0|   |1 0 0 -eye.x|
 * | up.x    up.y     up.z     0| * |0 1 0 -eye.y|
 * |-front.x -front.y -front.z 0|   |0 0 1 -eye.z|
 * | 0       0        0        1|   |0 0 0    1  |
 *
 *  right.x   right.y   right.z   -right.dot(eye)
 *  up.x      up.y      up.z      -up.dot(eye)
 *  -front.x  -front.y  -front.z nfront.dot(eye)
 *   0        0         0         1
 */
Matrix Matrix::lookAt(const Vector &eye, const Vector &target, const Vector &up_) {
    Matrix matrix;
    Vector front = target;
    front.sub(eye);
    front.normalize();
    Vector up = up_;
    Vector right = front.cross(up);
    right.normalize();
    up = right.cross(front);

    auto &m = matrix.m;
    m[0][0] = right.x;
    m[1][0] = right.y;
    m[2][0] = right.z;
    m[3][0] = -right.dot(eye);

    m[0][1] = up.x;
    m[1][1] = up.y;
    m[2][1] = up.z;
    m[3][1] = -up.dot(eye);

    m[0][2] = -front.x;
    m[1][2] = -front.y;
    m[2][2] = -front.z;
    m[3][2] = front.dot(eye);
    return matrix;
}

/*
 * 1/(asp * tan(fovy/2))       0            0         0
 *          0             1/tan(fovy/2)     0         0
 *          0                  0          n+f/n-f  2nf/n-f
 *          0                  0            -1        0
 */
Matrix Matrix::perspective(float fovy, float aspect, float near, float far) {
    Matrix matrix;
    matrix.setZero();
    auto &m = matrix.m;
    float t = 1.0f / tan(fovy / 2);
    m[0][0] = t / aspect;
    m[1][1] = t;
    m[2][2] = -(near + far) / (far - near);
    m[3][2] = 2 * near * far / (near - far);
    m[2][3] = -1.0f;
    return matrix;
}
