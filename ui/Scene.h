#ifndef __SCENE_H__
#define __SCENE_H__

#include "Node.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include <vector>

class Scene : public Node {
public:
    static Scene *create();

    void ignoreTouchEvent();

    bool isRunning() { return _isRunning; }

    virtual void draw(glm::vec4 &transform) override;

protected:
    Scene();

    virtual ~Scene();

    virtual bool init();

    bool _isRunning;
    std::vector<Camera *> _cameras;
    Camera *_defaultCamera;
};

#endif // __SCENE_H__
