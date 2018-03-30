#include "Director.h"
#include "Application.h"
#include "Shader.h"
#include "Render.h"

using namespace std;
Director *Director::s_director = nullptr;
Shader shader;

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
    _winSize.width = 200;
    _winSize.height = 200;
    return true;
}

Director::~Director(void) {
    s_director = nullptr;
    delete _render;
}

GLView *Director::createGLView() {
    if (!_glView) {
        _glView = GLView::createWithRect(0, 0, _winSize.width, _winSize.height);
    }
    return _glView;
}

unsigned int VBO, VAO;
bool inited = false;

void initDraw() {
    if (not inited) {
        inited = true;
        shader.loadStr(shader_positionColor_vert, shader_positionColor_frag);

        float vertices[] = {
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // bottom right
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom left
                0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top
        };

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
}

void Director::drawScene() {
//    initDraw();

//    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);

    // update shader uniform
//    shader.use();

//    glBindVertexArray(VAO);

    // render the TriangleDemo
//    glDrawArrays(GL_TRIANGLES, 0, 3);

//    _renderer->clear();
//    if (_nextScene) {
//        setNextScene();
//    }
    _render->clear();
    if (_runningScene) {
        glm::mat4 transform;
        _runningScene->draw(transform);
//        //render the scene
//        _openGLView->renderScene(_runningScene, _renderer);
    }

////    _renderer->render();
}

void Director::runWithScene(Scene *scene) {
    pushScene(scene);
}

void Director::replaceScene(Scene *scene) {
    if (_runningScene == nullptr) {
        runWithScene(scene);
        return;
    }

    if (scene == _nextScene)
        return;

//    if (_nextScene)
//    {
//        if (_nextScene->isRunning())
//        {
//            _nextScene->onExit();
//        }
//        _nextScene->cleanup();
//        _nextScene = nullptr;
//    }
//    _scenesStack.replace(index, scene);

    _nextScene = scene;

    if (_runningScene) {
        // to avoid rapid click multi-times
        _runningScene->ignoreTouchEvent();
    }
}

void Director::pushScene(Scene *scene) {
    _scenesStack.push_back(scene);
    _nextScene = scene;

    if (_runningScene) {
        _runningScene->ignoreTouchEvent();
    }
}

void Director::popScene(void) {
    _scenesStack.pop_back();
    ssize_t c = _scenesStack.size();

    if (c == 0) {
        release();
    } else {
        _nextScene = _scenesStack.at(c - 1);
    }

    if (_runningScene) {
        _runningScene->ignoreTouchEvent();
    }
}

void Director::popToRootScene(void) {
    popToSceneStackLevel(1);
}

void Director::popToSceneStackLevel(int level) {
    ssize_t c = _scenesStack.size();

    // level 0? -> end
    if (level == 0) {
        release();
        return;
    }

    // current level or lower -> nothing
    if (level >= c)
        return;

    auto firstOnStackScene = _scenesStack.back();
    if (firstOnStackScene == _runningScene) {
        _scenesStack.pop_back();
        --c;
    }

    // pop stack until reaching desired level
    while (c > level) {
        auto current = _scenesStack.back();

        if (current->isRunning()) {
            current->onExit();
        }

        current->cleanup();
        _scenesStack.pop_back();
        --c;
    }

    _nextScene = _scenesStack.back();

    if (_runningScene) {
        _runningScene->ignoreTouchEvent();
    }
}

void Director::release() {
    printf("Director::release");
    if (_glView) {
        _glView->release();
        _glView = nullptr;
    }
}

void Director::setNextScene() {
}

void Director::mainLoop() {
    // check and call events and swap the buffers
    _glView->pollEvents();
    processInput();
    // rendering commands here
    drawScene();
    _glView->swapBuffers();
}

void Director::processInput() {
}