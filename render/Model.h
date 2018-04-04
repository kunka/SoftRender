//
// Created by huangkun on 04/04/2018.
//

#ifndef GL_MODEL_H
#define GL_MODEL_H

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {

public:
    Model(const std::string &path) {
        loadModel(path);
    }

    void draw(Shader &shader);

private:

    void loadModel(const std::string &path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              const std::string &typeName);

    std::vector<Mesh> _meshes;
    std::string _directory;

    std::unordered_map<std::string, Texture> _texturesLoaded;
};

#endif //GL_MODEL_H
