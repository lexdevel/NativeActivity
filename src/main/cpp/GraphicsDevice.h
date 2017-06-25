#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

/**
 * @brief The graphics device class.
 */
class GraphicsDevice
{
private:
    EGLDisplay m_display = EGL_NO_DISPLAY;
    EGLSurface m_surface = EGL_NO_SURFACE;
    EGLContext m_context = EGL_NO_CONTEXT;
    uint32_t m_w = 0;
    uint32_t m_h = 0;
    ANativeWindow *m_nativeWindow;
public:
    explicit GraphicsDevice(ANativeWindow *nativeWindow);
    virtual ~GraphicsDevice();

    virtual void initialize();
    void finalize();

    bool isPrepared() const;
    void swapBuffer();

    inline const uint32_t &w() const { return this->m_w; }
    inline const uint32_t &h() const { return this->m_h; }
};

#endif // GRAPHICSDEVICE_H
