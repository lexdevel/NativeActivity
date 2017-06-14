#include "Application.h"

/**
 * @brief The application entry point.
 * @param app The android native application instance
 */
void android_main(struct android_app *app)
{
    Application::initialize(app);
    Application::instance()->mainLoop();
}
