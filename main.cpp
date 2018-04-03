//
//  main.cpp
//  gl
//
//  Created by huangkun on 11/09/2017.
//  Copyright © 2017 kk. All rights reserved.
//

#include <glad/glad.h>
#include <math.h>
#include <iostream>
#include <Camera.h>

#include "Application.h"
#include "GLView.h"
#include "Director.h"
#include "Scene.h"

#include "Triangle.h"
#include "Rectangle.h"
#include "Texture.h"
#include "MVP.h"
#include "Cubes.h"
#include "Camerz.h"
#include "BasicLighting.h"
#include "Materials.h"
#include "LightingMaps.h"
#include "DirectionalLight.h"

#define CREATE_TEST_SCENE(__CLASS__) \
Scene *scene = Scene::create(); \
gltest::__CLASS__ *node = new gltest::__CLASS__(); \
scene->addChild(node); \
Director::getInstance()->runWithScene(scene)

int main(int argc, char **argv) {
    auto app = Application::getInstance();
    app->applicationDidFinishLaunching([&]() {
#ifdef __APPLE__
        auto glView = GLView::createWithRect(0, 0, 200, 150);
#else
        auto glView = GLView::createWithRect(0, 0, 600, 450);
#endif
        Director::getInstance()->setGLView(glView);

        app->setUpdateInterval(1.0f / 60);

//        CREATE_TEST_SCENE(Triangle);
//        CREATE_TEST_SCENE(Rectangle);
//        CREATE_TEST_SCENE(Texture);
//        CREATE_TEST_SCENE(MVP);
//        CREATE_TEST_SCENE(Cubes);
//        CREATE_TEST_SCENE(Camerz);
//        CREATE_TEST_SCENE(BasicLighting);
//        CREATE_TEST_SCENE(Materials);
//        CREATE_TEST_SCENE(LightingMaps);
        CREATE_TEST_SCENE(DirectionalLight);

        return true;
    });
    app->run();
    app->stop();
    return 0;
}
