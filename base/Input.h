//
// Created by Administrator on 2018/4/2 0002.
//

#ifndef GL_INPUT_H
#define GL_INPUT_H

#include <glm/glm.hpp>

class Input {
public:
    static Input *getInstance();

    Input();

    ~Input();

    bool init();

    void update(float delta);

    void onScrollCallback(float xoffset, float yoffset);

    int getKey(int keyCode);

    bool isKeyPressed(int keyCode);

    bool isKeyClicked(int keyCode);

    const glm::vec2 &getMousePos() { return _mousePos; }

    const glm::vec2 &getScrollOffset() { return _scrollOffset; }

    glm::vec2 getMousePosDelta();

    glm::vec2 getScrollOffsetDelta();

protected:
    static Input *s_input;
    bool _firstMouseMove;
    bool _firstScroll;
    glm::vec2 _mousePosPre;
    glm::vec2 _mousePos;
    glm::vec2 _scrollOffsetPre;
    glm::vec2 _scrollOffset;
};

#endif //GL_INPUT_H
