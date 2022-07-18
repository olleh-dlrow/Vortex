#include "EditorLayer.h"

EditorLayer::EditorLayer()
{
	Vortex::Ref<Vortex::Camera> cam = Vortex::CreateRef<Vortex::Camera>(Vortex::OrthoParam(), glm::vec3(0, 0, 5.0f));
	m_ViewportWindow = Vortex::CreateRef<Vortex::ViewportWindow>("Viewport", cam);
    m_ViewportWindow->m_OnPostProcessCallback = VT_BIND_EVENT_FN(EditorLayer::OnPostProcess);

    m_EditorScene = Vortex::CreateRef<Vortex::Scene>(m_ViewportWindow.get());
    m_EditorScene->Init();

    m_DefaultScreenShader = Vortex::Shader::Create("assets/shaders/DefaultScreen.glsl");
}

void EditorLayer::PreUpdate(Vortex::Timestep ts)
{
    // background
    Vortex::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
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

void EditorLayer::OnPostProcess(Vortex::Texture2D& renderTexture)
{
    m_DefaultScreenShader->Bind();
    m_DefaultScreenShader->SetInt("screenTexture", 0);
    renderTexture.Bind();
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
