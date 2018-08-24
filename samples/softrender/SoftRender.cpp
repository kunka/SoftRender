//
// Created by huangkun on 2018/6/16.
//

#include "SoftRender.h"

TEST_NODE_IMP_BEGIN

    SoftRender::SoftRender() {
        TEX_WIDTH = 512;
        TEX_HEIGHT = 512;
    }

    bool SoftRender::init() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(a_position, 1.0);
    texCoord = a_texCoord;
}
)";

        const char *frag = R"(
#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, texCoord);;
}
)";
        shader.loadStr(vert, frag);
        texData = new GLubyte[TEX_WIDTH * TEX_HEIGHT * 4];

        float ratio = (float) TEX_WIDTH / TEX_HEIGHT;
        float vertices[] = {
                // postions       // texture coords
                ratio, ratio, 0.0f, 1.0f, 1.0f, // top right
                ratio, -ratio, 0.0f, 1.0f, 0.0f, // bottom right
                -ratio, -ratio, 0.0f, 0.0f, 0.0f, // bottom left
                -ratio, ratio, 0.0f, 0.0f, 1.0f, // bottom top
        };
        unsigned int indices[] = { // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3 // second triangle
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        shader.use();
        shader.setInt("ourTexture", 0);

        clipRect.setRect(0, 0, TEX_WIDTH, TEX_HEIGHT);
        isClipRect = true;
        depthBuff = new float[MAX_TEX_SIZE * MAX_TEX_SIZE];
        _depthTest = false;
        faceCulling = false;
        faceCullingMode = GL_BACK;
        _blend = false;
        projectMatrix = Matrix::perspective(radians(60.0f), (float) TEX_WIDTH / TEX_HEIGHT, 0.1, 100.0f);
        return true;
    }

    void SoftRender::draw(const mat4 &transform) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        texture = genTexture();

        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    SoftRender::~SoftRender() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        delete[] depthBuff;
        if (texData) {
            delete[] texData;
        }
    }

    unsigned int SoftRender::genTexture() {
        unsigned int textureID = texture;
        if (textureID == -1) {
            glGenTextures(1, &textureID);
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (textureID == 0) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        return textureID;
    }

    bool SoftRender::depthTest(int x, int y, float depth) {
        if (x >= 0 && y >= 0 && x < TEX_WIDTH && y < TEX_HEIGHT) {
            int index = y * TEX_WIDTH + x;
            if (!_depthTest or depth < depthBuff[index]) {
                return true;
            }
        }
        return false;
    }

    void SoftRender::setPixel(int x, int y, float depth, const vec4 &color) {
        if (x >= 0 && y >= 0 && x < TEX_WIDTH && y < TEX_HEIGHT) {
            int index = y * TEX_WIDTH + x;
            if (!_depthTest or depth < depthBuff[index]) {
                float r, g, b, a;
                depthBuff[index] = depth;
                if (_blend) {
                    // C_result =C_source∗F_source+C_destination∗F_destination
                    float F_source = 1;
                    float F_destination = 0;
                    if (_blendFuncSrc == GL_SRC_ALPHA) {
                        F_source = color.a / 255.0f;
                    }
                    if (_blendFuncDst == GL_ONE_MINUS_SRC_ALPHA) {
                        F_destination = 1.0f - color.a / 255.0f;
                    }
//                    r = F_source * color.r + F_destination * (float) texData[y][x][0];
//                    g = F_source * color.g + F_destination * (float) texData[y][x][1];
//                    b = F_source * color.b + F_destination * (float) texData[y][x][2];
//                    a = F_source * color.a + F_destination * (float) texData[y][x][3];
                    r = F_source * color.r + F_destination * (float) texData[4 * (y * TEX_WIDTH + x)];
                    g = F_source * color.g + F_destination * (float) texData[4 * (y * TEX_WIDTH + x) + 1];
                    b = F_source * color.b + F_destination * (float) texData[4 * (y * TEX_WIDTH + x) + 2];
                    a = F_source * color.a + F_destination * (float) texData[4 * (y * TEX_WIDTH + x) + 3];
                } else {
                    r = color.r;
                    g = color.g;
                    b = color.b;
                    a = color.a;
                }
                // overflow
                r = r > 255 ? 255 : (r < 0 ? 0 : r);
                g = g > 255 ? 255 : (g < 0 ? 0 : g);
                b = b > 255 ? 255 : (b < 0 ? 0 : b);
                a = a > 255 ? 255 : (a < 0 ? 0 : a);
//                texData[y][x][0] = (GLubyte) r;
//                texData[y][x][1] = (GLubyte) g;
//                texData[y][x][2] = (GLubyte) b;
//                texData[y][x][3] = (GLubyte) a;
                texData[4 * (y * TEX_WIDTH + x)] = (GLubyte) r;
                texData[4 * (y * TEX_WIDTH + x) + 1] = (GLubyte) g;
                texData[4 * (y * TEX_WIDTH + x) + 2] = (GLubyte) b;
                texData[4 * (y * TEX_WIDTH + x) + 3] = (GLubyte) a;

            }
        }
    }

    void SoftRender::setPixel(int x, int y, float depth, const vec3 &color) {
        setPixel(x, y, depth, vec4(color, 255));
    }

    void SoftRender::setPixel(int x, int y, float z, float u, float v) {
        // override to sample
    }

    void SoftRender::setDepthTest(bool depthTest) {
        this->_depthTest = depthTest;
    }

    void SoftRender::setFaceCull(bool cull) {
        this->faceCulling = cull;
    }

    void SoftRender::setFaceCullMode(int mode) {
        this->faceCullingMode = mode;
    }

    void SoftRender::clearColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
        for (int x = 0; x < TEX_WIDTH; x++)
            for (int y = 0; y < TEX_HEIGHT; y++) {
//                texData[y][x][0] = (GLubyte) r;
//                texData[y][x][1] = (GLubyte) g;
//                texData[y][x][2] = (GLubyte) b;
//                texData[y][x][3] = (GLubyte) a;
                texData[4 * (y * TEX_WIDTH + x)] = (GLubyte) r;
                texData[4 * (y * TEX_WIDTH + x) + 1] = (GLubyte) g;
                texData[4 * (y * TEX_WIDTH + x) + 2] = (GLubyte) b;
                texData[4 * (y * TEX_WIDTH + x) + 3] = (GLubyte) a;
            }
    }

    void SoftRender::clearDepth() {
        for (int i = 0; i < TEX_WIDTH * TEX_HEIGHT; i++) {
            if (_depthFBO) {
                _depthFBO[i] = 1.0f;
            }
            depthBuff[i] = 1.0f;
        }
    }

    // 简单裁剪，完全不可见三角形
    bool SoftRender::cvvCull(vec4 triangle[3]) {
        int code1 = encode(triangle[0]);
        int code2 = encode(triangle[1]);
        int code3 = encode(triangle[2]);
        if (code1 == 0 && code2 == 0 && code3 == 0) {
            return false;
        } else if (code1 & code2 & code3) {
            return true;
        }

        return false;
    }

