//
// Created by huangkun on 30/03/2018.
//

#ifndef GL_CUSTOMDRAW_H
#define GL_CUSTOMDRAW_H

#include "Node.h"
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Log.h"

#define TEST_NODE_BEGIN(__CLASS__) \
namespace gltest{ \
class __CLASS__ : public CustomDraw{ \
public: \
    __CLASS__(); \
    ~__CLASS__(); \
    virtual void draw(const mat4 &transform) override;

#define TEST_NODE_END(__CLASS__) };};

#define TEST_NODE_IMP_BEGIN namespace gltest{
#define TEST_NODE_IMP_END };

class CustomDraw : public Node {
public:
    CustomDraw();

    ~CustomDraw();

protected:
    Shader shader;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

#endif //GL_CUSTOMDRAW_H
