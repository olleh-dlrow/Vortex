#pragma once

#include "Core.h"
#include "Window.h"
#include "Vortex/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Vortex/ImGui/ImGuiLayer.h"

#include "Vortex/Renderer/Shader.h"
#include "Vortex/Renderer/Buffer.h"

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

        unsigned int m_VertexArray;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

    private:
        static Application* s_Instance;
    };

    //to be defined in client
    Application *CreateApplication();
}