//
// Created by huangkun on 27/03/2018.
//

#include "ShaderCache.h"
#include "shaders.h"

static ShaderCache *s_shaderCache = nullptr;

ShaderCache *ShaderCache::getInstance() {
    if (!s_shaderCache) {
        s_shaderCache = new(std::nothrow) ShaderCache();
        if (!s_shaderCache->init()) {
            delete s_shaderCache;
            s_shaderCache = nullptr;
        }
    }
    return s_shaderCache;
}

void ShaderCache::destroyInstance() {
    if (s_shaderCache) {
        delete s_shaderCache;
        s_shaderCache = nullptr;
    }
}

ShaderCache::ShaderCache() {

}

ShaderCache::~ShaderCache() {
    for (auto &s : _shaders) {
        delete s.second;
    }
}

bool ShaderCache::init() {
    loadDefaultShaders();
    return true;
}

void ShaderCache::loadDefaultShaders() {
    Shader *shader = new Shader();
    shader->loadStr(shader_position_color_size_vert, shader_position_color_size_frag);
    _shaders.emplace(shader_position_color_size, shader);
}

void ShaderCache::loadDefaultShader(Shader *shader, int type) {

}

Shader *ShaderCache::getShader(const std::string &key) {
    auto it = _shaders.find(key);
    return it != _shaders.end() ? it->second : nullptr;
}
