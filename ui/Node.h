#ifndef __NODE_H__
#define __NODE_H__

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class Node {
public:
    static Node *create();

    virtual void setPosition(const vec3 &position);

    virtual const vec3 &getPosition() const;

    virtual void setAnchor(const vec3 &anchor);

    virtual const vec3 &getAnchor() const;

    virtual void setRotation(const vec3 &rotation);

    virtual const vec3 &getRotation() const;

    virtual void setScale(const vec3 &scale);

    virtual const vec3 &getScale() const;

    virtual void setColor(const vec4 &color);

    virtual const vec4 &getColor() const;

    virtual void onEnter();

    virtual void onExit();

    virtual void cleanup();

    virtual void draw(vec4 &transform);

    virtual void visit();

    virtual void update(float delta);

    virtual void addChild(Node * child);

    virtual vector<Node *> &getChildren() { return _children; }

    virtual const vector<Node *> &getChildren() const { return _children; }

protected:
    // Nodes should be created using create();
    Node();

    virtual ~Node();

    virtual bool init();

    vec3 _position;
    vec3 _rotation;
    vec3 _scale;
    vec3 _anchor;
    vec4 _color;
    vector<Node *> _children;
};

#endif // __NODE_H__
