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

#include "samples/CustomDraw.h"
#include "samples/Triangle.h"
#include "samples/Rectangle.h"
#include "samples/Texture.h"
#include "samples/MVP.h"
#include "samples/Cubes.h"
#include "samples/Camerz.h"
#include "samples/BasicLighting.h"
#include "samples/Materials.h"
#include "samples/LightingMaps.h"
#include "samples/DirectionalLight.h"
#include "samples/PointLight.h"
#include "samples/Spotlight.h"
#include "samples/MultipleLights.h"
#include "samples/ModelLoad.h"
#include "samples/DepthTest.h"
#include "samples/StencilTest.h"
#include "samples/Blending.h"
#include "samples/FaceCulling.h"
#include "samples/FrameBuffer.h"
#include "samples/CubeMaps.h"
#include "samples/AdvancedData.h"
#include "samples/AdvancedGLSL.h"
#include "samples/GeometryShader.h"
#include "samples/GeometryShader2.h"
#include "samples/AntiAliasing.h"
#include "samples/BlinnPhongLighting.h"
#include "samples/GammaCorrection.h"
#include "samples/ShadowMapping.h"
#include "samples/PointShadows.h"
#include "samples/NormalMapping.h"

#include "alg/PathFinding.h"
#include <alg/FunctionDraw.h>
#include "alg/Quaternion.h"

#include "softrender/Rast2DPoint.h"
#include "softrender/Rast2DLine.h"
#include "softrender/Rast2DTriangle.h"
#include "softrender/RastCube.h"
#include "softrender/TextureCube.h"

template<typename T>
Scene *createScene() {
    Scene *scene = Scene::create();
    T *node = new T;
    scene->addChild(node);
    return scene;
}

#define ADD_TEST_SCENE(__CLASS__)\
CustomDraw::testScenes.push_back(std::make_pair(#__CLASS__, &createScene<gltest::__CLASS__>))


int main(int argc, char **argv) {
    auto app = Application::getInstance();
    app->applicationDidFinishLaunching([&]() {
#ifdef __APPLE__
//        auto glView = GLView::createWithRect(0, 0, 400, 300);
        auto glView = GLView::createWithRect(0, 0, 512, 512);
#else
        auto glView = GLView::createWithRect(0, 0, 600, 450);
#endif
        Director::getInstance()->setGLView(glView);

        app->setUpdateInterval(1.0f / 60);


        ADD_TEST_SCENE(Triangle);
        ADD_TEST_SCENE(Rectangle);
        ADD_TEST_SCENE(Texture);
        ADD_TEST_SCENE(MVP);
        ADD_TEST_SCENE(Cubes);
        ADD_TEST_SCENE(Camerz);
        ADD_TEST_SCENE(BasicLighting);
        ADD_TEST_SCENE(Materials);
        ADD_TEST_SCENE(LightingMaps);
        ADD_TEST_SCENE(DirectionalLight);
        ADD_TEST_SCENE(PointLight);
        ADD_TEST_SCENE(Spotlight);
        ADD_TEST_SCENE(MultipleLights);
        ADD_TEST_SCENE(ModelLoad);
        ADD_TEST_SCENE(DepthTest);
        ADD_TEST_SCENE(StencilTest);
        ADD_TEST_SCENE(Blending);
        ADD_TEST_SCENE(FaceCulling);
        ADD_TEST_SCENE(FrameBuffer);
        ADD_TEST_SCENE(CubeMaps);
        ADD_TEST_SCENE(AdvancedData);
        ADD_TEST_SCENE(AdvancedGLSL);
        ADD_TEST_SCENE(GeometryShader);
        ADD_TEST_SCENE(GeometryShader2);
        ADD_TEST_SCENE(AntiAliasing);
        ADD_TEST_SCENE(BlinnPhongLighting);
        ADD_TEST_SCENE(GammaCorrection);
        ADD_TEST_SCENE(ShadowMapping);
        ADD_TEST_SCENE(PointShadows);
        ADD_TEST_SCENE(NormalMapping);

        ADD_TEST_SCENE(PathFinding);
        ADD_TEST_SCENE(FunctionDraw);
        ADD_TEST_SCENE(Texture);

        ADD_TEST_SCENE(Rast2DPoint);
        ADD_TEST_SCENE(Rast2DLine);
        ADD_TEST_SCENE(Rast2DTriangle);
        ADD_TEST_SCENE(RastCube);
        ADD_TEST_SCENE(TextureCube);

        auto currentScene = "TextureCube";
        int index = 0;
        for (auto pair :  CustomDraw::testScenes) {
            if (pair.first == currentScene) {
                CustomDraw::currentSceneIndex = index;
                Director::getInstance()->runWithScene(pair.second());
                break;
            }
            index++;
        }

        return true;
    });
    app->run();
    app->stop();
    return 0;
}
