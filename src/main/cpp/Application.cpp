#include "Application.h"

std::unique_ptr<Application> Application::s_instance = nullptr;

Application::Application(struct android_app *app)
    : m_app(app)
{
}

Application::~Application()
{
}

void Application::initialize(struct android_app *app)
{
    // The following function call is required to ensure glue code isn't stripped.
    app_dummy();

    Application::s_instance.reset(new Application(app));
    app->onAppCmd       = Application::onAppCommand;
    app->onInputEvent   = Application::onInputEvent;
    app->userData       = static_cast<void *>(Application::s_instance.get());

    LOGI("Application initialized!");
}

void Application::mainLoop()
{
    LOGI("Application is starting main loop!");

    volatile bool isDestroyRequested = false;
    while (!isDestroyRequested)
    {
        int events = -1;
        struct android_poll_source *source = nullptr;

        while (ALooper_pollAll(0, nullptr, &events, (void **)&source) >= 0)
        {
            if (source != nullptr)
            {
                source->process(this->m_app, source);
            }

            if (this->m_app->destroyRequested != 0)
            {
                isDestroyRequested = true;
            }
        }

        if (this->m_display == EGL_NO_DISPLAY || this->m_surface == EGL_NO_SURFACE)
        {
            // Skip rendering if EGL not initialized...
            continue;
        }

        glClearColor(0.4f, 0.6f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: Render

        eglSwapBuffers(this->m_display, this->m_surface);
    }
}

void Application::glesInit()
{
    LOGI("Initializing OpenGL ES 2.0!");

    auto application = Application::instance();

    const EGLint displayAttribList[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE };
    const EGLint contextAttribList[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

    EGLConfig config;
    EGLint    format;
    EGLint    num;

    application->m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(application->m_display, nullptr, nullptr);

    eglChooseConfig(application->m_display, displayAttribList, &config, 1, &num);
    eglGetConfigAttrib(application->m_display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(application->m_app->window, 0, 0, format);

    application->m_surface = eglCreateWindowSurface(application->m_display, config, application->m_app->window, nullptr);
    application->m_context = eglCreateContext(application->m_display, config, nullptr, contextAttribList);

    eglMakeCurrent(application->m_display, application->m_surface, application->m_surface, application->m_context);

    eglQuerySurface(application->m_display, application->m_surface,  EGL_WIDTH, &application->m_screenW);
    eglQuerySurface(application->m_display, application->m_surface, EGL_HEIGHT, &application->m_screenH);
}

void Application::glesFree()
{
    auto application = Application::instance();

    eglMakeCurrent(application->m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(application->m_display, application->m_context);
    eglDestroySurface(application->m_display, application->m_surface);
    eglTerminate(application->m_display);

    application->m_context = EGL_NO_CONTEXT;
    application->m_surface = EGL_NO_SURFACE;
    application->m_display = EGL_NO_DISPLAY;
}

void Application::onAppCommand(struct android_app *app, int32_t cmd)
{
    auto application = Application::instance();
    
    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
        if (app->window)
        {
            Application::glesInit();
        }
        break;
    case APP_CMD_TERM_WINDOW:
        if (app->window)
        {
            Application::glesFree();
        }
        break;
    default:
        break;
    }
}

int32_t Application::onInputEvent(struct android_app *app, AInputEvent *inputEvent)
{
    auto application = Application::instance();

    if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_KEY)
    {
        if (AKeyEvent_getKeyCode(inputEvent) == AKEYCODE_BACK)
        {
            LOGI("Native activity finish request.");
            ANativeActivity_finish(app->activity);
        }

        return 1;
    }

    return 0;
}
