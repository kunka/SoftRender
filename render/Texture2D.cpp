//
// Created by huangkun on 2018/8/8.
//

#include "Texture2D.h"
#include "stb_image.h"
#include "MathUtil.h"

Texture2D::Texture2D() {
    width = height = 0;
    data = nullptr;
    channels = 1;
    this->filter = TF_NEAREST;
}

void Texture2D::setFilter(TextureFilter filter) {
    this->filter = filter;
}

void Texture2D::load(const std::string &path, int desiredChannels) {
    data = stbi_load(path.c_str(), &width, &height, &channels, desiredChannels);
    this->path = path;
}

glm::vec4 Texture2D::_sample(int x, int y) {
    x = MathUtil::clamp(x, 0, width - 1);
    y = MathUtil::clamp(y, 0, height - 1);
    unsigned char *pixel = data + (height * y + x) * channels;
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

glm::vec4 Texture2D::sample(float u, float v) {
    // clamp to 0~1
    u = u - floor(u);
    v = v - floor(v);
    if (this->filter == TF_NEAREST) {
        return _sample(width * u, height * v);
    } else if (this->filter == TF_LINEAR) {
        /*
         * 01 11
         * 00 10
         */
        float x = MathUtil::clamp(width * u, 0, width - 1);
        float y = MathUtil::clamp(height * v, 0, height - 1);
        int x0 = floor(x);
        int y0 = floor(y);
        int x1 = MathUtil::clamp(x0 + 1, 0, width - 1);
        int y1 = MathUtil::clamp(y0 + 1, 0, height - 1);
        float wx = x - x0;
        float wy = y - y0;
        glm::vec4 xx = _sample(x0, y0) * (1.0f - wx) + _sample(x1, y0) * wx;
        glm::vec4 yy = _sample(x0, y1) * (1.0f - wx) + _sample(x1, y1) * wx;
        return xx * (1.0f - wy) + yy * wy;
    }
}

Texture2D::~Texture2D() {
    if (data != nullptr) {
        stbi_image_free(data);
    }
}