//
//  GLView.h
//  gl
//
//  Created by huangkun on 15/11/2017.
//  Copyright © 2017 kk. All rights reserved.
//

#ifndef GLView_h
#define GLView_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Geometry.h"

class Scene;

class GLView {
public:
    virtual ~GLView();

    static GLView *createWithRect(int x, int y, int width, int height);

    virtual void pollEvents();

    virtual void swapBuffers();

    virtual bool windowShouldClose();

    GLFWwindow *getWindow() const { return _window; }

    const Size &getWinSize() { return _winSize; }

    void renderScene(Scene *scene);

    virtual void release();

protected:
    GLView();

    bool initWithRect(int x, int y, int width, int height);

    GLFWwindow *_window;
    Size _winSize;
};

#endif /* GLView_h */
