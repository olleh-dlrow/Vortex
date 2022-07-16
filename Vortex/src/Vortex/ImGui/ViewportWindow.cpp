#include "vtpch.h"
#include "ViewportWindow.h"

namespace Vortex
{

	// the whole off-screen render reference to:
	// https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp
	Vortex::ViewportWindow::ViewportWindow(const std::string& winName,
		const Ref<Camera>& cam,
		const glm::vec4& clearColor)
		:m_WindowName(winName), m_Camera(cam), m_IsFocused(false), m_ClearColor(clearColor)
	{
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();
		m_FB = FrameBuffer::Create(width, height);
		m_RB = RenderBuffer::Create(width, height);
		m_FB->AttachRenderBuffer(m_RB);
		if (!m_FB->CheckStatus())
		{
			VT_CORE_ASSERT(0, "FrameBuffer is not complete");
		}
		m_FB->Unbind();

		// TEST MSAA
		glGenFramebuffers(1, &intermediateFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
		// create a color attachment texture

		glGenTextures(1, &screenTexture);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		//glCreateTextures(GL_TEXTURE_2D, 1, &screenTexture);
		//glTextureStorage2D(screenTexture, 1, GL_RGB8, width, height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			VT_CORE_ASSERT(0, "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenFramebuffers(1, &finalScreenFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, finalScreenFBO);
		// create a color attachment texture

		glGenTextures(1, &finalScreenTexture);
		glBindTexture(GL_TEXTURE_2D, finalScreenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		// warning: can't use this, can't set GL_RGB to GL_RGB8 for framebuffer's texture,
		// this setting has unknown aliasing for lines
		//glCreateTextures(GL_TEXTURE_2D, 1, &finalScreenTexture);
		//glTextureStorage2D(finalScreenTexture, 1, GL_RGB8, width, height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalScreenTexture, 0);	// we only need a color buffer
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			VT_CORE_ASSERT(0, "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		// create 
		m_ScreenVertexArray = VertexArray::Create();
		m_ScreenVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		BufferLayout layout = {
			{ShaderDataType::Float2, "aPos"},
			{ShaderDataType::Float2, "aTexCoords"}
		};
		m_ScreenVertexBuffer->SetLayout(layout);
		m_ScreenVertexArray->AddVertexBuffer(m_ScreenVertexBuffer);
		m_ScreenVertexArray->Unbind();
		m_ScreenShader = Shader::Create("assets/shaders/Screen.glsl");
	}

	void Vortex::ViewportWindow::Begin()
	{
		m_FB->Bind();
		Renderer::SetDepthTest(true);
		Renderer::SetClearColor(m_ClearColor);
		Renderer::Clear();
	}

	float offset = 1.0f / 300.0f;

	void Vortex::ViewportWindow::End()
	{
		m_FB->Unbind();
		Renderer::SetDepthTest(false);
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();
		
		//glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
		//m_ScreenVertexArray->Bind();
		//m_ScreenShader->Bind();
		//m_ScreenShader->SetInt("screenTexture", 0);
		//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_FB->GetTextureID());
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// TEST MSAA
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FB->GetID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// postprocess
		glBindFramebuffer(GL_FRAMEBUFFER, finalScreenFBO);
		m_ScreenVertexArray->Bind();
		m_ScreenShader->Bind();
		m_ScreenShader->SetInt("screenTexture", 0);
		m_ScreenShader->SetFloat("offset", offset);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
			//ImGui::Image((ImTextureID)(uint64_t)m_FB->GetTextureID(), m_ContentSize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Image((ImTextureID)(uint64_t)finalScreenTexture, m_ContentSize, ImVec2(0, 1), ImVec2(1, 0));

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
		ImGui::DragFloat("Offset", &offset, 0.001f);
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