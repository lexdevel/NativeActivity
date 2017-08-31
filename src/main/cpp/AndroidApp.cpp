#include "AndroidApp.h"

std::unique_ptr<Application> Application::s_instance = nullptr;

AndroidApp::AndroidApp(struct android_app *app)
    : m_app(app), m_graphicsDevice(new GraphicsDevice(app->window))
{
}

AndroidApp::~AndroidApp()
{
}

void AndroidApp::initialize(struct android_app *app)
{
    // The following function call is required to ensure glue code isn't stripped.
    app_dummy();

    AndroidApp::s_instance  = std::unique_ptr<AndroidApp>(new AndroidApp(app));
    app->onAppCmd           = AndroidApp::onAppCommand;
    app->onInputEvent       = AndroidApp::onInputEvent;
}

void AndroidApp::mainLoop()
{
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

void AndroidApp::onAppCommand(struct android_app *app, int32_t cmd)
{
    auto androidApp = AndroidApp::instance();

    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
        androidApp->graphicsDevice()->initialize();
        break;
    case APP_CMD_TERM_WINDOW:
        androidApp->graphicsDevice()->finalize();
        break;
    default:
        break;
    }
}

int32_t AndroidApp::onInputEvent(struct android_app *app, AInputEvent *inputEvent)
{
    // auto androidApp = AndroidApp::instance();

    if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_KEY &&
        AKeyEvent_getKeyCode(inputEvent) == AKEYCODE_BACK)
    {
        ANativeActivity_finish(app->activity);
        return 1;
    }

    if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_MOTION)
    {
        // TODO: Handle touch event...
        return 1;
    }

    return 0;
}
