//
// Created by huangkun on 25/01/2018.
//

#ifndef GL_ENTITY_H
#define GL_ENTITY_H

#include <vector>

using namespace std;

class Component;

class Entity {

public:
    virtual void update(float delta) {};

    virtual void fixedUpdate(float delta) {};

    bool addComponent(Component *component);

    bool removeComponent(Component *component);

    Component *findComponent(const std::string &name);

protected:
    std::vector<Component *> components;
};


#endif //GL_ENTITY_H
