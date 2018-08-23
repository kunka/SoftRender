//
// Created by huangkun on 2018/8/8.
//

#include "Texture2D.h"
#include "stb_image.h"
#include "MathUtil.h"
#include <glad/glad.h>
#include "Log.h"

using namespace glm;

Texture2D::Texture2D() {
    width = height = 0;
    data = nullptr;
    channels = 1;
    wrap = GL_REPEAT;
    magFilter = GL_NEAREST;
    minFilter = GL_NEAREST;
}

void Texture2D::load(const std::string &path, int desiredChannels) {
    data = stbi_load(path.c_str(), &width, &height, &channels, desiredChannels);
    this->path = path;
}

glm::vec4 Texture2D::_sample(int x, int y, int lv) {
    unsigned char *src;
    int w, h;
    if (mipmaps.find(lv) != mipmaps.end()) {
        src = mipmaps.at(lv);
        w = width / (int) pow(2, lv);
        h = height / (int) pow(2, lv);
    } else {
        src = data;
        lv = 0;
        w = width;
        h = height;
    }
//    if (x > w - 1 || x < 0 || y > h - 1 || y < 0) {
//        log("%d, %d", x, y);
//    }
    x = MathUtil::clamp(x, 0, w - 1);
    y = MathUtil::clamp(y, 0, h - 1);
    unsigned char *pixel = src + (h * y + x) * channels;
    if (channels == 1) {
        unsigned int r = (unsigned int) pixel[0];
        return glm::vec4(r, 0, 0, 255);
    } else if (channels == 3) {
        unsigned int r = (unsigned int) pixel[0];
        unsigned int g = (unsigned int) pixel[1];
        unsigned int b = (unsigned int) pixel[2];
        return glm::vec4(r, g, b, 255);
    } else if (channels == 4) {
        unsigned int r = (unsigned int) pixel[0];
        unsigned int g = (unsigned int) pixel[1];
        unsigned int b = (unsigned int) pixel[2];
        unsigned int a = (unsigned int) pixel[3];
        return glm::vec4(r, g, b, a);
    }
}

// only support pow of 2 texture
void Texture2D::genMipmaps(int maxLv) {
    if (mipmaps.size() == 0) {
        int lv = 1;
        for (int i = width / 2; i > 1 || lv > maxLv; i /= 2) {
            unsigned char *texData = new unsigned char[i * i * channels];
            for (int y = 0; y < i; y++) {
                for (int x = 0; x < i; x++) {
                    vec4 p1 = _sample(x * 2, y * 2, lv - 1);
                    vec4 p2 = _sample(x * 2 + 1, y * 2, lv - 1);
                    vec4 p3 = _sample(x * 2, y * 2 + 1, lv - 1);
                    vec4 p4 = _sample(x * 2 + 1, y * 2 + 1, lv - 1);
                    vec4 p = (p1 + p2 + p3 + p4) / 4.0f;
                    texData[channels * (y * i + x)] = (GLubyte) p.r;
                    texData[channels * (y * i + x) + 1] = (GLubyte) p.g;
                    texData[channels * (y * i + x) + 2] = (GLubyte) p.b;
                    if (channels == 4)
                        texData[channels * (y * i + x) + 3] = (GLubyte) p.a;
                }
            }
            mipmaps.insert(std::make_pair(lv, texData));
            lv++;
        }
    }
}

//float
//mip_map_level(in vec2 texture_coordinate)
//{
//    // The OpenGL Graphics System: A Specification 4.2
//    //  - chapter 3.9.11, equation 3.21
//
//
//    vec2  dx_vtc        = dFdx(texture_coordinate);
//    vec2  dy_vtc        = dFdy(texture_coordinate);
//    float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
//
//
//    //return max(0.0, 0.5 * log2(delta_max_sqr) - 1.0); // == log2(sqrt(delta_max_sqr));
//    return 0.5 * log2(delta_max_sqr); // == log2(sqrt(delta_max_sqr));
//}

