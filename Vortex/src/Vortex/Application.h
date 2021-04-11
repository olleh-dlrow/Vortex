#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace Vortex {

    class VORTEX_API Application
    {
    public:
        Application();
        ~Application();

        void Run();

        void OnEvent(Event& e);
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    //to be defined in client
    Application *CreateApplication();
}