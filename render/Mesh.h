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

static const std::vector<Vertex> createVertexs(const std::vector<float> &data, int position, int normal, int uv) {
    std::vector<Vertex> verts;
    int total = data.size();
    int column = position + normal + uv;
    int row = total / column;
    for (int i = 0; i < row; i++) {
        Vertex vert;
        vert.position.x = data[i * column];
        vert.position.y = data[i * column + 1];
        vert.position.z = data[i * column + 2];
        if (normal > 0) {
            vert.normal.x = data[i * column + 3];
            vert.normal.y = data[i * column + 4];
            vert.normal.z = data[i * column + 5];
        }
        if (uv > 0) {
            vert.texCoords.x = data[i * column + 3 + normal];
            vert.texCoords.y = data[i * column + 4 + normal];
        }
        verts.push_back(vert);
    }
    return verts;
}

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

    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

    Mesh(const std::vector<Vertex> &vertices);

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
