#ifndef __NODE_H__
#define __NODE_H__

#include <glm/glm.hpp>
#include <vector>

class Node {
public:
    static Node *create();

    virtual void setPosition(const glm::vec3 &position);

    virtual const glm::vec3 &getPosition() const;

    virtual void setAnchor(const glm::vec3 &anchor);

    virtual const glm::vec3 &getAnchor() const;

    virtual void setRotation(const glm::vec3 &rotation);

    virtual const glm::vec3 &getRotation() const;

    virtual void setScale(const glm::vec3 &scale);

    virtual const glm::vec3 &getScale() const;

    virtual void setColor(const glm::vec4 &color);

    virtual const glm::vec4 &getColor() const;

    virtual void onEnter();

    virtual void onExit();

    virtual void cleanup();

    virtual void draw(const glm::mat4 &transform);

    virtual void visit(const glm::mat4 &parentTransform);

    virtual void update(float delta);

    virtual void addChild(Node *child);

    virtual std::vector<Node *> &getChildren() { return _children; }

    virtual const std::vector<Node *> &getChildren() const { return _children; }

protected:
    // Nodes should be created using create();
    Node();

    virtual ~Node();

    virtual bool init();

    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;
    glm::vec3 _anchor;
    glm::vec4 _color;
    std::vector<Node *> _children;
};

#endif // __NODE_H__
