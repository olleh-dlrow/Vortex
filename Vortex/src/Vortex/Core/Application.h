#pragma once

#include "Vortex/Core/Core.h"
#include "Vortex/Core/Window.h"
#include "Vortex/Core/LayerStack.h"
#include "Vortex/Events/Event.h"
#include "Vortex/Events/ApplicationEvent.h"

#include "Vortex/ImGui/ImGuiLayer.h"

namespace Vortex {

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        // push layer to Layer part in LayerStack (below Overlay part)
        void PushLayer(Layer* layer);
        // push layer to Overlay part in LayerStack (above Layer part)
        void PushOverlay(Layer* layer);

        inline static Application& Get() {return *s_Instance;}  //get application instance
        inline Window& GetWindow() {return *m_Window;}
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;

        bool m_Running = true;
        bool m_Minimized = false;

        LayerStack m_LayerStack;

        float m_LastFrameTime = 0.0f;
    private:
        static Application* s_Instance;
    };

    //to be defined in client
    Application *CreateApplication();
}
