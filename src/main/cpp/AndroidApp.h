#ifndef ANDROIDAPP_H
#define ANRDOIDAPP_H

#include <jni.h>
#include <memory>
#include "GraphicsDevice.h"

/**
 * @brief The application class.
 */
class AndroidApp
{
private:
    static std::unique_ptr<Application> s_instance;
private:
    struct android_app              *m_app;
    std::shared_ptr<GraphicsDevice>  m_graphicsDevice;
private:
    explicit AndroidApp(struct android_app *app);
public:
    ~AndroidApp();

    void mainLoop();

    inline const std::shared_ptr<GraphicsDevice> &graphicsDevice() const { return this->m_graphicsDevice; }
public:
    static void initialize(struct android_app *app);
    inline static AndroidApp *instance() { return AndroidApp::s_instance.get(); }

private:
    static void onAppCommand(struct android_app *app, int32_t cmd);
    static int32_t onInputEvent(struct android_app *app, AInputEvent *inputEvent);
};

#endif // ANRDOIDAPP_H
