#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Vortex {

    class VORTEX_API Application
    {
    public:
        Application();
        ~Application();
        void Run();
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    //to be defined in client
    Application *CreateApplication();
}