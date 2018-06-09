//
// Created by huangkun on 2018/5/9.
//

#include "Quaternion.h"

TEST_NODE_IMP_BEGIN

    Quaternion::Quaternion() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
}
)";
        const char *frag = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}
)";
        shader.loadStr(vert, frag);

        int piece = 50;
        float degree = 2 * M_PI / piece;
        for (int i = 0; i < piece; i++)
            for (int j = 0; j < piece; j++) {
                vec3 p1, p2, p3, p4;
                p1 = vec3(cos(degree * j) * cos(degree * i), sin(degree * j), cos(degree * j) * sin(degree * i));
                p2 = vec3(cos(degree * j) * cos(degree * (i + 1)), sin(degree * j),
                          cos(degree * j) * sin(degree * (i + 1)));
                p3 = vec3(cos(degree * (j + 1)) * cos(degree * (i + 1)), sin(degree * (j + 1)),
                          cos(degree * (j + 1)) * sin(degree * (i + 1)));
                p4 = vec3(cos(degree * (j + 1)) * cos(degree * i), sin(degree * (j + 1)),
                          cos(degree * (j + 1)) * sin(degree * i));
                vertices.push_back(p1.x);
                vertices.push_back(p1.y);
                vertices.push_back(p1.z);
                vertices.push_back(p2.x);
                vertices.push_back(p2.y);
                vertices.push_back(p2.z);
                vertices.push_back(p3.x);
                vertices.push_back(p3.y);
                vertices.push_back(p3.z);
                vertices.push_back(p1.x);
                vertices.push_back(p1.y);
                vertices.push_back(p1.z);
                vertices.push_back(p3.x);
                vertices.push_back(p3.y);
                vertices.push_back(p3.z);
                vertices.push_back(p4.x);
                vertices.push_back(p4.y);
                vertices.push_back(p4.z);
            }

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);


//        cameraPos = vec3(3.0f, 3.0f, 3.0f);
//        cameraDir = vec3(0.0f) - cameraPos;
//
//        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
//        glm::quat quat = glm::quat_cast(view);
//        vec3 angles = eulerAngles(quat);
//        log("%f,%f,%f", degrees(angles.x), degrees(angles.y), degrees(angles.z));
////        pitch = -degrees(angles.x);

        shader.use();

//        elapsed = 0;
//        cameraPos = vec3(2.0f, 2.0f, 2.0f);
//        linear(yaw, yaw, -120, 5);
//        linear(pitch, pitch, -20, 5);

        // p1
        float x = 45;
        float y = 50;
        pA = vec3(cos(radians(y)) * cos(radians(x)), sin(radians(y)),
                  cos(radians(y)) * sin(radians(x)));
        pA *= 2;

        // p2
        x = 45;
        y = -50;
        pB = vec3(cos(radians(y)) * cos(radians(x)), sin(radians(y)),
                  cos(radians(y)) * sin(radians(x)));
        pB *= 2;

        p1 = pA;
        p2 = pA;
        p3 = pA;
        p4 = pA;
        lerp(p1, pA, pB, 5);
        slerp(p2, pA, pB, 5);
        slerpQ(p3, pA, pB, 5);
        slerpT(p4, pA, pB, 5);
    }

    void Quaternion::lerp(vec3 &var, vec3 from, vec3 to, float dur) {
        tickers.push_back([=, &var](float delta) mutable {
            elapsed1 += delta;
            if (elapsed1 > 0) {
                if (elapsed1 < dur) {
                    var = from + (to - from) * elapsed1 / dur;
                } else {
                    var = to;
                    elapsed1 = INT_MIN;
                }
            } else {
                elapsed1 = 0;
                var = from;
            }
        });
    }

    void Quaternion::slerp(vec3 &var, vec3 from, vec3 to, float dur) {
        tickers.push_back([=, &var](float delta) mutable {
            elapsed2 += delta;
            if (elapsed2 > 0) {
                if (elapsed2 < dur) {
                    var = from + (to - from) * elapsed2 / dur;
                    var = var * length(from) / length(var);
                } else {
                    var = to;
                    elapsed2 = INT_MIN;
                }
            } else {
                elapsed2 = 0;
                var = from;
            }
        });
    }

    void Quaternion::slerpQ(vec3 &var, vec3 from, vec3 to, float dur) {
        tickers.push_back([=, &var](float delta) mutable {
            elapsed3 += delta;
            if (elapsed3 > 0) {
                if (elapsed3 < dur) {
                    // Slerp(p,q,t)=(sin[(1−t)θ] * p + sintθ * q) / sinθ

                    auto p = glm::normalize(from);
                    auto q = glm::normalize(to);
                    float dot = glm::dot(p, q);
//                    if (abs(dot) >= 1.0f) {
//                        var = from;
//                        return;
//                    }
//                    if (dot < 0) {
//                        q *= -1;
//                        dot *= -1;
//                    }
                    float theta = acos(dot);
                    float s = sin(theta);
//                    if (abs(s) < 0.00001f) {
//                        var = from;
//                        return;
//                    }
                    float t = elapsed3 / dur;
                    p *= sin((1.0 - t) * theta);
                    q *= sin(t * theta);
                    vec3 v = p + q;
                    v /= s;
                    v *= glm::length(from);
                    var = v;
                } else {
                    var = to;
                    elapsed3 = INT_MIN;
                }
            } else {
                elapsed3 = 0;
                var = from;
            }
        });
    }

