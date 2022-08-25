#include "vtpch.h"
#include "ViewportWindow.h"
#include "Vortex/Events/GraphicsEvent.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Geo/DrawGeoConfig.h"
#include "Vortex/Core/Application.h"
#include "Vortex/Core/Window.h"
#include "Vortex/Renderer/GraphicsContext.h"
#include "Vortex/Geo/Rect.h"

#include "Vortex/Renderer/Buffer.h"
#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/Camera.h"
#include "Vortex/Core/Application.h"
#include "Vortex/Renderer/Material.h"
#include "Vortex/Renderer/Texture.h"

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
		int nSamples = m_MSAAOpened ? 
			Application::Get().GetWindow().GetGraphicsContext().GetMSAANSamples() : 0;

		InitFrameBuffers();
		AttachTextures(nSamples, m_HDROpened);
		AttachRenderBuffers(nSamples);
		CheckFrameBuffers();

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

	void ViewportWindow::InitFrameBuffers()
	{
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();
		m_TargetFB = FrameBuffer::Create();
		m_DownsampleFB = FrameBuffer::Create();

		// init final screen frame buffer
		m_FinalScreenFB = FrameBuffer::Create();
		m_FinalTex2D = Texture2D::Create(width, height, "RGBA8");
		m_FinalScreenFB->AttachTexture2D(*m_FinalTex2D, 0);

		// unbind
		m_TargetFB->Unbind();
		m_DownsampleFB->Unbind();
		m_FinalScreenFB->Unbind();
		m_FinalScreenFB->Unbind();
	}

	void ViewportWindow::AttachTextures(int nSamples, bool hdr)
	{
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();
		const char* format = hdr ? "RGBA16F" : "RGBA8";
		if (m_MSAAOpened) {
			m_TargetTex2D = MultisampleTexture2D::Create(width, height, nSamples, format);

			m_DownsampleTex2D = Texture2D::Create(width, height, format);
			m_DownsampleFB->Bind();
			m_DownsampleFB->AttachTexture2D(*m_DownsampleTex2D, 0);
			m_DownsampleFB->Unbind();
		}
		else
		{
			m_TargetTex2D = Texture2D::Create(width, height, format);
		}
		m_TargetFB->Bind();
		m_TargetFB->AttachTexture2D(*m_TargetTex2D, 0);
		m_TargetFB->Unbind();
	}

	void ViewportWindow::AttachRenderBuffers(int nSamples)
	{
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();
		if (m_MSAAOpened)
		{
			m_TargetRB = RenderBuffer::Create(width, height, nSamples);
		} 
		else
		{
			m_TargetRB = RenderBuffer::Create(width, height);
		}
		m_TargetFB->Bind();
		m_TargetFB->AttachRenderBuffer(m_TargetRB);
		m_TargetFB->Unbind();
		m_TargetRB->Unbind();
	}

	void ViewportWindow::CheckFrameBuffers()
	{
		m_TargetFB->Bind();
		if (!m_TargetFB->CheckStatus()) {
			VT_CORE_ASSERT(0, "FrameBuffer is not complete");
		}
		m_TargetFB->Unbind();

		if (m_MSAAOpened)
		{
			m_DownsampleFB->Bind();
			if (!m_DownsampleFB->CheckStatus()) {
				VT_CORE_ASSERT(0, "FrameBuffer is not complete");
			}
			m_DownsampleFB->Unbind();
		}

		m_FinalScreenFB->Bind();
		if (!m_FinalScreenFB->CheckStatus()) {
			VT_CORE_ASSERT(0, "FrameBuffer is not complete");
		}
		m_FinalScreenFB->Unbind();
	}
	
	void Vortex::ViewportWindow::Begin()
	{
		int width = Application::Get().GetWindow().GetWidth();
		int height = Application::Get().GetWindow().GetHeight();
		m_TargetFB->Bind();
		Renderer::SetViewport(0, 0, width, height);
		Renderer::SetDepthTest(true);
		Renderer::SetClearColor(m_ClearColor);
		Renderer::Clear();
	}

	Texture2D& ViewportWindow::GetRenderTexture()
	{
		if (m_MSAAOpened)
		{
			FrameBuffer::Blit(*m_TargetFB, 
				RectInt(0, 0, m_TargetTex2D->GetWidth(), m_TargetTex2D->GetHeight()),
				*m_DownsampleFB,
				RectInt(0, 0, m_DownsampleTex2D->GetWidth(), m_DownsampleTex2D->GetHeight()));

			return *m_DownsampleTex2D;
		}
		else
		{
			return *m_TargetTex2D;
		}
	}

	void Vortex::ViewportWindow::End()
	{
		m_TargetFB->Unbind();
		Renderer::SetDepthTest(false);

		// wait to implement:
		// open or close postprocess
		Texture2D& renderTexture = GetRenderTexture();
		m_FinalScreenFB->Bind();
		m_ScreenVertexArray->Bind();
		m_OnPostProcessCallback(renderTexture);
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
			ImGui::Image((ImTextureID)(uint64_t)m_FinalTex2D->GetID(), m_ContentSize, ImVec2(0, 1), ImVec2(1, 0));
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
		bool gamma = Application::Get().GetWindow().GetGraphicsContext().GetGamma();
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
		int nSamples = 0;
		switch (e.GetEventType())
		{
		case EventType::MSAAOpened:
			m_MSAAOpened = true;
			nSamples = ((MSAAOpenedEvent&)e).m_NSamples;
			break;
		case EventType::MSAAClosed:
			m_MSAAOpened = false;
			break;
		case EventType::MSAANSamplesChanged:
			nSamples = ((MSAANSamplesChangedEvent&)e).m_NSamples;
			break;
		default:
			break;
		}
		AttachTextures(nSamples, m_HDROpened);
		AttachRenderBuffers(nSamples);
		CheckFrameBuffers();
		
		return false;
	}

	bool ViewportWindow::OnHDRChanged(Event& e)
	{
		int exposure = 0;
		switch (e.GetEventType())
		{
		case EventType::HDROpened:
			m_HDROpened = true;
			break;
		case EventType::HDRClosed:
			m_HDROpened = false;
			break;
		default:
			break;
		}
		int nSamples = m_MSAAOpened ?
			Application::Get().GetWindow().GetGraphicsContext().GetMSAANSamples() : 0;
		AttachTextures(nSamples, m_HDROpened);
		CheckFrameBuffers();

		return false;
	}

	void ViewportWindow::BindTargetFrameBuffer() const
	{
		m_TargetFB->Bind();
	}
}