#pragma once

#include "EditorLayer.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

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

class ModelImportTest: public EditorLayer 
{
	Ref<Batch<Triangle1>> batch;
	Ref<Shader> shader;
	aiMesh* mesh;
public:
	ModelImportTest()
	{
#pragma region ReadScene
		std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
		// Create an instance of the Importer class
		Assimp::Importer importer;

		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll
		// probably to request more postprocessing than we do in this example.
		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		// If the import failed, report it
		if (nullptr == scene) {
			VT_ERROR(importer.GetErrorString());
			return;
		}
		
		OutputMeshesInfo(scene);
#pragma endregion
		
		// set camera position and mode
		Camera& cam = GetCamera();
		cam.SetProjectionMode(false);
		cam.Position() = glm::vec3(0, 0, 50);
		cam.MovementSpeed() = 12.5f;

		// get vertices list
		mesh = scene->mMeshes[0];
		batch = CreateRef<Batch<Triangle1>>(mesh->mNumFaces);
		glm::vec4 white = glm::vec4(1, 1, 1, 1);
		// set vertex's color
		// add vertex to batch
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace* face = mesh->mFaces + i;
			VT_CORE_ASSERT(face->mNumIndices == 3, "face not triangle!");
			std::array<glm::vec3, 3> pos;
			for (int j = 0; j < 3; j++)
			{
				int idx = face->mIndices[j];
				auto& position = mesh->mVertices[idx];
				pos[j] = glm::vec3(position.x, position.y, position.z);
			}
			Triangle1 tri(pos, { white, white, white });
			batch->TryAddBatchUnit(tri);
		}

		// create shader and set value
		shader = Shader::Create("assets/shaders/Vertex1.glsl");
		// mesh

		// mesh component
		
		// mesh renderer component

	}

	virtual void OnUpdate(Vortex::Timestep ts) override
	{
		batch->m_VertexBuffer->SetData(&batch->m_TempVertexBuffer[0], batch->GetTempBufferBatchUnitCount() * sizeof(Triangle1));
		Camera& cam = GetCamera();
		batch->m_VertexArray->Bind();
		shader->Bind();
		shader->SetMat4("u_ViewProjection", cam.GetViewProjMatrix());
		Vortex::DrawTriangleConfig attr(batch->GetTempBufferVertexCount(), 0);
		Renderer::DrawTriangles(batch->m_VertexArray, attr);
		// batch->m_FreeVertexBufferBaseIndex = 0;
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
			ImGui::DragInt("TempVertexCnt", &batch->m_FreeVertexBufferBaseIndex);
		}
		ImGui::End();
	}

protected:

};