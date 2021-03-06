#include "vtpch.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/KeyEvent.h"

#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/RendererAPI.h"

#include "Vortex/Renderer/GraphicsContext.h"

namespace Vortex {
    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description) {
        VT_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) {
        Init(props);
    }

    WindowsWindow::~WindowsWindow() {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        VT_CORE_INFO("Create window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        // if it's first create window, initialize glfw
        if ( s_GLFWWindowCount == 0 ) {
            VT_CORE_INFO("Initializing GLFW");
            int success = glfwInit();
            VT_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

#if defined(VT_DEBUG)
        if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        // inc window cnt
        ++s_GLFWWindowCount;

        // create and initialize graphics context
        m_Context = GraphicsContext::Create(m_Window);
        m_Context->Init();
        
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
        // open multiple samples, the effect is low, so we don't use this but opengl's MSAA
        //if(m_Context->GetMSAA())
        //  glfwWindowHint(GLFW_SAMPLES, m_Context->GetMSAANSamples());
        
        //set glfw callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizeEvent event(width, height);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch(action)
                {
                    case GLFW_PRESS:
                    {
                        KeyPressedEvent event(key, 0);
                        data.EventCallback(event);
                        break;                
                    }
                    case GLFW_RELEASE:
                    {
                        KeyReleasedEvent event(key);
                        data.EventCallback(event);
                        break;                
                    }
                    case GLFW_REPEAT:
                    {
                        KeyPressedEvent event(key, 1);
                        data.EventCallback(event);
                        break;                
                    }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keyCode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                KeyTypedEvent event(keyCode);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch(action)
                {
                    case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent event(button);
                        data.EventCallback(event);
                        break;                    
                    }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
    }

    void WindowsWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
        if (--s_GLFWWindowCount == 0)
        {
            VT_CORE_INFO("Ternimating GLFW");
            glfwTerminate();
        }
    }

    void WindowsWindow::OnUpdate() {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    /*
    tips:
    ensure the calculate speed of GPU and screen refresh speed the same.
    Vertical Sync
    */
    void WindowsWindow::SetVSync(bool enabled) {
        if ( enabled ) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const {
        return m_Data.VSync;
    }
}