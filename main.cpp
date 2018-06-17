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


#define CREATE_TEST_SCENE(__CLASS__) \
Scene *scene = Scene::create(); \
gltest::__CLASS__ *node = new gltest::__CLASS__(); \
scene->addChild(node); \
Director::getInstance()->runWithScene(scene)

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

//        CREATE_TEST_SCENE(Triangle);
//        CREATE_TEST_SCENE(Rectangle);
//        CREATE_TEST_SCENE(Texture);
//        CREATE_TEST_SCENE(MVP);
//        CREATE_TEST_SCENE(Cubes);
//        CREATE_TEST_SCENE(Camerz);
//        CREATE_TEST_SCENE(BasicLighting);
//        CREATE_TEST_SCENE(Materials);
//        CREATE_TEST_SCENE(LightingMaps);
//        CREATE_TEST_SCENE(DirectionalLight);
//        CREATE_TEST_SCENE(PointLight);
//        CREATE_TEST_SCENE(Spotlight);
//        CREATE_TEST_SCENE(MultipleLights);
//        CREATE_TEST_SCENE(ModelLoad);
//        CREATE_TEST_SCENE(DepthTest);
//        CREATE_TEST_SCENE(StzencilTest);
//        CREATE_TEST_SCENE(Blending);
//        CREATE_TEST_SCENE(FaceCulling);
//        CREATE_TEST_SCENE(FrameBuffer);
//        CREATE_TEST_SCENE(CubeMaps);
//        CREATE_TEST_SCENE(AdvancedData);
//        CREATE_TEST_SCENE(AdvancedGLSL);
//        CREATE_TEST_SCENE(GeometryShader);
//        CREATE_TEST_SCENE(GeometryShader2);
//        CREATE_TEST_SCENE(AntiAliasing);
//        CREATE_TEST_SCENE(BlinnPhongLighting);
//        CREATE_TEST_SCENE(GammaCorrection);
//        CREATE_TEST_SCENE(ShadowMapping);
//        CREATE_TEST_SCENE(PointShadows);
//        CREATE_TEST_SCENE(NormalMapping);

//        CREATE_TEST_SCENE(PathFinding);
//        CREATE_TEST_SCENE(FunctionDraw);
//        CREATE_TEST_SCENE(Texture);


//        CREATE_TEST_SCENE(Rast2DPoint);
        CREATE_TEST_SCENE(Rast2DLine);

        return true;
    });
    app->run();
    app->stop();
    return 0;
}
