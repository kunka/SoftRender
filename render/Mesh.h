//
// Created by huangkun on 04/04/2018.
//

#ifndef GL_MESH_H
#define GL_MESH_H

#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture2D.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
         const std::vector<Texture> &textures);

    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
         const std::vector<Texture2D *> &textures);

    ~Mesh();

    void draw(Shader &shader);

public:
    void setupMesh();

    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;
    std::vector<Texture2D *> _texture2Ds;
};


#endif //GL_MESH_H
