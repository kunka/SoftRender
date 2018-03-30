#include "Scene.h"

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
    addChild(_defaultCamera);

    return true;
}

void Scene::ignoreTouchEvent() {

}

void Scene::draw(glm::vec4 &transform) {
    for (const auto *camera : _cameras) {

    }
}

