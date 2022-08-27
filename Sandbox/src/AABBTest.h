#pragma once

#include "EditorLayer.h"
#include <Vortex/Renderer/Texture.h>
#include <Vortex/Renderer/VertexArray.h>
#include <Vortex/Scene/TransformComponent.h>
#include <Vortex/Renderer/Shader.h>
using namespace Vortex;

class AABBComponent: public Component {
public:
	void Init() override 
	{
		m_Shader = Shader::Create("assets/shaders/Cube.glsl");
		m_Camera = GetEntity()->GetScene()->GetInnerCamera();
		m_Trans = GetEntity()->GetComponent<TransformComponent>();
	}

	glm::vec3 GetMin() 
	{
		return m_Trans->GetModelMatrix() * glm::vec4(Cube4::GetMinPosition(), 1);
	}

	glm::vec3 GetMax() 
	{
		return m_Trans->GetModelMatrix() * glm::vec4(Cube4::GetMaxPosition(), 1);
	}

	void OnUpdate(Timestep ts) override 
	{
		
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", m_Camera->GetViewProjMatrix());
		m_Shader->SetMat4("u_Transform", m_Trans->GetModelMatrix());
		m_Shader->SetFloat4("u_Color", color);
		RenderCube();
	}

	void RenderCube()
	{
		static Ref<VertexArray> cubeVA = nullptr;
		static Ref<VertexBuffer> cubeVB = nullptr;
		if (cubeVA == nullptr)
		{
			cubeVA = VertexArray::Create();
			cubeVB = VertexBuffer::Create(Cube4::GetVertices(), Cube4::GetVerticesSize());
			cubeVB->SetLayout(Cube4::VertexType::GetLayout());
			cubeVA->AddVertexBuffer(cubeVB);
		}
		cubeVA->Bind();
		Renderer::DrawTriangles(cubeVA, Vortex::DrawTriangleConfig(36, 0, PolygonMode::LINE));
	}

	void RenderConfigGUI()
	{
		ImGui::ColorEdit4("color", (float*)&color);
	}

	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	Ref<Shader> m_Shader;
	Camera* m_Camera = nullptr;
	TransformComponent* m_Trans;
};

class AABBTest : public EditorLayer {
public:
	
	Entity* e1;
	Entity* e2;

	Entity* GetAABB() {
		auto e1 = GetEditorScene()->AddEntity("E1");
		e1->AddComponent<TransformComponent>();
		e1->AddComponent<AABBComponent>();
		return e1;
	}

	bool Intersect(AABBComponent* c1, AABBComponent* c2) {
		auto min1 = c1->GetMin(), min2 = c2->GetMin();
		auto max1 = c1->GetMax(), max2 = c2->GetMax();
		return max1.x > min2.x
			&& max1.y > min2.y
			&& max1.z > min2.z
			&& max2.x > min1.x
			&& max2.y > min1.y
			&& max2.z > min1.z;
	}

	AABBTest() {
		// set camera position and mode
		Camera& cam = GetCamera();
		cam.SetProjectionMode(false);
		cam.m_Position = glm::vec3(0, 0, 5);
		cam.m_MovementSpeed = 2.0f;
		cam.ReCalculateCameraVectors();

		// config e1
		e1 = GetAABB();
		e1->GetComponent<AABBComponent>()->color = glm::vec4(1, 0, 0, 1);

		// config e2
		e2 = GetAABB();
		e2->GetComponent<AABBComponent>()->color = glm::vec4(0, 1, 0, 1);
	}

	void OnUpdate(Timestep ts) override {
		EditorLayer::OnUpdate(ts);
	}

	void OnImGuiRender() override {
		ImGui::Begin("Debug");
		{
			ImGui::Text("E1");
			ImGui::PushID("E1");
			e1->GetComponent<AABBComponent>()->RenderConfigGUI();
			e1->GetComponent<TransformComponent>()->RenderConfigGUI();
			ImGui::PopID();

			ImGui::Text("E2");
			ImGui::PushID("E2");
			e2->GetComponent<AABBComponent>()->RenderConfigGUI();
			e2->GetComponent<TransformComponent>()->RenderConfigGUI();
			ImGui::PopID();
			if (Intersect(e1->GetComponent<AABBComponent>(),
				e2->GetComponent<AABBComponent>())) {
				ImGui::Text("Intersect");
			}
		}
		ImGui::End();
	}
};