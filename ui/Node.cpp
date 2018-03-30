#include "Node.h"

Node::Node() {
}

Node *Node::create() {
    Node *ret = new Node();
    if (ret && ret->init()) {
    }
    return ret;
}

Node::~Node() {
}

bool Node::init() {
    return true;
}

void Node::setPosition(const vec3 &position) {
    _position = position;
}

const vec3 &Node::getPosition() const {
    return _position;
}

void Node::setAnchor(const vec3 &anchor) {
    _anchor = anchor;
}

const vec3 &Node::getAnchor() const {
    return _anchor;
}

void Node::setRotation(const vec3 &rotation) {
    _rotation = rotation;
}

const vec3 &Node::getRotation() const {
    return _rotation;
}

void Node::setScale(const vec3 &scale) {
    _scale = scale;
}

const vec3 &Node::getScale() const {
    return _scale;
}

void Node::setColor(const vec4 &color) {
    _color = color;
}

const vec4 &Node::getColor() const {
    return _color;
}

void Node::onEnter() {}

void Node::onExit() {}

void Node::cleanup() {}

void Node::draw(const mat4 &transform) {
}

void Node::visit(const mat4 &parentTransform) {
    draw(parentTransform);
    for (Node *child : _children) {
        child->visit(parentTransform);
    }
}

void Node::update(float delta) {}

void Node::addChild(Node *child) {
    _children.push_back(child);
}