/*
 *  000000
 *
 *       3   6
 *       |  /
 *       | /
 *       |/
 *  2----/----- 1
 *      /|
 *     / |
 *    5  4
 */
    int SoftRender::encode(const vec4 &p) {
        int code = 0;
        if (p.x > p.w)
            code |= 1;
        else if (p.x < -p.w)
            code |= 2;
        if (p.y > p.w)
            code |= 4;
        else if (p.y < -p.w)
            code |= 8;
        if (p.z > p.w)
            code |= 16;
        else if (p.z < -p.w)
            code |= 32;
        return code;
    }

    int SoftRender::encodeScreen(const vec4 &p) {
        int code = 0;
        if (p.x > TEX_WIDTH)
            code |= 1;
        else if (p.x < 0)
            code |= 2;
        if (p.y > TEX_HEIGHT)
            code |= 4;
        else if (p.y < 0)
            code |= 8;
        if (p.z > 1.0)
            code |= 16;
        else if (p.z < -1.0)
            code |= 32;
        return code;
    }

    bool SoftRender::clip_3D_line(vec4 &p1, vec4 &p2) {
        float dy = p2.y - p1.y;
        float dx = p2.x - p1.x;
        float dz = p2.z - p1.z;
        float m = dx == 0 ? 0 : dy / dx;
        float my = dz == 0 ? 0 : dy / dz;
        float mx = dz == 0 ? 0 : dx / dz;
        while (true) {
            int code1 = encodeScreen(p1);
            int code2 = encodeScreen(p2);
            if (code1 == 0 && code2 == 0) {
                return false;
            } else if (code1 & code2) {
                return true;
            } else {
                if (code1 & 1) {
                    p1.y += m * (TEX_WIDTH - p1.x);
                    p1.x = TEX_WIDTH;
                } else if (code1 & 2) {
                    p1.y += m * (0 - p1.x);
                    p1.x = 0;
                } else if (code1 & 4) {
                    p1.x += (TEX_HEIGHT - p1.y) / m;
                    p1.y = TEX_HEIGHT;
                } else if (code1 & 8) {
                    p1.x += (0 - p1.y) / m;
                    p1.y = 0;
                } else if (code1 & 16) {
                    p1.x += mx * (1 - p1.z);
                    p1.y += my * (1 - p1.z);
                    p1.z = 1;
                } else if (code1 & 32) {
                    p1.x += mx * (0 - p1.z);
                    p1.y += my * (0 - p1.z);
                    p1.z = 0;
                }

                code1 = encodeScreen(p1);
                if (code1 == 0 and code2 == 0) {
                    return false;
                } else if (code1 & code2) {
                    return true;
                } else {
                    if (code2 & 1) {
                        p2.y += m * (TEX_WIDTH - p2.x);
                        p2.x = TEX_WIDTH;
                    } else if (code2 & 2) {
                        p2.y += m * (0 - p2.x);
                        p2.x = 0;
                    } else if (code2 & 4) {
                        p2.x += (TEX_HEIGHT - p2.y) / m;
                        p2.y = TEX_HEIGHT;
                    } else if (code2 & 8) {
                        p2.x += (0 - p2.y) / m;
                        p2.y = 0;
                    } else if (code2 & 16) {
                        p2.x += mx * (1 - p2.z);
                        p2.y += my * (1 - p2.z);
                        p2.z = 1;
                    } else if (code2 & 32) {
                        p2.x += mx * (0 - p2.z);
                        p2.y += my * (0 - p2.z);
                        p2.z = 0;
                    }
                }
            }
        }
    }

    bool SoftRender::faceCull(vec4 triangle[3]) {
        if (faceCulling) {
            vec3 v1 = vec3(triangle[1] - triangle[0]);
            vec3 v2 = vec3(triangle[2] - triangle[0]);
            vec3 normal = glm::cross(v1, v2);
            vec3 v0 = vec3(0, 0, -1);//vec3(triangle[0]) - cameraPos;
            if (faceCullingMode == GL_BACK) {
                return glm::dot(v0, normal) >= 0;
            } else if (faceCullingMode == GL_FRONT) {
                return glm::dot(v0, normal) < 0;
            }
        } else {
            return false;
        }
    }

    void SoftRender::pointToScreen(vec4 *triangle, int num) {
        for (int j = 0; j < num; j++) {
            vec4 &p = triangle[j];
            // （透视除法） --> NDC空间
            if (fabs(p.w) < 0.01) {
                p.w = p.w > 0 ? 0.01 : -0.01;
            }
            p.x /= p.w; // [-1,1]
            p.y /= p.w; // [-1,1]
            p.z /= p.w; // [-1,1]
            p.z = (p.z + 1.0f) / 2; // [0,1]
            //p.w 顶点到相机的距离
            p.w = 1.0f / p.w;
            // NDC空间 --> 窗口坐标（视口变换）
            p.x = (p.x + 1.0f) / 2.0f * TEX_WIDTH;
            p.y = (p.y + 1.0f) / 2.0f * TEX_HEIGHT;
        }
    }

    void SoftRender::drawPoint(const vec3 &worldPos, const vec3 &color) {
        // draw light
        Matrix m;
        m.translate(Vector(worldPos.x, worldPos.y, worldPos.z));
        m.mult(viewMatrix);
        m.mult(projectMatrix);
        Vector v = m.applyPoint(Vector(0, 0, 0));
        vec4 point = vec4(v.x, v.y, v.z, v.w);
        pointToScreen(&point, 1);
        SoftRender::setPixel(point.x - 1, point.y, 0, color);
        SoftRender::setPixel(point.x + 1, point.y, 0, color);
        SoftRender::setPixel(point.x, point.y, 0, color);
        SoftRender::setPixel(point.x, point.y - 1, 0, color);
        SoftRender::setPixel(point.x, point.y + 1, 0, color);
    }


TEST_NODE_IMP_END