#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include "Scene.h"
#include "GLView.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Geometry.h"

class Render;

class Director {
public:
    static Director *getInstance();

    Director();

    ~Director();

    bool init();

    Scene *getRunningScene() { return _runningScene; }

    void runWithScene(Scene *scene);

    void pushScene(Scene *scene);

    void popScene();

//    void popToRootScene();

//    void popToSceneStackLevel(int level);

    void replaceScene(Scene *scene);

    void release();

    void drawScene();

    void processInput(float delta);

    void mainLoop(float delta);

    const Size &getWinSize() { return _glView->getWinSize(); }

    const Size &getFrameBufferSize() { return _glView->getFrameBufferSize(); }

    GLView *getGLView() { return _glView; }

    void setGLView(GLView *glView);

protected:
    void setNextScene();

    Scene *_runningScene;
    Scene *_toBeReleaseScene;
    Scene *_nextScene;
    std::vector<Scene *> _scenesStack;
    GLView *_glView;
    Render *_render;
    static Director *s_director;
};

#endif // __DIRECTOR_H__
