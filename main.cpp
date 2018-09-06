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

#include "Application.h"
#include <Camera.h>
#include "GLView.h"
#include "Director.h"
#include "Scene.h"

#include "samples/CustomDraw.h"
#include "gl/Triangle.h"
#include "gl/Rectangle.h"
#include "gl/Texture.h"
#include "gl/TextureFiltering.h"
#include "gl/Mipmaps.h"
#include "gl/MVP.h"
#include "gl/Cubes.h"
#include "gl/Camerz.h"
#include "gl/BasicLighting.h"
#include "gl/Materials.h"
#include "gl/LightingMaps.h"
#include "gl/DirectionalLight.h"
#include "gl/PointLight.h"
#include "gl/Spotlight.h"
#include "gl/MultipleLights.h"
#include "gl/ModelLoad.h"
#include "gl/DepthTest.h"
#include "gl/StencilTest.h"
#include "gl/Blending.h"
#include "gl/FaceCulling.h"
#include "gl/FrameBuffer.h"
#include "gl/CubeMaps.h"
#include "gl/AdvancedData.h"
#include "gl/AdvancedGLSL.h"
#include "gl/GeometryShader.h"
#include "gl/GeometryShader2.h"
#include "gl/AntiAliasing.h"
#include "gl/BlinnPhongLighting.h"
#include "gl/GammaCorrection.h"
#include "gl/ShadowMapping.h"
#include "gl/PointShadows.h"
#include "gl/NormalMapping.h"
#include "gl/HDR.h"
#include "gl/Bloom.h"
#include "gl/DeferredShading.h"

#include "alg/PathFinding.h"
#include "alg/FunctionDraw.h"
#include "alg/Quaternion.h"

#include "softrender/Rast2DPoint.h"
#include "softrender/Rast2DLine.h"
#include "softrender/Rast2DTriangle.h"
#include "softrender/RastCube.h"
#include "softrender/TextureCube.h"
#include "softrender/Textures.h"
#include "softrender/TextureFilter.h"
#include "softrender/MipMap.h"
#include "softrender/Lighting.h"
#include "softrender/LightingMap.h"
#include "softrender/LoadModel.h"
#include "softrender/VisualizeDepth.h"
#include "softrender/Blend.h"
#include "softrender/ShadowMap.h"
#include "softrender/NormalMap.h"

template<typename T>
Scene *createScene() {
    Scene *scene = Scene::create();
    T *node = new T;
    node->init();
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
        ADD_TEST_SCENE(TextureFiltering);
        ADD_TEST_SCENE(Mipmaps);
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
        ADD_TEST_SCENE(HDR);
        ADD_TEST_SCENE(Bloom);
        ADD_TEST_SCENE(DeferredShading);

        ADD_TEST_SCENE(PathFinding);
        ADD_TEST_SCENE(FunctionDraw);
        ADD_TEST_SCENE(Texture);

        // soft render
        ADD_TEST_SCENE(Rast2DPoint);
        ADD_TEST_SCENE(Rast2DLine);
        ADD_TEST_SCENE(Rast2DTriangle);
        ADD_TEST_SCENE(RastCube);
        ADD_TEST_SCENE(TextureCube);
        ADD_TEST_SCENE(BasicLighting);// opengl
        ADD_TEST_SCENE(Lighting);
        ADD_TEST_SCENE(LightingMaps);// opengl
        ADD_TEST_SCENE(LightingMap);
        ADD_TEST_SCENE(ModelLoad); // opengl
        ADD_TEST_SCENE(LoadModel);
        ADD_TEST_SCENE(DepthTest); // opengl
        ADD_TEST_SCENE(VisualizeDepth);
        ADD_TEST_SCENE(Blending); // opengl
        ADD_TEST_SCENE(Blend);
        ADD_TEST_SCENE(ShadowMapping);// opengl
        ADD_TEST_SCENE(ShadowMap);
        ADD_TEST_SCENE(Texture);// opengl
        ADD_TEST_SCENE(Textures);
        ADD_TEST_SCENE(TextureFiltering);// opengl
        ADD_TEST_SCENE(TextureFilter);
        ADD_TEST_SCENE(Mipmaps);// opengl
        ADD_TEST_SCENE(MipMap);
        ADD_TEST_SCENE(NormalMapping);// opengl
        ADD_TEST_SCENE(NormalMap);

        auto currentScene = "Rast2DPoint";
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
