#pragma once

#include "Vortex/Layer.h"

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
        float m_Time;
    };
}
