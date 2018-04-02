#include "Director.h"
#include "Application.h"
#include "Render.h"
#include "Log.h"

using namespace std;
Director *Director::s_director = nullptr;

Director *Director::getInstance() {
    if (!s_director) {
        s_director = new Director();
        s_director->init();
    }

    return s_director;
}

Director::Director() {
}

bool Director::init(void) {
    _runningScene = nullptr;
    _nextScene = nullptr;
    _scenesStack.reserve(15);
    _render = new(std::nothrow) Render;
    return true;
}

Director::~Director(void) {
    s_director = nullptr;
    delete _render;
}

void Director::setGLView(GLView *glView) {
    _glView = glView;
}

void Director::processInput(float delta) {
    if (_runningScene) {
        _runningScene->fixedUpdate(Application::getInstance()->getUpdateInterval());
        _runningScene->update(delta);
    }
}

void Director::drawScene() {
    if (_runningScene) {
        _runningScene->render(_render);
    }
}

void Director::runWithScene(Scene *scene) {
    pushScene(scene);
}

void Director::replaceScene(Scene *scene) {
    if (_runningScene == nullptr) {
        runWithScene(scene);
        return;
    }
    ssize_t index = _scenesStack.size() - 1;
    auto back = _scenesStack.back();
    delete back;
    _scenesStack[index] = scene;

    _nextScene = scene;
}

void Director::pushScene(Scene *scene) {
    log("pushScene");
    _scenesStack.push_back(scene);
    _nextScene = scene;
}

void Director::popScene(void) {
    auto back = _scenesStack.back();
    delete back;
    _scenesStack.pop_back();

    if (_scenesStack.size() == 0) {
        release();
    } else {
        _nextScene = _scenesStack.back();
    }
}

//void Director::popToRootScene(void) {
//    popToSceneStackLevel(1);
//}
//
//void Director::popToSceneStackLevel(int level) {
//    ssize_t c = _scenesStack.size();
//
//    // level 0? -> end
//    if (level == 0) {
//        release();
//        return;
//    }
//
//    // current level or lower -> nothing
//    if (level >= c)
//        return;
//
//    auto firstOnStackScene = _scenesStack.back();
//    if (firstOnStackScene == _runningScene) {
//        _scenesStack.pop_back();
//        --c;
//    }
//
//    // pop stack until reaching desired level
//    while (c > level) {
//        auto current = _scenesStack.back();
//
//        if (current->isRunning()) {
//            current->onExit();
//        }
//
//        current->cleanup();
//        _scenesStack.pop_back();
//        --c;
//    }
//
//    _nextScene = _scenesStack.back();
//}

void Director::release() {
    log("Director::release");
    if (_glView) {
        _glView->release();
        _glView = nullptr;
    }
}

void Director::setNextScene() {
}

void Director::mainLoop(float delta) {
    // switch scene
    if (_nextScene) {
        _runningScene = _nextScene;
        _nextScene = nullptr;
    }

    // check and call events and swap the buffers
    _glView->pollEvents();
    processInput(delta);
    // rendering commands here
    drawScene();
    _glView->swapBuffers();
}
