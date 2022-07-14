#pragma once

#include "EditorLayer.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <Vortex/Scene/SceneImporter.h>
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>

using Vortex::Ref;
using Vortex::CreateRef;
using Vortex::ViewportWindow;
using Vortex::VertexArray;
using Vortex::VertexBuffer;
using Vortex::BufferLayout;
using Vortex::Camera;
using Vortex::OrthoParam;
using Vortex::Renderer;
using Vortex::DrawPointConfig;
using Vortex::DrawLineConfig;
using Vortex::IndexBuffer;
using Vortex::Ray;
using Vortex::Shader;

using Vortex::Vertex1;
using Vortex::Triangle1;
using Vortex::Batch;

using Vortex::Mesh;
using Vortex::MeshComponent;
using Vortex::MeshRendererComponent;
using Vortex::Material;
using Vortex::Scene;
using Vortex::Entity;

class ModelImportTest: public EditorLayer 
{
	Ref<Batch<Triangle1>> batch;
	Ref<Shader> shader;
	aiMesh* mesh;

	std::vector<MeshRendererComponent*> comps;
	std::unordered_map<std::string, bool> visible;
public:
	ModelImportTest()
	{
		visible = {
			{"Cube_002", true},
			{"Gun", true},
			{"Gun_trigger", true},
			{"muzzle_fire", true},
			{"Cube_005", true},

		};

		std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
		std::string filename2 = "assets/models/45-acp-smith-and-wesson-with-animation-obj/45-acp-smith-and-wesson-with-animation.obj";
		
		Vortex::SceneImporter sceneImporter(filename2.c_str());
		VT_INFO(sceneImporter.GetNodesInfo());
		VT_INFO(sceneImporter.GetMeshesInfo());

		std::vector<Ref<Mesh>> meshes;
		std::vector <Ref<Material>> mats;
		sceneImporter.ImportMeshAndMat(meshes, mats);

		// create shader and set value
		shader = Shader::Create("assets/shaders/Mesh.glsl");

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
				//if (mesh->m_Name == "Gun_trigger")
				//	vert.color = glm::vec4(1, 0, 0, 1);
				vert.color = colors[i];
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
			auto callback = [this](const Material& mat)
			{
				auto& shader = mat.m_Shader;
				shader->SetMat4("u_ViewProjection", GetCamera().GetViewProjMatrix());
				shader->SetMat4("u_Transform", glm::identity<glm::mat4>());
			};

			mr->m_ConfigMatCallback = callback;

			// add to comps list
			comps.push_back(mr);
		}
	}

	virtual void OnUpdate(Vortex::Timestep ts) override
	{
		for (auto mr : comps)
		{
			// mr->m_DrawCfg.polygonMode = GL_LINE;
			// if (mr->m_MeshComp->m_Mesh->m_Name != "Cube_002")continue;
			for (auto& p : visible)
			{
				if(mr->m_MeshComp->m_Mesh->m_Name == p.first && p.second)
					mr->DrawMesh();
			}
		}
	}

	void OutputMeshesInfo(const aiScene* scene)
	{
		// get mesh info
		if (scene->HasMeshes())
		{
			VT_INFO("Mesh Count: {}", scene->mNumMeshes);
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				auto curMesh = scene->mMeshes[i];
				VT_INFO("\tMesh name: {}", curMesh->mName.C_Str());
				if (curMesh->HasPositions())
				{
					VT_INFO("\tVertices Count: {}", curMesh->mNumVertices);
				}
				if (curMesh->HasNormals())
				{
					VT_INFO("\tHas Normals");
				}
				if (curMesh->HasFaces())
				{
					VT_INFO("\tFaces Count: {}", curMesh->mNumFaces);
				}
				if (curMesh->HasTextureCoords(0))
				{
					VT_INFO("Has TextureCoords");
					//VT_INFO("\tNormals Count: {}", curMesh->texture);
				}
			}
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		{
			for (auto& p : visible)
			{
				ImGui::Checkbox(p.first.c_str(), &p.second);
			}
		}
		ImGui::End();
	}

};