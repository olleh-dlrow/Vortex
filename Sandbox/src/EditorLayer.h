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
	virtual void AfterUpdate(Vortex::Timestep ts) override;


	virtual void PreImGuiRender() override;
	virtual void OnImGuiRender() override;

	virtual void PreEvent(Vortex::Event& e) override;
	virtual void OnEvent(Vortex::Event& e) override;

	inline Vortex::Camera& GetCamera() const { return *m_ViewportWindow->GetCamera(); };

protected:
	Vortex::Ref<Vortex::ViewportWindow> m_ViewportWindow;
};