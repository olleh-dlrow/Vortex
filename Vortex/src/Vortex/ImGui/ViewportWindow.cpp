#include "vtpch.h"
#include "ViewportWindow.h"
#include "Vortex/Events/GraphicsEvent.h"

namespace Vortex
{
	// the whole off-screen render reference to:
	// https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp
	Vortex::ViewportWindow::ViewportWindow(const std::string& winName,
		const Ref<Camera>& cam,
		const glm::vec4& clearColor)
		:m_WindowName(winName), m_Camera(cam), m_IsFocused(false), m_ClearColor(clearColor)
	{
		m_MSAAOpened = Application::Get().GetWindow().GetGraphicsContext().GetMSAA();
		m_HDROpened = Application::Get().GetWindow().GetGraphicsContext().GetHDR();

		CreateFrameBuffers();

		// create screen quad
		float quadVertices[] = {
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

	
	void ViewportWindow::CreateFrameBuffers()
	{
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();

		Ref<Texture2D> tex;
		if (!m_HDROpened)
		{
			tex = Texture2D::Create(width, height, m_MSAAOpened, "RGBA8");
			m_FB = FrameBuffer::Create(width, height, m_MSAAOpened, std::vector<Ref<Texture2D>>{tex});
		}
		else
		{
			tex = Texture2D::Create(width, height, m_MSAAOpened, "RGBA16F");
			m_FB = FrameBuffer::Create(width, height, m_MSAAOpened, std::vector<Ref<Texture2D>>{tex});
		}
			
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
			if (!m_HDROpened)
			{
				tex = Texture2D::Create(width, height, false, "RGBA8");
				m_DownsampleFB = FrameBuffer::Create(width, height, false, std::vector<Ref<Texture2D>>{tex});
			}
			else
			{
				tex = Texture2D::Create(width, height, false, "RGBA16F");
				m_DownsampleFB = FrameBuffer::Create(width, height, false, std::vector<Ref<Texture2D>>{tex});
			}
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

		// wait to implement:
		// open or close postprocess
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
		if (ImGui::Checkbox("MSAA", &m_MSAAOpened))
		{
			Application::Get().GetWindow().GetGraphicsContext().SetMSAA(m_MSAAOpened);
		}
		int nSamples = Application::Get().GetWindow().GetGraphicsContext().GetMSAANSamples();
		if (ImGui::InputInt("Samples", &nSamples, 1))
		{
			Application::Get().GetWindow().GetGraphicsContext().SetMSAANSamples(nSamples);
		}
		if (ImGui::Checkbox("HDR", &m_HDROpened))
		{
			Application::Get().GetWindow().GetGraphicsContext().SetHDR(m_HDROpened);
		}
		float exposure = Application::Get().GetWindow().GetGraphicsContext().GetHDRExposure();
		if (ImGui::DragFloat("HDR Exposure", &exposure))
		{
			Application::Get().GetWindow().GetGraphicsContext().SetHDRExposure(exposure);
		}
		bool gamma = Application::Get().GetWindow().GetGraphicsContext().GetGammea();
		if (ImGui::Checkbox("Gamma", &gamma))
		{
			Application::Get().GetWindow().GetGraphicsContext().SetGamma(gamma);
		}
	}

	void Vortex::ViewportWindow::OnEvent(Event& e)
	{
		if (m_IsFocused)
		{
			m_Camera->OnEvent(e);
		}

		// handle MSAA event
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MSAAClosedEvent>(
			VT_BIND_EVENT_FN(ViewportWindow::OnMSAAChanged));
		dispatcher.Dispatch<MSAAOpenedEvent>(
			VT_BIND_EVENT_FN(ViewportWindow::OnMSAAChanged));
		dispatcher.Dispatch<MSAANSamplesChangedEvent>(
			VT_BIND_EVENT_FN(ViewportWindow::OnMSAAChanged));

		// handle HDR event
		dispatcher.Dispatch<HDROpenedEvent>(
			VT_BIND_EVENT_FN(ViewportWindow::OnHDRChanged));
		dispatcher.Dispatch<HDRClosedEvent>(
			VT_BIND_EVENT_FN(ViewportWindow::OnHDRChanged));
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
	bool ViewportWindow::OnMSAAChanged(Event& e)
	{
		CreateFrameBuffers();
		return false;
	}

	bool ViewportWindow::OnHDRChanged(Event& e)
	{
		CreateFrameBuffers();
		return false;
	}
}