#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/TransformComponent.h>
#include <Vortex/Scene/PointRendererComponent.h>

using Vortex::Entity;

static std::vector<glm::vec3> positions;
static std::vector<glm::vec4> colors;

class SceneTest : public EditorLayer
{
public:
	SceneTest()
		:EditorLayer()
	{
		positions = {
			{0, 0, 0},
			{1, 1, 1},
			{-1, -1, -1}
		};

		colors = {
			{0.2f, 0.8f, 0.6f, 1.0f},
			{0.2f, 0.8f, 0.6f, 1.0f},
			{0.2f, 0.8f, 0.6f, 1.0f}
		};

		Vortex::Scene* scene = GetEditorScene();
		//Vortex::Scene& scene = *(m_EditorScene.get());
		Entity* e1 = scene->AddEntity("e1");
		Entity* e2 = scene->AddEntity("e2");

		e1->AddComponent<Vortex::TransformComponent>();
		e2->AddComponent<Vortex::DerivedComponent>();
		pr = e1->AddComponent<Vortex::PointRendererComponent>();

		Vortex::TransformComponent* t = e1->GetComponent<Vortex::TransformComponent>();
	}

	virtual void OnUpdate(Vortex::Timestep ts) override
	{
		pr->DrawPoints(positions, 1.0f, colors);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("SceneTest");
		{
			Vortex::Scene* scene = GetEditorScene();
			auto t = scene->FindFirstEntityByName("e1")
				->GetComponent<Vortex::TransformComponent>();
			ImGui::DragFloat3("position", (float*)&t->GetPosition());
		}
		ImGui::End();
	}

	Vortex::PointRendererComponent* pr;
};
