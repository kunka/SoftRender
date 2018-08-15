//
// Created by huangkun on 04/04/2018.
//

#include "Mesh.h"

using namespace std;

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
           const std::vector<Texture> &textures) {
    _vertices = vertices;
    _indices = indices;
    _textures = textures;
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
           const std::vector<Texture2D *> &textures) {
    _vertices = vertices;
    _indices = indices;
    _texture2Ds = textures;
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
    _vertices = vertices;
    _indices = indices;
}

Mesh::Mesh(const std::vector<Vertex> &vertices) {
    _vertices = vertices;
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(
            Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(
            Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < _textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = _textures[i].type;
        if (name == "texture_diffuse")
            ss << diffuseNr++; // transfer unsigned int to stream
        else if (name == "texture_specular")
            ss << specularNr++; // transfer unsigned int to stream
        number = ss.str();
        shader.setFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, _textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
}