// Rotate Matrix
// [ cos(theta)+(1-cos(theta))xx  (1-cos(theta))xy-sin(theta)z  (1-cos(theta))xz+sin(theta)y ]
// | (1-cos(theta)yx+sin(theta)z  cos(theta) +(1-cos(theta))yy  (1-cos(theta))yz-sin(theta)x |
// [ (1-cos(theta)zx-sin(theta)y  (1-cos(theta))zy+sin(theta)x  cos(theta)+(1-cos(theta))zz  ]
    void Quaternion::slerpT(vec3 &var, vec3 from, vec3 to, float dur) {
        tickers.push_back([=, &var](float delta) mutable {
            elapsed4 += delta;
            if (elapsed4 > 0) {
                if (elapsed4 < dur) {
                    // Slerp(p,q,t)=(sin[(1−t)θ] * p + sintθ * q) / sinθ
                    auto p = glm::normalize(from);
                    auto q = glm::normalize(to);
                    float dot = glm::dot(p, q);
//                    if (abs(dot) >= 1.0f) {
//                        var = from;
//                        return;
//                    }
//                    if (dot < 0) {
//                        q *= -1;
//                        dot *= -1;
//                    }
                    float theta = acos(dot);
                    theta *= elapsed4 / dur;
                    float c = cos(theta);
                    float s = sin(theta);
                    vec3 n = glm::cross(from, to);
                    n = glm::normalize(n);
                    float x = n.x;
                    float y = n.y;
                    float z = n.z;
                    float m11 = c + (1 - c) * x * x;
                    float m12 = (1 - c) * x * y - s * z;
                    float m13 = (1 - c) * x * z + s * y;
                    float m21 = (1 - c) * y * x + s * z;
                    float m22 = c + (1 - c) * y * y;
                    float m23 = (1 - c) * y * z - s * x;
                    float m31 = (1 - c) * z * x - s * y;
                    float m32 = (1 - c) * z * y + s * x;
                    float m33 = c + (1 - c) * z * z;
                    mat3 rotate(m11, m12, m13, m21, m22, m23, m31, m32, m33);
                    var = from * rotate;
                } else {
                    var = to;
                    elapsed4 = INT_MIN;
                }
            } else {
                elapsed4 = 0;
                var = from;
            }
        });
    }

    void Quaternion::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vec3 offset = vec3(0, 0, 0);
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        model = glm::mat4();
        model = glm::translate(model, offset);
        model = glm::scale(model, vec3(2));
        shader.setVec3("color", vec3(1.0, 1.0, 1.0));
        shader.setMat4("model", model);

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframemode
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

        // pA
        model = glm::mat4();
        model = glm::translate(model, offset);
        model = glm::translate(model, pA);
        model = glm::scale(model, vec3(0.03));
        shader.setMat4("model", model);
        shader.setVec3("color", vec3(1.0, 1.0, 0.5));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

        // pB
        model = glm::mat4();
        model = glm::translate(model, pB);
        model = glm::translate(model, offset);
        model = glm::scale(model, vec3(0.03));
        shader.setMat4("model", model);
        shader.setVec3("color", vec3(0.5, 1.0, 1.0));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

        // p1
        model = glm::mat4();
        model = glm::translate(model, p1);
        model = glm::translate(model, offset);
        model = glm::scale(model, vec3(0.03));
        shader.setMat4("model", model);
        shader.setVec3("color", vec3(1.0, 1.0, 0));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

        // p2
        model = glm::mat4();
        model = glm::translate(model, p2);
        model = glm::translate(model, offset);
        model = glm::scale(model, vec3(0.03));
        shader.setMat4("model", model);
        shader.setVec3("color", vec3(1.0, 1.0, 0));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

        // p3
        model = glm::mat4();
        model = glm::translate(model, p3);
        model = glm::translate(model, offset);
        model = glm::scale(model, vec3(0.04));
        shader.setMat4("model", model);
        shader.setVec3("color", vec3(1.0, 0.0, 0));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

        // p4
        model = glm::mat4();
        model = glm::translate(model, p4);
        model = glm::translate(model, offset);
        model = glm::scale(model, vec3(0.03));
        shader.setMat4("model", model);
        shader.setVec3("color", vec3(0.0, 1.0, 0));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    }

    void Quaternion::fixedUpdate(float delta) {
        CustomDraw::fixedUpdate(delta);

        for (auto function  : tickers) {
            function(delta);
        }
    }

    Quaternion::~Quaternion() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END