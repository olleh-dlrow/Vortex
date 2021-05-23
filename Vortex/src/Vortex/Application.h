#pragma once

#include "Core.h"
#include "Window.h"
#include "Vortex/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Vortex/ImGui/ImGuiLayer.h"

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

        inline static Application& Get() {return *s_Instance;}  //get application instance
        inline Window& GetWindow() {return *m_Window;}
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
    private:
        static Application* s_Instance;
    };

    //to be defined in client
    Application *CreateApplication();
}