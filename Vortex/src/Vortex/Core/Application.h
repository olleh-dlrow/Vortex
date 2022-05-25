#pragma once

#include "Vortex/Core/Core.h"
#include "Vortex/Core/Window.h"
#include "Vortex/Core/LayerStack.h"
#include "Vortex/Events/Event.h"
#include "Vortex/Events/ApplicationEvent.h"

#include "Vortex/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Vortex {

    class Application
    {
    public:
        Application();
        virtual ~Application();

        // CORE part to event handling
        // the TRIGGER TIME depend on other function's callback
        // it's not certain
        // 
        // ==== process:
        // e -> captured by glfw 
        // -> trigger window::eventCallBack(App::OnEvent)
        // -> Application::OnEvent(e) -> end
        // 
        // ==== handle event:
        // e -> dispatch<some event>(handler) -> dispatch... -> ...
        // e -> layer1(OnEvent) -> layer2(OnEvent) -> ...
        void OnEvent(Event& e);

        // push layer to Layer part in LayerStack (below Overlay part)
        void PushLayer(Layer* layer);
        // push layer to Overlay part in LayerStack (above Layer part)
        void PushOverlay(Layer* layer);

        inline static Application& Get() {return *s_Instance;}  //get application instance
        inline Window& GetWindow() {return *m_Window;}
    private:
        // MAIN LOOP of the application
        // Calulate game time
        // Update all layers
        // Update all GUIs for layers
        // update window
        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;

        float m_LastFrameTime = 0.0f;

        // states
        bool m_Running = true;
        bool m_Minimized = false;
    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    };

    //to be defined in client
    Application *CreateApplication();
}
