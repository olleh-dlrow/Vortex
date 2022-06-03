#pragma once

#include "Vortex.h"

#include "Vortex/Renderer/Camera.h"
#include "imgui.h"

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
		ImVec2 ConvertToWinPos(ImVec2 scrPos);
		// X+: right, Y+: down
		ImVec2 ConvertToNormalizedPos(ImVec2 scrPos);

		inline Ref<Camera> GetCamera() const { return m_Camera; }
		inline ImVec2 GetContentSize() const { return m_ContentSize; }
	private:
		std::string m_WindowName;
		ImVec2 m_ContentSize;
		ImVec2 m_AbsContentPos;
		Ref<Camera> m_Camera;
		Ref<FrameBuffer> m_FB;
		bool m_IsFocused;
	};
}