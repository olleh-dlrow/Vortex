#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/SceneImporter.h>
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>
#include <Vortex/Renderer/Batch.h>
#include <Vortex/Scene/SkyboxRendererComponent.h>

using Vortex::Ref;
using Vortex::CreateRef;
using Vortex::ViewportWindow;
using Vortex::Camera;
using Vortex::Renderer;
using Vortex::Shader;
using Vortex::VertexArray;
using Vortex::VertexBuffer;
using Vortex::Cube4;
using Vortex::Cubemap;

using Vortex::Texture;
using Vortex::Texture2D;
using Vortex::Mesh;
using Vortex::MeshComponent;
using Vortex::MeshRendererComponent;
using Vortex::Material;
using Vortex::Scene;
using Vortex::Entity;
using Vortex::SkyboxRendererComponent;

Ref<Shader> shader;

Ref<VertexArray> cubeVA;
Ref<VertexBuffer> cubeVB;

SkyboxRendererComponent* sbr;
class CubemapTest : public EditorLayer
{
public:
    CubemapTest()
    {
        auto scene = GetEditorScene();
        auto skyEnt = scene->AddEntity("skybox");
        sbr = skyEnt->AddComponent<SkyboxRendererComponent>();

        // set camera position and mode
        Camera& cam = GetCamera();
        cam.SetProjectionMode(false);
        cam.m_Position = glm::vec3(0, 0, 5);
        cam.m_MovementSpeed = 2.0f;

        shader = Shader::Create("assets/shaders/CubemapTest2.glsl");        // cubemap

        // init cube
        cubeVA = VertexArray::Create();
        cubeVA->Bind();
        cubeVB = VertexBuffer::Create(Cube4::GetVertices(), Cube4::GetVerticesSize());
        cubeVB->SetLayout(Cube4::VertexType::GetLayout());
        cubeVA->AddVertexBuffer(cubeVB);

        std::vector<std::string> faces
        {
            "assets/textures/skybox1/right.jpg",
            "assets/textures/skybox1/left.jpg",
            "assets/textures/skybox1/top.jpg",
            "assets/textures/skybox1/bottom.jpg",
            "assets/textures/skybox1/front.jpg",
            "assets/textures/skybox1/back.jpg",
        };
        sbr->SetCubemapTextures(faces);

        //cubemap = Cubemap::Create(faces);
    }

    virtual void OnUpdate(Vortex::Timestep ts) override
    {
        EditorLayer::OnUpdate(ts);

        Camera& camera = GetCamera();

        // render cube
        shader->Bind();
        shader->SetFloat3("cameraPos", camera.m_Position);
        glm::mat4 model = glm::mat4(1.0f);
        shader->SetMat4("model", model);
        shader->SetMat4("view", camera.GetViewMatrix());
        shader->SetMat4("projection", camera.GetProjMatrix());
        shader->SetInt("skybox", 0);
        cubeVA->Bind();
        sbr->GetCubemap().Bind();
        Renderer::DrawTriangles(cubeVA, Vortex::DrawTriangleConfig(Cube4::GetVertexCount(), 0));
    }

    virtual void OnPostProcess(Vortex::Texture2D& renderTexture) override
    {

    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Debug");
        {

        }
        ImGui::End();
    }
};