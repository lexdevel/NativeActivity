#include "Application.h"

std::unique_ptr<Application> Application::s_instance = nullptr;

Application::Application(struct android_app *app)
    : m_app(app), m_graphicsDevice(new GraphicsDevice(app->window))
{
}

Application::~Application()
{
}

void Application::initialize(struct android_app *app)
{
    // The following function call is required to ensure glue code isn't stripped.
    app_dummy();

    Application::s_instance = std::unique_ptr<Application>(new Application(app));
    app->onAppCmd           = Application::onAppCommand;
    app->onInputEvent       = Application::onInputEvent;

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

        if (!this->m_graphicsDevice->isPrepared())
        {
            // Skip rendering if EGL is not initialized...
            continue;
        }

        glClearColor(0.4f, 0.6f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: Render

        this->m_graphicsDevice->swapBuffer();
    }
}

void Application::onAppCommand(struct android_app *app, int32_t cmd)
{
    auto application = Application::instance();
    
    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
        if (app->window)
        {
            application->graphicsDevice()->initialize();
        }
        break;
    case APP_CMD_TERM_WINDOW:
        if (app->window)
        {
            application->graphicsDevice()->finalize();
        }
        break;
    default:
        break;
    }
}

int32_t Application::onInputEvent(struct android_app *app, AInputEvent *inputEvent)
{
    // auto application = Application::instance();

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
