#include "EditorLayer.h"

EditorLayer::EditorLayer()
{
	Vortex::Ref<Vortex::Camera> cam = Vortex::CreateRef<Vortex::Camera>(Vortex::OrthoParam(), glm::vec3(0, 0, 5.0f));
	m_ViewportWindow = Vortex::CreateRef<Vortex::ViewportWindow>("Viewport", cam);
    m_ViewportWindow->m_OnPostProcessCallback = VT_BIND_EVENT_FN(EditorLayer::OnPostProcessBase);

    m_EditorScene = Vortex::CreateRef<Vortex::Scene>(m_ViewportWindow.get());
    m_EditorScene->Init();

    auto postProcessShader = Vortex::Shader::Create("assets/shaders/DefaultPostProcess.glsl");
    m_PostProcessMaterial = Vortex::CreateRef<Vortex::Material>("PostProcessMat");
    m_PostProcessMaterial->m_Shader = postProcessShader;
}

void EditorLayer::PreUpdate(Vortex::Timestep ts)
{
    // background
    Vortex::Renderer::SetClearColor(m_ViewportWindow->GetClearColor());
    // clear glfw background and imgui 
    Vortex::Renderer::Clear();
    // start render in viewport window
    m_ViewportWindow->Begin();

    m_ViewportWindow->OnUpdate(ts);
}

void EditorLayer::OnUpdate(Vortex::Timestep ts)
{
    m_EditorScene->OnUpdate(ts);
}

void EditorLayer::PostUpdate(Vortex::Timestep ts)
{
    m_EditorScene->PostUpdate(ts);

    m_ViewportWindow->End();
}

void EditorLayer::OnPostProcessBase(Vortex::Texture2D& renderTexture)
{
    m_PostProcessMaterial->m_Shader->Bind();
    m_PostProcessMaterial->SetInt("screenTexture", 0);
    renderTexture.Bind();
    if (Vortex::Application::Get().GetWindow().GetGraphicsContext().GetHDR())
    {
        m_PostProcessMaterial->SetInt("hdr", 1);
        m_PostProcessMaterial->SetFloat("hdr_exposure", Vortex::Application::Get().GetWindow().GetGraphicsContext().GetHDRExposure());
    }
    else 
    {
        m_PostProcessMaterial->SetInt("hdr", 0);
    }
    bool gamma = Vortex::Application::Get().GetWindow().GetGraphicsContext().GetGamma();
    m_PostProcessMaterial->SetInt("gamma_open", gamma ? 1 : 0);
    
    OnPostProcess(renderTexture);

    m_PostProcessMaterial->ApplyProperties();
}

void EditorLayer::OnPostProcess(Vortex::Texture2D& renderTexture)
{
    
}

void EditorLayer::PreImGuiRender()
{
    // render window
    m_ViewportWindow->OnImGuiRender();

    ImGui::Begin("Viewport Window Config");
    {
        m_ViewportWindow->GetCamera()->RenderConfigGUI();
        m_ViewportWindow->RenderConfigGUI();
    }
    ImGui::End();
    ImGui::ShowDemoWindow();
}

void EditorLayer::OnImGuiRender()
{

}

void EditorLayer::PreEvent(Vortex::Event& e)
{
    m_ViewportWindow->OnEvent(e);
}

void EditorLayer::OnEvent(Vortex::Event& e)
{
}
