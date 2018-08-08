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
}

void Texture2D::load(const std::string &path, int desiredChannels) {
    data = stbi_load(path.c_str(), &width, &height, &channels, desiredChannels);
}

glm::vec4 Texture2D::sample(float u, float v) {
    int x = MathUtil::clamp(width * u, 0, width - 1);
    int y = MathUtil::clamp(height * v, 0, height - 1);
    unsigned char *pixel = data + (height * y + x) * 3;
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

Texture2D::~Texture2D() {
    stbi_image_free(data);
}