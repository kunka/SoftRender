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
#include "Director.h"
#include "stb_image.h"

#define TEST_NODE_BEGIN(__CLASS__) \
namespace gltest{ \
class __CLASS__ : public CustomDraw{ \
public: \
    __CLASS__(); \
    ~__CLASS__(); \
    virtual void draw(const mat4 &transform) override;

#define TEST_NODE_BEGIN2(__CLASS__, __SUPER_CLASS__) \
namespace gltest{ \
class __CLASS__ : public __SUPER_CLASS__{ \
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

    virtual void fixedUpdate(float delta) override;

    unsigned int loadTexture(const std::string &path, bool mipmap = true, bool gammaCorrection = false);

    static int currentSceneIndex;
    static std::vector<std::pair<std::string, Scene *(*)()>> testScenes;

    virtual bool init() override;

protected:
    void renderCube();

    Shader shader;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int lightVAO;
    unsigned int lightVBO;
    unsigned int texture;
    mat4 model;
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
    vec3 cameraDir;
    vec3 cameraUp;
    float pitch;
    float yaw;
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;
};

#endif //GL_CUSTOMDRAW_H
