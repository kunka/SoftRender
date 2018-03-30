//
// Created by huangkun on 27/03/2018.
//

#ifndef GL_SHADERCACHE_H
#define GL_SHADERCACHE_H


#include "Shader.h"
#include <unordered_map>

// Default Shaders
static const char *shader_position_color_size = "position_color_size";
static const char *shader_position_color = "position_color";

class ShaderCache {

public:
    ShaderCache();

    ~ShaderCache();

    static ShaderCache *getInstance();

    static void destroyInstance();

    Shader *getShader(const std::string &key);

    void loadDefaultShaders();

private:
    bool init();

    void loadDefaultShader(Shader *shader, int type);

    std::unordered_map<std::string, Shader *> _shaders;
};

#endif //GL_SHADERCACHE_H
