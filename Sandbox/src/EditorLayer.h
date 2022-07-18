#pragma once

#include <imgui.h>
#include <Vortex.h>

class EditorLayer : public Vortex::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void PreUpdate(Vortex::Timestep ts) override;
	virtual void OnUpdate(Vortex::Timestep ts) override;
	virtual void PostUpdate(Vortex::Timestep ts) override;
	virtual void OnPostProcess(Vortex::Texture2D& renderTexture);

	virtual void PreImGuiRender() override;
	virtual void OnImGuiRender() override;

	virtual void PreEvent(Vortex::Event& e) override;
	virtual void OnEvent(Vortex::Event& e) override;

	inline Vortex::Camera& GetCamera() { return *m_ViewportWindow->GetCamera(); };
	inline Vortex::Scene* GetEditorScene() { return m_EditorScene.get(); }
protected:
	Vortex::Ref<Vortex::ViewportWindow> m_ViewportWindow;
	Vortex::Ref<Vortex::Scene>			m_EditorScene;
	Vortex::Ref<Vortex::Shader>			m_DefaultScreenShader;
};
