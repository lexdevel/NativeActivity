#ifndef APPLICATION_H
#define APPLICATION_H

#include <jni.h>
#include <memory>
#include <android_native_app_glue.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

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
    struct android_app *m_app;
    EGLDisplay m_display = EGL_NO_DISPLAY;
    EGLSurface m_surface = EGL_NO_SURFACE;
    EGLContext m_context = EGL_NO_CONTEXT;
    int32_t m_screenW = -1;
    int32_t m_screenH = -1;
private:
    explicit Application(struct android_app *app);
public:
    virtual ~Application();

    void mainLoop();
public:
    static void initialize(struct android_app *app);
    inline static Application *instance() { return Application::s_instance.get(); }
private:
    static void glesInit();
    static void glesFree();
    static void onAppCommand(struct android_app *app, int32_t cmd);
    static int32_t onInputEvent(struct android_app *app, AInputEvent *inputEvent);
};

#endif // APPLICATION_H