float mip_map_level(float dudx, float dvdy) {
    float delta_max_sqr = max(dudx * dudx, dvdy * dvdy);
    return delta_max_sqr == 0 ? 1 : 0.5 * log2(delta_max_sqr);
}

glm::vec4 Texture2D::sample(float u, float v, float dudx, float dvdy) {
    // Wrap
    if (wrap == GL_REPEAT) {
        u = u - floor(u);
        v = v - floor(v);
    } else if (wrap == GL_MIRRORED_REPEAT) {
        int ui = floor(u);
        int vi = floor(v);
        if ((ui > 0 ? ui : -ui) % 2 == 1) {
            u = 1.0f - (u - ui);
        }
        if ((vi > 0 ? vi : -vi) % 2 == 1) {
            v = 1.0f - (v - vi);
        }
    } else if (wrap == GL_CLAMP_TO_EDGE) {
        u = MathUtil::clamp(u, 0, 1);
        v = MathUtil::clamp(v, 0, 1);
    } else if (wrap == GL_CLAMP_TO_BORDER) {
        if (u > 1.0 || u < 0 || v > 1.0 || v < 0)
            return borderColor;
    }

    dudx *= width;
    dvdy *= height;
    if (dudx < 1 && dvdy < 1) {
        // mag filter
        // filter
        if (magFilter == GL_NEAREST) {
            return _sample(width * u, height * v);
        } else if (magFilter == GL_LINEAR) {
            /*
             * 01 11
             * 00 10
             */
            float x = width * u - 0.5f; // fix offset
            float y = height * v - 0.5f;
            int x0 = floor(x);
            int y0 = floor(y);
            int x1 = x0 + 1;
            int y1 = y0 + 1;
            float wx = x - x0;
            float wy = y - y0;
            if (x0 < 0) {
                wx = 1;
            }
            if (x0 > width - 1) {
                wx = 0;
            }
            if (y < 0) {
                wy = 1;
            }
            if (y > height - 1) {
                wy = 0;
            }
            glm::vec4 xx = _sample(x0, y0) * (1.0f - wx) + _sample(x1, y0) * wx;
            glm::vec4 yy = _sample(x0, y1) * (1.0f - wx) + _sample(x1, y1) * wx;
            return xx * (1.0f - wy) + yy * wy;
        }
    } else {
        if (mipmaps.size() == 0) {
            // filter
            if (minFilter == GL_NEAREST) {
                return _sample(width * u, height * v);
            } else if (minFilter == GL_LINEAR) {
                /*
                 * 01 11
                 * 00 10
                 */
                float x = width * u - 0.5f; // fix offset
                float y = height * v - 0.5f;
                int x0 = floor(x);
                int y0 = floor(y);
                int x1 = x0 + 1;
                int y1 = y0 + 1;
                float wx = x - x0;
                float wy = y - y0;
                if (x0 < 0) {
                    wx = 1;
                }
                if (x0 > width - 1) {
                    wx = 0;
                }
                if (y < 0) {
                    wy = 1;
                }
                if (y > height - 1) {
                    wy = 0;
                }
                glm::vec4 xx = _sample(x0, y0) * (1.0f - wx) + _sample(x1, y0) * wx;
                glm::vec4 yy = _sample(x0, y1) * (1.0f - wx) + _sample(x1, y1) * wx;
                return xx * (1.0f - wy) + yy * wy;
            }
        } else {
            float level = mip_map_level(dudx, dvdy);
            int lv = int(level);
            if (lv == 0) {

            }
            if (minFilter == GL_NEAREST_MIPMAP_NEAREST) {
                // calcu lod
                if (lv == 0) {
                    return _sample(width * u, height * v);
                } else if (mipmaps.find(lv) != mipmaps.end()) {
                    int w = width / (int) pow(2, lv);
                    int h = height / (int) pow(2, lv);
                    return _sample(w * u, h * v, lv);
                }
            }
        }
    }
}

Texture2D::~Texture2D() {
    if (data != nullptr) {
        stbi_image_free(data);
    }
    for (auto iter = mipmaps.begin(); iter != mipmaps.end(); ++iter)
        delete iter->second;
}
