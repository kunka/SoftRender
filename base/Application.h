#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Application {
public:
    static Application *getInstance();

    Application();

    virtual ~Application();

    virtual bool applicationDidFinishLaunching();

    virtual void setUpdateInterval(float interval);

    int run();

    void release();

protected:
    static Application *s_application;
    long _interval;  //micro second
};

#endif  // end of __APPLICATION_H__;
