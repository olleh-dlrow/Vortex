#include "vtpch.h"
#include "Vortex/Core/Application.h"

#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Core/Input.h"
#include "Vortex/Core/Window.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/ImGui/ImGuiLayer.h"

#include <GLFW/glfw3.h>

namespace Vortex {

    Application* Application::s_Instance = nullptr;

    Application::Application() 
    {
        VT_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Window::Create();
        /*
        tips:
        bind to a pointer to member function
        */
        m_Window->SetEventCallback(VT_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        // Add an ImGuiLayer to Overlay
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() 
    {
        Renderer::Shutdown();
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        // QUESTION: should layer->OnAttach() be put here or in LayerStack.PushLayer?
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack.PushOverlay(layer);
        // QUESTION: should layer->OnAttach() be put here or in LayerStack.PushLayer?
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        // Will window be closed?
        dispatcher.Dispatch<WindowCloseEvent>(VT_BIND_EVENT_FN(Application::OnWindowClose));
        // Will window be resized?
        dispatcher.Dispatch<WindowResizeEvent>(VT_BIND_EVENT_FN(Application::OnWindowResize));
        
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            if ( e.Handled )break;
            it--;
            (*it)->PreEvent(e);
            (*it)->OnEvent(e);
            (*it)->PostEvent(e);

        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    void Application::Run() {
        while(m_Running) {
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                {
                    layer->PreUpdate(timestep);
                    layer->OnUpdate(timestep);
                    layer->PostUpdate(timestep);
                }
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
                layer->PreImGuiRender();
                layer->OnImGuiRender();
                layer->PostImGuiRender();
            }

            m_ImGuiLayer->End();

            // execute only if events have been handled
            // so the color buffer can't update when you 
            // dragging, resizing.. windows!!
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if(e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        // Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}
