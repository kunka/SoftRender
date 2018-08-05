//
// Created by Administrator on 2018/4/2 0002.
//

#include "Input.h"
#include "Director.h"
#include "Log.h"

Input *Input::s_input = nullptr;

Input *Input::getInstance() {
    if (!s_input) {
        s_input = new Input();
        s_input->init();
    }

    return s_input;
}

Input::Input() {
}

bool Input::init(void) {
    _firstMouseMove = true;
    _firstScroll = true;
    return true;
}

Input::~Input(void) {
    s_input = nullptr;
}

void Input::onScrollCallback(float xoffset, float yoffset) {
    _scrollOffsetPre = _scrollOffset;
    _scrollOffset.x = xoffset;
    _scrollOffset.y = yoffset;
    if (_firstScroll) {
        _firstScroll = false;
        _scrollOffsetPre = _scrollOffset;
    }
}

glm::vec2 Input::getMousePosDelta() {
    return _mousePos - _mousePosPre;
}

glm::vec2 Input::getScrollOffsetDelta() {
    return _scrollOffset - _scrollOffsetPre;
}

void Input::update(float delta) {
    GLFWwindow *window = Director::getInstance()->getGLView()->getWindow();
    if (isKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    _mousePosPre = _mousePos;
    _mousePos.x = x;
    _mousePos.y = y;
    if (_firstMouseMove) {
        _firstMouseMove = false;
        _mousePosPre = _mousePos;
    }
}

int Input::getKey(int keyCode) {
    GLFWwindow *window = Director::getInstance()->getGLView()->getWindow();
    return glfwGetKey(window, keyCode);
}

bool Input::isKeyPressed(int keyCode) {
    return getKey(keyCode) == GLFW_PRESS;
}

bool Input::isKeyClicked(int keyCode) {
    return getKey(keyCode) == GLFW_PRESS;
}
