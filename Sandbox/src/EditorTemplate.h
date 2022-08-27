#pragma once

#include "EditorLayer.h"
using namespace Vortex;

class EditorTemplate : public EditorLayer {
public:
	EditorTemplate() {
		// set camera position and mode
		Camera& cam = GetCamera();
		cam.SetProjectionMode(false);
		cam.m_Position = glm::vec3(0, 0, 5);
		cam.m_MovementSpeed = 2.0f;
		cam.ReCalculateCameraVectors();
	}

	void OnAttach() override {

	}

	void OnUpdate(Timestep ts) override {
		EditorLayer::OnUpdate(ts);

		//...
	}

	void OnImGuiRender() override {
		ImGui::Begin("Debug");
		{

		}
		ImGui::End();
	}
};