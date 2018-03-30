//
// Created by huangkun on 27/03/2018.
//

#include "Camera.h"
#include "Director.h"

Camera *Camera::createPerspective(float fovy, float aspect, float zNear, float zFar) {
    Camera *camera = new Camera();
    camera->initPerspective(fovy, aspect, zNear, zFar);
    return camera;
}

void Camera::initPerspective(float fovy, float aspect, float zNear, float zFar) {
    _projection = perspective(fovy, aspect, zNear, zFar);
    setPosition(vec3(0, 10, 10));
    lookAt(vec3(0, 0, -10));
}

void Camera::lookAt(const vec3 &target, const vec3 &up) {
    _target = target;
    _up = up;
    _view = glm::lookAt(_position, target, up);
}

void Camera::initDefault() {
    auto size = Director::getInstance()->getWinSize();
    initPerspective(60, size.width / size.height, 10, 1000);
    setPosition(vec3(0, 10, 10));
    lookAt(vec3(0, 10, 0), vec3(0, 1, 0));
}

const mat4 &Camera::getProjectionMatrix() {
    return _projection;
}

const mat4 &Camera::getViewMatrix() {
    return _view;
}
