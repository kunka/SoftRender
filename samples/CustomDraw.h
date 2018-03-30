//
// Created by huangkun on 30/03/2018.
//

#ifndef GL_CUSTOMDRAW_H
#define GL_CUSTOMDRAW_H

#include "Node.h"
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

class CustomDraw : public Node {
public:
    CustomDraw();

    ~CustomDraw();

    virtual void draw(const mat4 &transform) override;

private:
    Shader shader;
    unsigned int VAO;
    unsigned int VBO;
};


#endif //GL_CUSTOMDRAW_H
