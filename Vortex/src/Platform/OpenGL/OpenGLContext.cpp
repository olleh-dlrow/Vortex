#include "vtpch.h"
#include "OpenGLContext.h"

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
        VT_CORE_INFO(" Vendor: {0}", glGetString(GL_VENDOR));
        VT_CORE_INFO(" Renderer: {0}", glGetString(GL_RENDERER));
        VT_CORE_INFO(" Version: {0}", glGetString(GL_VERSION));

    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}
