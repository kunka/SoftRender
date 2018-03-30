#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Director.h"
#include <sys/time.h>
#include <unistd.h>

static long getCurrentMillSecond() {
    long lLastTime = 0;
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime, NULL);
    lLastTime = stCurrentTime.tv_sec * 1000 + stCurrentTime.tv_usec * 0.001; // milliseconds
    return lLastTime;
}

Application *Application::s_application = nullptr;

Application *Application::getInstance() {
    if (!s_application) {
        s_application = new Application();
    }
    return s_application;
}

Application::Application() : _interval(1.0f / 1.0f * 1000.0f) {

}

Application::~Application() {
}

bool Application::applicationDidFinishLaunching() {
    printf("Application::applicationDidFinishLaunching");
    if (Director::getInstance()->createGLView()) {
        return true;
    }
    return false;
}

int Application::run() {
    if (!applicationDidFinishLaunching()) {
        return 1;
    }

    long lastTime = 0L;
    long curTime = 0L;
    auto director = Director::getInstance();
    auto glView = director->getGLView();

    while (!glView->windowShouldClose()) {
        lastTime = getCurrentMillSecond();

        director->mainLoop();  // handle event logic and draw

        curTime = getCurrentMillSecond();
        if (curTime - lastTime < _interval) {
            usleep(static_cast<useconds_t>((_interval - curTime + lastTime) * 1000));
        }
    }
    director->release();

    return 0;
}

void Application::setUpdateInterval(float interval) {
    _interval = interval * 1000.0f;
}

void Application::release() {
    printf("Application::release");
    auto glView = Director::getInstance()->getGLView();
    glView->windowShouldClose();
}
