//
// Created by huangkun on 27/03/2018.
//

#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include "Node.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Camera : public Node {
public:
    /**
    * The type of camera.
    */
    enum class Type {
        PERSPECTIVE = 1,
        ORTHOGRAPHIC = 2
    };
public:
    static Camera *createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

    virtual void lookAt(const vec3 &target, const vec3 &up = vec3(0, 1, 0));

    void initDefault();

    const mat4 &getProjectionMatrix();

    const mat4 &getViewMatrix();

protected:
    void initPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

private:
    mat4 _projection;
    mat4 _view;
    vec3 _target;
    vec3 _up;
};

#endif //GL_CAMERA_H
