#include "Application.h"
#include "Director.h"
#include <sys/time.h>
#include <unistd.h>
#include "Log.h"

static long nowInMS() {
    long ret = 0;
    struct timeval now;
    gettimeofday(&now, NULL);
    ret = now.tv_sec * 1000 + now.tv_usec / 1000;
    return ret;
}

Application *Application::s_application = nullptr;

Application *Application::getInstance() {
    if (!s_application) {
        s_application = new Application();
    }
    return s_application;
}

Application::Application() : _interval(1000 / 60) {

}

Application::~Application() {
}

int Application::run() {
    if (!_applicationDidFinishLaunching()) {
        log("Application::run fail");
        return 1;
    }
    log("Application::run");

    long last, now = 0;
    float delta;
    auto director = Director::getInstance();
    auto glView = director->getGLView();

    int count = 0;
    long start = nowInMS();
    while (glView && !glView->windowShouldClose()) {
        last = nowInMS();
        if (now == 0) {
            delta = _interval / 1000.0f;
        } else {
            delta = (last - now) / 1000.0f;
        }

        director->mainLoop(delta);  // handle event logic and draw

        now = nowInMS();
        if (now - last < _interval) {
            usleep(static_cast<useconds_t>((_interval - now + last) * 1000));
        }
        count++;
        if (count >= 10) {
            now = nowInMS();
            log("FPS = %.1f", 1000.0f / ((now - start) / count));
            count = 0;
            start = now;
        }
    }

    return 0;
}

void Application::setUpdateInterval(float interval) {
    _interval = long(interval * 1000.0f);
}

void Application::stop() {
    log("Application::stop");
    auto director = Director::getInstance();
    director->release();
}
