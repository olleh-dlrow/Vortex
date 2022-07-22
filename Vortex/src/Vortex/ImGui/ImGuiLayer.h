#pragma once

#include "Vortex/Core/Layer.h"

namespace Vortex {
    class Event;
    class ImGuiLayer: public Layer 
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
		virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;
        
        void Begin();
        void End();

    private:
        float m_Time;
    };
}
