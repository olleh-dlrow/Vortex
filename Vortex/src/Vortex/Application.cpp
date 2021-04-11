#include "vtpch.h"
#include "Application.h"

#include "Log.h"

#include <GLFW/glfw3.h>
#include "Platform/Windows/WindowsWindow.h"

namespace Vortex {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application::Application() {
        m_Window = std::unique_ptr<Window>(Window::Create());
        /*
        tips:
        bind to a pointer to member function
        */
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application() {}

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispath<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        VT_CORE_TRACE("{0}", e);
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    void Application::Run() {
        while(m_Running) {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
}
