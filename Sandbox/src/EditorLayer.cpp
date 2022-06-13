#include "EditorLayer.h"

EditorLayer::EditorLayer()
{
	Vortex::Ref<Vortex::Camera> cam = Vortex::CreateRef<Vortex::Camera>(Vortex::OrthoParam(), glm::vec3(0, 0, 5.0f));
	m_ViewportWindow = Vortex::CreateRef<Vortex::ViewportWindow>("Viewport", cam);
    m_EditorScene = Vortex::CreateRef<Vortex::Scene>(m_ViewportWindow.get());
    m_EditorScene->Init();
}

void EditorLayer::PreUpdate(Vortex::Timestep ts)
{
    m_ViewportWindow->OnUpdate(ts);

    // background
    Vortex::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    // clear glfw background and imgui 
    Vortex::Renderer::Clear();
    // start render in viewport window
    m_ViewportWindow->Begin();

    m_EditorScene->OnUpdate(ts);
}

void EditorLayer::OnUpdate(Vortex::Timestep ts)
{

}

void EditorLayer::AfterUpdate(Vortex::Timestep ts)
{
    m_EditorScene->AfterUpdate(ts);

    m_ViewportWindow->End();
}

void EditorLayer::PreImGuiRender()
{
    m_ViewportWindow->OnImGuiRender();
    m_ViewportWindow->GetCamera()->RenderConfigGUI();
    m_ViewportWindow->RenderConfigGUI();
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
