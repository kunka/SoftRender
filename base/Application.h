#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <functional>

typedef std::function<bool()> OnApplicationDidFinishLaunching;

class Application {
public:
    static Application *getInstance();

    Application();

    virtual ~Application();

    void applicationDidFinishLaunching(
            OnApplicationDidFinishLaunching callback) { _applicationDidFinishLaunching = callback; }

    void setUpdateInterval(float interval);

    int run();

    void stop();

protected:
    static Application *s_application;
    long _interval;  //micro second
    OnApplicationDidFinishLaunching _applicationDidFinishLaunching;
};

#endif  // end of __APPLICATION_H__;
