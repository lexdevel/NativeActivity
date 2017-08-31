#include "GraphicsDevice.h"

GraphicsDevice::GraphicsDevice(ANativeWindow *nativeWindow)
    : m_nativeWindow(nativeWindow)
{
}

GraphicsDevice::~GraphicsDevice()
{
}

void GraphicsDevice::initialize()
{
    const EGLint displayAttribList[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE };
    const EGLint contextAttribList[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

    EGLint w = -1;
    EGLint h = -1;

    EGLConfig config        = nullptr;
    EGLint    configNumber  = -1;
    EGLint    configAttrib  = -1;

    this->m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(this->m_display, nullptr, nullptr);

    eglChooseConfig(this->m_display, displayAttribList, &config, 1, &configNumber);
    eglGetConfigAttrib(this->m_display, config, EGL_NATIVE_VISUAL_ID, &configAttrib);

    ANativeWindow_setBuffersGeometry(this->m_nativeWindow, 0, 0, configAttrib);

    this->m_surface = eglCreateWindowSurface(this->m_display, config, this->m_nativeWindow, nullptr);
    this->m_context = eglCreateContext(this->m_display, config, nullptr, contextAttribList);

    eglMakeCurrent(this->m_display, this->m_surface, this->m_surface, this->m_context);

    eglQuerySurface(this->m_display, this->m_surface,  EGL_WIDTH, &w);
    eglQuerySurface(this->m_display, this->m_surface, EGL_HEIGHT, &h);

    this->m_w = static_cast<unsigned>(w);
    this->m_h = static_cast<unsigned>(h);
}

void GraphicsDevice::finalize()
{
    eglMakeCurrent(this->m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(this->m_display, this->m_context);
    eglDestroySurface(this->m_display, this->m_surface);
    eglTerminate(this->m_display);

    this->m_context = EGL_NO_CONTEXT;
    this->m_surface = EGL_NO_SURFACE;
    this->m_display = EGL_NO_DISPLAY;
}

bool GraphicsDevice::isPrepared() const
{
    return (this->m_display != EGL_NO_DISPLAY &&
            this->m_surface != EGL_NO_SURFACE);
}

void GraphicsDevice::swapBuffer()
{
    eglSwapBuffers(this->m_display, this->m_surface);
}
