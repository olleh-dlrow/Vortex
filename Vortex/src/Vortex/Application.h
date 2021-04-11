#pragma once

#include "Core.h"
#include "Window.h"
#include "Vortex/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"


namespace Vortex {

    class VORTEX_API Application
    {
    public:
        Application();
        ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline static Application& Get() {return *s_Instance;}
        inline Window& GetWindow() {return *m_Window;}
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
    private:
        static Application* s_Instance;
    };

    //to be defined in client
    Application *CreateApplication();
}