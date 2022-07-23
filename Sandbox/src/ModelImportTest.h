#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/SceneImporter.h>
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>

using Vortex::Ref;
using Vortex::CreateRef;
using Vortex::ViewportWindow;
using Vortex::Camera;
using Vortex::Renderer;
using Vortex::Shader;

using Vortex::Mesh;
using Vortex::MeshComponent;
using Vortex::MeshRendererComponent;
using Vortex::Material;
using Vortex::Scene;
using Vortex::Entity;

class ModelImportTest: public EditorLayer 
{
	std::vector<MeshRendererComponent*> comps;
	bool drawLine;
public:
	ModelImportTest()
	{
		std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
		std::string filename2 = "assets/models/45-acp-smith-and-wesson-with-animation-obj/45-acp-smith-and-wesson-with-animation.obj";
		
		Vortex::SceneImporter sceneImporter(filename2.c_str());
		VT_INFO(sceneImporter.GetNodesInfo());
		VT_INFO(sceneImporter.GetMeshesInfo());

		std::vector<Ref<Mesh>> meshes;
		std::vector <Ref<Material>> mats;
		sceneImporter.ImportMeshAndMat(meshes, mats);

		// create shader and set value
		Ref<Shader> shader = Shader::Create("assets/shaders/ModelImportTest.glsl");

		// set camera position and mode
		Camera& cam = GetCamera();
		cam.SetProjectionMode(false);
		cam.m_Position = glm::vec3(0, 0, 5);
		cam.m_MovementSpeed = 2.0f;

		// create entity, init mesh component and mesh renderercomponent
		Scene* scene = GetEditorScene();
		glm::vec4 colors[5] = {
			{1, 0, 0, 1},	// red
			{0, 1, 0, 1},	// green
			{0, 0, 1, 1},	// blue
			{1, 1, 0, 1},	// yellow
			{1, 0, 1, 1}	// puple
		};
		for (int i = 0; i < meshes.size(); i++)
		{
			Mesh* mesh = meshes[i].get();

			// statistic
			// VT_INFO(mesh->StatisticVertex());
			
			// set mesh color to white
			for (int j = 0; j < mesh->m_Vertices.size(); j++)
			{
				auto& vert = mesh->m_Vertices[j];
				vert.color = colors[i % 5];
			}

			auto e = scene->AddEntity("Entity" + std::to_string(i));
			
			auto mc = e->AddComponent<MeshComponent>();
			mc->m_Mesh = meshes[i];

			// init mesh renderer comp, set shader, mat, mesh
			auto mr = e->AddComponent<MeshRendererComponent>();
			mr->SetMeshComponent(mc);
			mats[i]->m_Shader = shader;
			mr->m_Material = mats[i];

			// set config material callback
			auto callback = [this](Material& mat)
			{
				mat.SetMat4("u_ViewProjection", GetCamera().GetViewProjMatrix());
				mat.SetMat4("u_Transform", glm::identity<glm::mat4>());
			};

			mr->m_ConfigMatCallback = callback;
			mr->m_DrawCfg.polygonMode = Vortex::PolygonMode::LINE;
			// add to comps list
			comps.push_back(mr);
		}
	}

	virtual void OnUpdate(Vortex::Timestep ts) override
	{
		EditorLayer::OnUpdate(ts);

		for (auto& c : comps)
		{
			c->m_DrawCfg.polygonMode = drawLine ? Vortex::PolygonMode::LINE : Vortex::PolygonMode::FILL;

		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		{
			ImGui::Text("Meshes");
			for (auto& c : comps)
			{
				bool v = c->GetEnableValue();
				if (ImGui::Checkbox(c->m_MeshComp->m_Mesh->m_Name.c_str(), &v))
				{
					c->SetEnableValue(v);
				}
			}
			ImGui::Text("Mode");
			ImGui::Checkbox("DrawLine", &drawLine);
		}
		ImGui::End();
	}
};