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

class BlingPhongTest : public EditorLayer
{	
	std::vector<MeshRendererComponent*> comps;
	bool drawLine = false;
public:
	BlingPhongTest()
	{
		std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
		std::string filename2 = "assets/models/45-acp-smith-and-wesson-with-animation-obj/45-acp-smith-and-wesson-with-animation.obj";

		Vortex::SceneImporter sceneImporter(filename2.c_str());
		VT_INFO(sceneImporter.GetNodesInfo());
		VT_INFO(sceneImporter.GetMeshesInfo());

		std::vector<Ref<Mesh>> meshes;
		std::vector <Ref<Material>> mats;
		sceneImporter.ImportMeshAndMat(meshes, mats);

		// set camera position and mode
		Camera& cam = GetCamera();
		cam.SetProjectionMode(false);
		cam.m_Position = glm::vec3(0, 0, 5);
		cam.m_MovementSpeed = 12.0f;

		// create entity, init mesh component and mesh renderercomponent
		Scene* scene = GetEditorScene();
		for (int i = 0; i < meshes.size(); i++)
		{
			Mesh* mesh = meshes[i].get();

			auto e = scene->AddEntity("Entity" + std::to_string(i));

			auto mc = e->AddComponent<MeshComponent>();
			mc->m_Mesh = meshes[i];

			// init mesh renderer comp, set shader, mat, mesh
			auto mr = e->AddComponent<MeshRendererComponent>();
			mr->SetMeshComponent(mc);
			mats[i]->m_Shader = Shader::Create("assets/shaders/BlingPhongTest.glsl");
			// set all meshes' material the same in convenience to adjust parameters
			mr->m_Material = mats[1];

			mats[i]->m_Shader->Bind();
			mats[i]->SetFloat3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
			mats[i]->SetFloat3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
			mats[i]->SetFloat3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));

			// set config material callback
			auto callback = [this](Material& mat)
			{
				glm::mat4 I = glm::identity<glm::mat4>();
				glm::mat4 trans = I;// glm::scale(I, glm::vec3(0.1, 0.1, 0.1));
				mat.SetMat4("u_ViewProjection", GetCamera().GetViewProjMatrix());
				mat.SetMat4("u_Transform", trans);

				mat.SetFloat3("viewPos", GetCamera().m_Position);
			};

			mr->m_ConfigMatCallback = callback;
			// add to comps list
			comps.push_back(mr);
		}
	}

	virtual void OnUpdate(Vortex::Timestep ts) override
	{
		EditorLayer::OnUpdate(ts);

		for (auto& c : comps)
		{
			c->m_DrawCfg.polygonMode = drawLine ? GL_LINE : GL_FILL;
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
		
			auto getIdx = [&]()->int{
				static int curItem = 0;
				char** items = new char* [comps.size()];
				for (int i = 0; i < comps.size(); i++)
				{
					char* item = new char[20];
					sprintf(item, "%s", comps[i]->m_MeshComp->m_Mesh->m_Name.c_str());
					items[i] = item;
				}
				ImGui::Combo("Meshes", &curItem, items, comps.size());
				for (int i = 0; i < comps.size(); i++)
				{
					delete[] items[i];
				}
				delete[] items;
				return curItem;
			};

			comps[getIdx()]->m_Material->RenderConfigGUI();
		}
		ImGui::End();
	}
};