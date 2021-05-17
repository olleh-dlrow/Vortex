#pragma once

#include "Vortex/Layer.h"

#include "Vortex/Events/KeyEvent.h"
#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/ApplicationEvent.h"

namespace Vortex {
    class VORTEX_API ImGuiLayer: public Layer 
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
		void OnDetach();
        void OnUpdate();
        void OnEvent(Event& event);
    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnWindowResizeEvent(WindowResizeEvent& e);
    private:
        float m_Time;
    };
}
