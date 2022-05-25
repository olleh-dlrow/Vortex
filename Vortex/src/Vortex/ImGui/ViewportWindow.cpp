#include "vtpch.h"
#include "ViewportWindow.h"
#include "imgui.h"

Vortex::ViewportWindow::ViewportWindow(const std::string& winName, const Ref<Camera>& cam)
	:m_WindowName(winName), m_Camera(cam), m_IsFocused(false)
{
	int width = Application::Get().GetWindow().GetWidth();
	int height = Application::Get().GetWindow().GetHeight();
	m_FB = Vortex::FrameBuffer::Create(width, height);
}

void Vortex::ViewportWindow::Begin()
{
	m_FB->Bind();
	Renderer::Clear();
}

void Vortex::ViewportWindow::End()
{
	m_FB->Unbind();
}

void Vortex::ViewportWindow::OnImGuiRender()
{
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
		ImVec2 sz = ImGui::GetContentRegionAvail();

		m_Camera->m_Aspect = (float)sz.x / sz.y;
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)m_FB->GetTextureID(), sz, ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
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
