//
// Created by huangkun on 26/03/2018.
//

#ifndef GL_CUBE_H
#define GL_CUBE_H

#include "Node.h"
#include "Shader.h"

class Cube : public Node {
public:
    static Cube *create();

protected:
    Cube();

    virtual ~Cube();

    virtual bool init();

    virtual void draw() override;

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int shaderProgram;
    Shader shader;
};

#endif //GL_CUBE_H
