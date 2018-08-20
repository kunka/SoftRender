//
// Created by huangkun on 2018/8/8.
//

#ifndef GL_TEXTURE2D_H
#define GL_TEXTURE2D_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Texture2D {
public:
    Texture2D();

    ~Texture2D();

    void load(const std::string &path, int desiredChannels = 0);

    std::string getPath() { return path; }

    int getWidth() { return width; }

    int getHeight() { return height; }

    int getChannels() { return channels; }

    const unsigned char *getData() const { return data; }

    glm::vec4 sample(float u, float v, float dudx = 0, float dvdy = 0);

//    void genMipmaps();
//
    void setWrap(int wrap) { this->wrap = wrap; }

    void setBorderColor(const glm::vec4 &color) { borderColor = color; }

    void setMagFilter(int filter) { magFilter = filter; }

    void setMinFilter(int filter) { minFilter = filter; }

    void genMipmaps(int maxLv = 10);

    std::string type;

private:
    glm::vec4 _sample(int x, int y, int lv = 0);

    unsigned char *data;
    int channels;
    int width;
    int height;
    int minFilter;
    int magFilter;
    int wrap;
    std::string path;
    glm::vec4 borderColor;
    std::unordered_map<int, unsigned char *> mipmaps;
};

#endif //GL_TEXTURE2D_H
