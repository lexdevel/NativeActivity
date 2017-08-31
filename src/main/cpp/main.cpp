#include "AndroidApp.h"

/**
 * @brief The application entry point.
 * @param app The android native application instance
 */
void android_main(struct android_app *app)
{
    AndroidApp::initialize(app);
    AndroidApp::instance()->mainLoop();
}
