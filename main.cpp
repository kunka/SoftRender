//
//  main.cpp
//  gl
//
//  Created by huangkun on 11/09/2017.
//  Copyright © 2017 kk. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <unistd.h>
#include "getopt.h"

#include "Application.h"

////#include "TriangleDemo.h"
////#include "TextureDemo.h"
////#include "DrawFuncDemo.h"
////#include "PlaneDemo.h"
////#include "CameraDemo.h"
////
////Canvas* canvas[] = {
////    new TriangleDemo(),
////    new DrawFuncDemo(),
////    new TextureDemo(),
////    new PlaneDemo(),
////    new CameraDemo(),
////};
//
//int count = sizeof(canvas)/sizeof(*canvas);
//int count =1;
//int displayIndex = 1;
//
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (action != GLFW_PRESS)
//        return;
//
//    switch (key)
//    {
//        case GLFW_KEY_LEFT:
//        {
//            displayIndex--;
//            if (displayIndex == -1){
//                displayIndex = count-1;
//            }
//            break;
//        }
//        case GLFW_KEY_RIGHT:
//        {
//            displayIndex++;
//            if (displayIndex == count) {
//                displayIndex = 0;
//            }
//            break;
//        }
//        case GLFW_KEY_ESCAPE:
//            glfwSetWindowShouldClose(window, GLFW_TRUE);
//            break;
//    }
//}
//
//static void error_callback(int error, const char* description)
//{
//    fprintf(stderr, "Error: %s\n", description);
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and
//    // height will be significantly larger than specified on retina displays.
//    printf("framebuffer_size_callback: %d, %d", width, height);
//    glViewport(0, 0, width, height);
//}

int main(int argc, char **argv) {
    auto ret = Application::getInstance()->run();
    if (ret == 0) {
        printf("Application run success");
    } else {
        printf("Application run fail");
    }
    return ret;


//    int decorated = GLFW_FALSE;
//    int running = GLFW_TRUE;
//
//    glfwSetErrorCallback(error_callback);
//    if (!glfwInit())
//        exit(EXIT_FAILURE);
//
//    glfwWindowHint(GLFW_DECORATED, decorated);
//    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint(GLFW_FLOATING, GL_TRUE);
//
//    GLFWwindow* window = glfwCreateWindow(200, 200, "Demo", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        exit(EXIT_FAILURE);
//    }
//    glfwMakeContextCurrent(window);
//    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
//    glfwSetKeyCallback(window, key_callback);
//    glfwSetWindowPos(window, 0 ,0);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwShowWindow(window);
//
//    GLenum __error = glGetError();
//    if(__error) {
//        printf("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__);
//    }
//
//    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
////    for (int i = 0;  i < count;  i++){
////        canvas[i]->onCreate();
////    }
//
////    Canvas* runningCanvas = nullptr;
////    while (running){
////        Canvas* cs = canvas[displayIndex];
////        if (runningCanvas != cs)
////        {
////            if (runningCanvas && runningCanvas->inited)
////            {
////                runningCanvas->onDestroy();
////                runningCanvas->inited = false;
////            }
////            runningCanvas = cs;
////        }
////        if (!cs->inited)
////        {
////            cs->onCreate();
////            cs->inited = true;
////        }
////        cs->onDraw();
////        glfwSwapBuffers(window);
////        if (glfwWindowShouldClose(window)){
////            running = GLFW_FALSE;
////        }
////        glfwPollEvents();
////        usleep(16000);
////    }
////
////    for (int i = 0;  i < count;  i++){
////        if(canvas[i]->inited)
////        {
////            canvas[i]->onDestroy();
////        }
////    }

    printf("exit success");
//    glfwTerminate();
//    exit(EXIT_SUCCESS);
    return 0;
}
