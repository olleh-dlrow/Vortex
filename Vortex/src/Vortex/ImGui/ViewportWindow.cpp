#include "vtpch.h"
#include "ViewportWindow.h"

namespace Vortex
{

	// the whole off-screen render reference to:
	// https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp
	
	// wait to implement:
	// when change the MSAA state and MSAA samples, we should recreate fb, rb and their textures
	Vortex::ViewportWindow::ViewportWindow(const std::string& winName,
		const Ref<Camera>& cam,
		const glm::vec4& clearColor)
		:m_WindowName(winName), m_Camera(cam), m_IsFocused(false), m_ClearColor(clearColor)
	{
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();
		m_MSAAOpened = Application::Get().GetWindow().GetGraphicsContext().GetMSAA();
		m_FB = FrameBuffer::Create(width, height, m_MSAAOpened);
		m_RB = RenderBuffer::Create(width, height, m_MSAAOpened);
		m_FB->AttachRenderBuffer(m_RB);
		if (!m_FB->CheckStatus())
		{
			VT_CORE_ASSERT(0, "FrameBuffer is not complete");
		}
		m_FB->Unbind();

		if (m_MSAAOpened)
		{
			// TEST MSAA
			m_DownsampleFB = FrameBuffer::Create(width, height);
			m_DownsampleFB->Bind();

			if (!m_DownsampleFB->CheckStatus())
			{
				VT_CORE_ASSERT(0, "Downsample FrameBuffer is not complete");
			}
			m_DownsampleFB->Unbind();
		}

		// create final screen frame buffer
		m_FinalScreenFB = FrameBuffer::Create(width, height);
		if (!m_FinalScreenFB->CheckStatus())
		{
			VT_CORE_ASSERT(0, "FinalScreen FrameBuffer is not complete");
		}

		// create screen quad
		float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		m_ScreenVertexArray = VertexArray::Create();
		m_ScreenVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		BufferLayout layout = {
			{ShaderDataType::Float2, "aPos"},
			{ShaderDataType::Float2, "aTexCoords"}
		};
		m_ScreenVertexBuffer->SetLayout(layout);
		m_ScreenVertexArray->AddVertexBuffer(m_ScreenVertexBuffer);
		m_ScreenVertexArray->Unbind();
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
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();

		Texture2D* sampledTexture = nullptr;
		if (m_MSAAOpened)
		{
			FrameBuffer::Blit(*m_FB, *m_DownsampleFB);
			m_FB->Unbind();

			sampledTexture = &m_DownsampleFB->GetInnerTexture();
		}
		else
		{
			sampledTexture = &m_FB->GetInnerTexture();
		}
		m_FinalScreenFB->Bind();
		m_ScreenVertexArray->Bind();
		m_OnPostProcessCallback(*sampledTexture);
		Renderer::DrawTriangles(m_ScreenVertexArray, DrawTriangleConfig(6, 0));
		m_FinalScreenFB->Unbind();
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
			ImGui::Image((ImTextureID)(uint64_t)m_FinalScreenFB->GetTextureID(), m_ContentSize, ImVec2(0, 1), ImVec2(1, 0));
			
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
}