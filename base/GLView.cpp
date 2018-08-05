//
//  GLView.cpp
//  gl
//
//  Created by huangkun on 15/11/2017.
//  Copyright © 2017 kk. All rights reserved.
//

#include "GLView.h"
#include <stdio.h>
#include <stdlib.h>
#include "Application.h"
#include "Render.h"
#include "Director.h"
#include "Log.h"
#include "Input.h"
#include "CustomDraw.h"

void keycallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) {
            CustomDraw::currentSceneIndex--;
            if (CustomDraw::currentSceneIndex < 0) {
                CustomDraw::currentSceneIndex = CustomDraw::testScenes.size() - 1;
            }
            auto pair = CustomDraw::testScenes[CustomDraw::currentSceneIndex];
            Director::getInstance()->replaceScene(pair.second());
            log("switch to scene: %s", pair.first.c_str());
        } else if (key == GLFW_KEY_RIGHT) {
            CustomDraw::currentSceneIndex++;
            if (CustomDraw::currentSceneIndex >= CustomDraw::testScenes.size()) {
                CustomDraw::currentSceneIndex = 0;
            }
            auto pair = CustomDraw::testScenes[CustomDraw::currentSceneIndex];
            Director::getInstance()->replaceScene(pair.second());
            log("switch to scene: %s", pair.first.c_str());
        }
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    Input::getInstance()->onScrollCallback(xoffset, yoffset);
}

static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    log("framebuffer_size_callback: %d, %d", width, height);
    glViewport(0, 0, width, height);
}

void window_size_callback(GLFWwindow *window, int width, int height) {
    log("window_size_callback: %d, %d", width, height);
}

GLView::GLView() {
}

GLView::~GLView() {
}

GLView *GLView::createWithRect(int x, int y, int width, int height) {
    auto ret = new GLView();
    if (ret->initWithRect(x, y, width, height)) {
        return ret;
    }
    return nullptr;
}

bool GLView::initWithRect(int x, int y, int width, int height) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        log("Failed to initialize GLFW");
        return false;
    }
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_FLOATING, GL_TRUE);
    // MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(width, height, "GL", NULL, NULL);
    if (!window) {
        log("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        log("Failed to initialize GLAD");
        glfwTerminate();
        return false;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetWindowPos(window, x, y);
    glfwSetKeyCallback(window, keycallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwShowWindow(window);
    _window = window;
    int w = 0, h = 0;
    glfwGetWindowSize(window, &w, &h);
    _winSize.width = w;
    _winSize.height = h;
    glfwGetFramebufferSize(window, &w, &h);
    _frameBufferSize.width = w;
    _frameBufferSize.height = h;
    log("GLView::initWithRect(%d,%d,%d,%d), winSize(%.0f,%.0f), frameBufferSize(%.0f,%.0f)", x, y, width, height,
        _winSize.width, _winSize.height, _frameBufferSize.width, _frameBufferSize.height);

    return true;
}

void GLView::pollEvents() {
    glfwPollEvents();
}

void GLView::swapBuffers() {
    glfwSwapBuffers(_window);
}

bool GLView::windowShouldClose() {
    return glfwWindowShouldClose(_window);
}

void GLView::release() {
    printf("GLView::release");
    glfwTerminate();
}