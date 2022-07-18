#pragma once

#include "Vortex/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Vortex
{
    class OpenGLContext: public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        // set glfw context to opengl and check opengl version
        virtual void Init() override;
        virtual void SwapBuffers() override;
        virtual void SetMSAA(bool open) override;
        virtual bool GetMSAA() const override { return m_MSAAOpened; }
    private:
        GLFWwindow* m_WindowHandle;
        bool        m_MSAAOpened;       // may use event to broadcast the change of msaa
    };
}