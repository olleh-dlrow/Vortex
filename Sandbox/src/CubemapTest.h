#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/SceneImporter.h>
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>
#include <Vortex/Renderer/Batch.h>
#include <stb_image.h>

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

Ref<Shader> shader;
Ref<Shader> SkyboxShader;

Ref<VertexArray> cubeVA;
Ref<VertexBuffer> cubeVB;

Ref<VertexArray> skyboxVA;
Ref<VertexBuffer> skyboxVB;
Ref<Cubemap>      cubemap;

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

class CubemapTest : public EditorLayer
{
public:
    CubemapTest()
    {
        // set camera position and mode
        Camera& cam = GetCamera();
        cam.SetProjectionMode(false);
        cam.m_Position = glm::vec3(0, 0, 5);
        cam.m_MovementSpeed = 2.0f;

        shader = Shader::Create("assets/shaders/CubemapTest2.glsl");        // cubemap
        SkyboxShader = Shader::Create("assets/shaders/CubemapTest1.glsl");   // skybox

        // init cube
        cubeVA = VertexArray::Create();
        cubeVA->Bind();
        cubeVB = VertexBuffer::Create(Cube4::GetVertices(), Cube4::GetVerticesSize());
        cubeVB->SetLayout(Cube4::VertexType::GetLayout());
        cubeVA->AddVertexBuffer(cubeVB);

        // init skybox
        skyboxVA = VertexArray::Create();
        skyboxVA->Bind();
        skyboxVB = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
        skyboxVB->SetLayout(Vortex::BufferLayout{
            { Vortex::ShaderDataType::Float3, "aPos"}
        });
        skyboxVA->AddVertexBuffer(skyboxVB);

        // load textures
        // -------------
        std::vector<std::string> faces
        {
            "assets/textures/skybox1/right.jpg",
            "assets/textures/skybox1/left.jpg",
            "assets/textures/skybox1/top.jpg",
            "assets/textures/skybox1/bottom.jpg",
            "assets/textures/skybox1/front.jpg",
            "assets/textures/skybox1/back.jpg",
        };

        cubemap = Cubemap::Create(faces);
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
        cubemap->Bind();
        Renderer::DrawTriangles(cubeVA, Vortex::DrawTriangleConfig(Cube4::GetVertexCount(), 0));

        // skybox
        glDepthFunc(GL_LEQUAL);
        SkyboxShader->Bind();
        auto view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        SkyboxShader->SetMat4("view", view);
        SkyboxShader->SetMat4("projection", camera.GetProjMatrix());
        SkyboxShader->SetInt("skybox", 0);
        skyboxVA->Bind();
        // skybox cube
        cubemap->Bind();
        Renderer::DrawTriangles(skyboxVA, Vortex::DrawTriangleConfig(36, 0));
        glDepthFunc(GL_LESS);
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