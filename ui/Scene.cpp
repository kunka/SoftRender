#include "Scene.h"
#include "Render.h"
#include "Log.h"

Scene::Scene()
        : _isRunning(false) {
}

Scene *Scene::create() {
    Scene *ret = new Scene();
    if (ret && ret->init()) {
    }
    return ret;
}

Scene::~Scene() {
}

bool Scene::init() {
    _defaultCamera = new Camera();
    _defaultCamera->initDefault();
    _cameras.push_back(_defaultCamera);
    addChild(_defaultCamera);

    return true;
}

void Scene::render(Render *render) {
    log("render");
    mat4 transform;
    for (const auto *camera : _cameras) {
        for (Node *child : _children) {
            child->visit(transform);
        }
    }
}

