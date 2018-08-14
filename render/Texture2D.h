//
// Created by huangkun on 2018/8/8.
//

#ifndef GL_TEXTURE2D_H
#define GL_TEXTURE2D_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

typedef enum {
    TF_NEAREST = 0,
    TF_LINEAR,
} TextureFilter;

class Texture2D {
public:
    Texture2D();

    ~Texture2D();

    void load(const std::string &path, int desiredChannels = 0);

    int getWidth() { return width; }

    int getHeight() { return height; }

    int getChannels() { return channels; }

    const unsigned char *getData() const { return data; }

    glm::vec4 sample(float u, float v);

//    void genMipmaps();
//
    void setFilter(TextureFilter filter);

private:
    glm::vec4 _sample(int x, int y);

    unsigned char *data;
    int channels;
    int width;
    int height;
    TextureFilter filter;
    std::unordered_map<int, unsigned char *> mipmaps;
};

#endif //GL_TEXTURE2D_H
