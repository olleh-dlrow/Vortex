#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/SceneImporter.h>
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>
#include <stb_image.h>

using Vortex::Ref;
using Vortex::CreateRef;
using Vortex::ViewportWindow;
using Vortex::Camera;
using Vortex::Renderer;
using Vortex::Shader;
using Vortex::VertexArray;
using Vortex::VertexBuffer;

using Vortex::Texture;
using Vortex::Texture2D;
using Vortex::Mesh;
using Vortex::MeshComponent;
using Vortex::MeshRendererComponent;
using Vortex::Material;
using Vortex::Scene;
using Vortex::Entity;
using Vortex::Cube4;

std::vector<glm::vec3> lightPositions;
std::vector<glm::vec3> lightColors;
Ref<Texture> woodtexture;
Ref<Shader> shader;

Ref<VertexArray> VA;
Ref<VertexBuffer> VB;

class HDRTest : public EditorLayer
{
public:
	HDRTest()
	{
		// set camera position and mode
		Camera& cam = GetCamera();
		cam.SetProjectionMode(false);
		cam.m_Position = glm::vec3(0, 0, 0);
		cam.m_MovementSpeed = 2.0f;
        cam.m_Yaw = 90.0f;
        cam.ReCalculateCameraVectors();

        // Light sources
        // - Positions
        lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
        lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
        lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
        lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
        // - Colors
        lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
        lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
        lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
        lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

        woodtexture = Texture2D::Create("assets/textures/wood.png", true);
        shader = Shader::Create("assets/shaders/HDRTest.glsl");
        
        // init cube
        VA = VertexArray::Create();
        VA->Bind();
        int v = Cube4::GetVerticesSize();
        VB = VertexBuffer::Create(Cube4::GetVertices(), Cube4::GetVerticesSize());
        VB->Bind();
        VB->SetLayout(Cube4::VertexType::GetLayout());
        VA->AddVertexBuffer(VB);
        VB->Unbind();
        VA->Unbind();
	}

	virtual void OnUpdate(Vortex::Timestep ts) override
	{
		EditorLayer::OnUpdate(ts);

        Camera& cam = GetCamera();
        shader->Bind();
        shader->SetMat4("u_ViewProjection", cam.GetViewProjMatrix());

        woodtexture->Bind();
        for (int i = 0; i < lightPositions.size(); i++)
        {
            shader->SetFloat3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
            shader->SetFloat3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        }
        shader->SetFloat3("viewPos", cam.m_Position);

        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 55.0f));
        shader->SetMat4("model", model);
        shader->SetInt("diffuseTexture", 0);
        shader->SetInt("inverse_normals", 1);

        // render cube
        VA->Bind();
        Renderer::DrawTriangles(VA, Vortex::DrawTriangleConfig(Cube4::GetVertexCount(), 0));
	}

	virtual void OnPostProcess(Vortex::Texture2D& renderTexture) override
	{

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		{
            for (int i = 0; i < lightColors.size(); i++)
            {
                ImGui::DragFloat3(("light" + std::to_string(i)).c_str(), (float*)&lightColors[i]);
            }
		}
		ImGui::End();
	}
};