#ifndef __SCENE_H__
#define __SCENE_H__

#include "Node.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include <vector>

class Render;

class Scene : public Node {
public:
    static Scene *create();

    bool isRunning() { return _isRunning; }

    void render(Render *render);


    virtual ~Scene();

protected:
    Scene();

    virtual bool init();

    bool _isRunning;
    std::vector<Camera *> _cameras;
    Camera *_defaultCamera;
};

#endif // __SCENE_H__
