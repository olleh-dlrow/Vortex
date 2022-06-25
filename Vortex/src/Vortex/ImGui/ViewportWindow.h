#pragma once

#include "Vortex/Renderer/Buffer.h"
#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/Camera.h"
#include "Vortex/Core/Application.h"

#include "imgui_internal.h"
#include "imgui.h"

namespace Vortex
{
	class ViewportWindow
	{
	public:
		ViewportWindow(const std::string& winName, 
					   const Ref<Camera>& cam,
					   const glm::vec4& clearColor=glm::vec4(0.1f, 0.1f, 0.1f, 1));
		~ViewportWindow() {};
		void Begin();
		void End();
		void OnImGuiRender();
		// show config to imGUI, this can used to debug, change config value and so on
		void RenderConfigGUI();

		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);
		// convert screen pos to inner viewport window pos
		ImVec2 ConvertToWinPos(ImVec2 scrPos);
		// X+: right, Y+: down
		ImVec2 ConvertToNormalizedPos(ImVec2 scrPos);

		inline Ref<Camera> GetCamera() const { return m_Camera; }
		inline Camera* GetCamera() { return m_Camera.get(); }
		inline ImVec2 GetContentSize() const { return m_ContentSize; }
		inline ImVec2 GetTopLeftPosInScreen() const { return m_AbsContentPos; }
		inline ImVec2 GetBottomRightPosInScreen() const { return ImVec2(m_AbsContentPos.x + m_ContentSize.x, m_AbsContentPos.y + m_ContentSize.y); }
		inline glm::vec4& GetClearColor() { return m_ClearColor; }
		inline bool IsInside(ImVec2 scrPos) const 
		{
			auto vMin = GetTopLeftPosInScreen();
			auto vMax = GetBottomRightPosInScreen();
			return scrPos.x >= vMin.x && scrPos.y >= vMin.y &&
				   scrPos.x <= vMax.x && scrPos.y <= vMax.y;
		}
		inline bool IsFocused() const { return m_IsFocused; }
	private:
		std::string					m_WindowName;
		ImVec2						m_ContentSize;
		ImVec2						m_AbsContentPos;	// screen pos of top-left content
		Ref<Camera>					m_Camera;
		Ref<FrameBuffer>			m_FB;
		bool						m_IsFocused;
		glm::vec4					m_ClearColor;
	};
}