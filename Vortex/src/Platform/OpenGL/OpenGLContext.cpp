#include "vtpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include <gl/GL.h>

namespace Vortex
{
    
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        :m_WindowHandle(windowHandle)
    {
        VT_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        VT_CORE_ASSERT(status, "Failed to initialize Glad!");

        VT_CORE_INFO("OpenGL Info:");
        VT_CORE_INFO("Vendor: {0}",   (const char*)glGetString(GL_VENDOR));
        VT_CORE_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        VT_CORE_INFO("Version: {0}",  (const char*)glGetString(GL_VERSION));

#ifdef VT_ENABLE_ASSERTS
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        VT_CORE_ASSERT(versionMajor > 4 ||
                           (versionMajor == 4 && versionMinor >= 5),
                       "Vortex requires at least OpenGL version 4.5!");
#endif
        // set MSAA state
        SetMSAA(true);
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}
