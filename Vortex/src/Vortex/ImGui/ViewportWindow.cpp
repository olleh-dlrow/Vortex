#include "vtpch.h"
#include "ViewportWindow.h"

// the whole off-screen render reference to:
// https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp
Vortex::ViewportWindow::ViewportWindow(const std::string& winName, 
									   const Ref<Camera>& cam,
									   const glm::vec4& clearColor)
	:m_WindowName(winName), m_Camera(cam), m_IsFocused(false), m_ClearColor(clearColor)
{
	int width = Application::Get().GetWindow().GetWidth();
	int height = Application::Get().GetWindow().GetHeight();
	m_FB = Vortex::FrameBuffer::Create(width, height);
	m_RB = Vortex::RenderBuffer::Create(width, height);
	m_FB->AttachRenderBuffer(m_RB);
	if (!m_FB->CheckStatus())
	{
		VT_CORE_ASSERT(0, "FrameBuffer is not complete");
	}
	m_FB->Unbind();
}

void Vortex::ViewportWindow::Begin()
{
	m_FB->Bind();
	Renderer::SetDepthTest(true);
	Renderer::SetClearColor(m_ClearColor);
	Renderer::Clear();
}

void Vortex::ViewportWindow::End()
{
	m_FB->Unbind();
	Renderer::SetDepthTest(false);
}

void Vortex::ViewportWindow::OnImGuiRender()
{
	// viewport
	ImGui::Begin(m_WindowName.c_str());
	{
		if (ImGui::IsWindowFocused())
		{
			m_IsFocused = true;
		}
		else
		{
			m_IsFocused = false;
		}
		m_AbsContentPos = ImGui::GetWindowContentRegionMin();
		m_AbsContentPos.x += ImGui::GetWindowPos().x;
		m_AbsContentPos.y += ImGui::GetWindowPos().y;

		m_ContentSize = ImGui::GetContentRegionAvail();

		m_Camera->m_Aspect = (float)m_ContentSize.x / m_ContentSize.y;
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)(uint64_t)m_FB->GetTextureID(), m_ContentSize, ImVec2(0, 1), ImVec2(1, 0));

		// TEST
		//{
		//	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		//	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		//	vMin.x += ImGui::GetWindowPos().x;
		//	vMin.y += ImGui::GetWindowPos().y;
		//	vMax.x += ImGui::GetWindowPos().x;
		//	vMax.y += ImGui::GetWindowPos().y;

		//	ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
		//	//VT_CORE_INFO("({}, {})", vMin.x, vMin.y);
		//	//VT_CORE_INFO("({}, {})", vMax.x, vMax.y);
		//}
	}
	ImGui::End();
}

void Vortex::ViewportWindow::RenderConfigGUI()
{
	ImGui::Text(("Cfg_" + m_WindowName).c_str());
	ImGui::ColorEdit4("Clear Color", (float*)&m_ClearColor);
	ImGui::Text("IsFocused: %s", m_IsFocused ? "Yes" : "No");
}

void Vortex::ViewportWindow::OnEvent(Event& e)
{
	if (m_IsFocused)
	{
		m_Camera->OnEvent(e);
	}
}

void Vortex::ViewportWindow::OnUpdate(Timestep ts)
{
	if (m_IsFocused)
	{
		m_Camera->OnUpdate(ts);
	}
}

ImVec2 Vortex::ViewportWindow::ConvertToWinPos(ImVec2 scrPos)
{
	ImVec2 winPos = scrPos;
	winPos.x -= m_AbsContentPos.x;
	winPos.y -= m_AbsContentPos.y;
	return winPos;
}

ImVec2 Vortex::ViewportWindow::ConvertToNormalizedPos(ImVec2 scrPos)
{
	ImVec2 winPos = ConvertToWinPos(scrPos);
	ImVec2 sz = GetContentSize();
	return ImVec2(winPos.x / sz.x, winPos.y / sz.y);
}
