#ifndef APPLICATION_H
#define APPLICATION_H

#include <jni.h>
#include <memory>
#include <android/log.h>
#include "GraphicsDevice.h"

#define LOGI(...)           ((void)__android_log_print(ANDROID_LOG_INFO, "NativeActivity", __VA_ARGS__))
#define LOGW(...)           ((void)__android_log_print(ANDROID_LOG_WARN, "NativeActivity", __VA_ARGS__))

/**
 * @brief The application class.
 */
class Application
{
private:
    static std::unique_ptr<Application> s_instance;
private:
    struct android_app              *m_app;
    std::shared_ptr<GraphicsDevice>  m_graphicsDevice;
private:
    explicit Application(struct android_app *app);
public:
    ~Application();

    void mainLoop();

    inline std::shared_ptr<GraphicsDevice> &graphicsDevice() { return this->m_graphicsDevice; }
public:
    static void initialize(struct android_app *app);
    inline static Application *instance() { return Application::s_instance.get(); }

private:
    static void onAppCommand(struct android_app *app, int32_t cmd);
    static int32_t onInputEvent(struct android_app *app, AInputEvent *inputEvent);
};

#endif // APPLICATION_H
