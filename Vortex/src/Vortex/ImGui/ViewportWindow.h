#pragma once

#include "Vortex.h"
#include "Vortex/Renderer/Camera.h"

namespace Vortex
{
	class ViewportWindow
	{
	public:
		ViewportWindow(const std::string& winName, const Ref<Camera>& cam);
		~ViewportWindow() {};
		void Begin();
		void End();
		void OnImGuiRender();
		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);
		inline Ref<Camera> GetCamera() const { return m_Camera; }
	private:
		std::string m_WindowName;
		Ref<Camera> m_Camera;
		Ref<FrameBuffer> m_FB;
		bool m_IsFocused;
	};
}