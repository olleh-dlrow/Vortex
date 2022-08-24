#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>
#include <Vortex/Scene/SkyboxRendererComponent.h>
#include <Vortex/Renderer/Texture.h>
#include <Vortex/Renderer/VertexArray.h>
using namespace Vortex;

float planeVertices[] = {
    // positions            // normals         // texcoords
     25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
    -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

     25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
     25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
};

glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

Ref<VertexArray> planeVA;

Ref<Texture2D> woodTexture;

Ref<FrameBuffer> depthMapFB;
Ref<Texture2D> depthMap;

Ref<Shader> shader;
Ref<Shader> simpleDepthShader;

glm::mat4 lightProjection, lightView;
glm::mat4 lightSpaceMatrix;

class ShadowTest : public EditorLayer {
public:
    std::vector<MeshRendererComponent*> comps;
    int shadowWidth = 1024, shadowHeight = 1024;

    void LoadMeshes(const std::string& filename)
    {
        Vortex::SceneImporter sceneImporter(filename.c_str());
        VT_INFO(sceneImporter.GetNodesInfo());
        VT_INFO(sceneImporter.GetMeshesInfo());

        std::vector<Ref<Mesh>> meshes;
        std::vector <Ref<Material>> mats;
        sceneImporter.ImportMeshAndMat(meshes, mats);

        auto scene = GetEditorScene();
        // create entity, init mesh component and mesh renderercomponent
        for (int i = 0; i < meshes.size(); i++)
        {
            Mesh* mesh = meshes[i].get();

            auto e = scene->AddEntity("Entity" + std::to_string(i));

            auto mc = e->AddComponent<MeshComponent>();
            mc->m_Mesh = meshes[i];

            // init mesh renderer comp, set shader, mat, mesh
            auto mr = e->AddComponent<MeshRendererComponent>();
            mr->SetMeshComponent(mc);

            // set all meshes' material the same in convenience to adjust parameters
            mr->m_Material = mats[i];
            // add to comps list
            comps.push_back(mr);
        }
    }

	ShadowTest() {
        // set camera position and mode
        Camera& cam = GetCamera();
        cam.SetProjectionMode(false);
        cam.m_Position = glm::vec3(0, 2.9, 5);
        cam.m_MovementSpeed = 2.0f;
        cam.m_Pitch = -26;
        cam.ReCalculateCameraVectors();

        // create wood texture
        woodTexture = Texture2D::Create("assets/textures/wood.png");

        // init plane
        planeVA = VertexArray::Create();
        auto planeVB = VertexBuffer::Create(planeVertices, sizeof(planeVertices));
        planeVB->SetLayout(BufferLayout{
            {ShaderDataType::Float3, "Position"},
            {ShaderDataType::Float3, "Normal"},
            {ShaderDataType::Float2, "Texcoords"}
        });
        planeVA->AddVertexBuffer(planeVB);

        // load mesh
        std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
        LoadMeshes(filename);

        // create depth map
        depthMapFB = FrameBuffer::Create();
        depthMap = Texture2D::Create(shadowWidth, shadowHeight, "DEPTH");
        depthMap->SetTextureFilterMode(TextureFilterOperation::MINIFY, TextureFilterMode::NEAREST);
        depthMap->SetTextureFilterMode(TextureFilterOperation::MAGNIFY, TextureFilterMode::NEAREST);

        depthMap->SetTextureWrapMode(TextureWrapAxis::S, TextureWrapMode::CLAMP_TO_BORDER);
        depthMap->SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::CLAMP_TO_BORDER);
        depthMap->SetBorderColor({1.0, 1.0, 1.0, 1.0});
        depthMap->ApplySettings();
	
        // attach depth texture as FBO's depth buffer
        depthMapFB->Bind();
        depthMapFB->AttachDepthBuffer(*depthMap);
        Renderer::SetDrawBufferMode(RDBufferMode::NONE);
        Renderer::SetReadBufferMode(RDBufferMode::NONE);
        depthMapFB->Unbind();

        // create shader
        shader = Shader::Create("assets/shaders/shadow_mapping.glsl");
        simpleDepthShader = Shader::Create("assets/shaders/shadow_mapping_depth.glsl");
    
        // config shader
        shader->Bind();
        shader->SetInt("diffuseTexture", 0);
        shader->SetInt("shadowMap", 1);


        // 1. render depth of scene to texture (from light's perspective)

        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader->Bind();
        simpleDepthShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

        Renderer::SetViewport(0, 0, shadowWidth, shadowHeight);
        depthMapFB->Bind();

        Renderer::Clear(BitField::DEPTH_BUFFER_BIT);
        simpleDepthShader->Bind();
        woodTexture->Bind(0);
        // floor
        glm::mat4 model = glm::mat4(1.0f);
        simpleDepthShader->SetMat4("model", model);
        planeVA->Bind();
        Renderer::DrawTriangles(planeVA, DrawTriangleConfig(6, 0));
        // mesh
        comps[0]->m_Material->m_Shader = simpleDepthShader;
        comps[0]->m_ConfigMatCallback = [this](Material& _mat) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
            model = glm::scale(model, glm::vec3(0.1f));
            _mat.SetMat4("model", model);
        };
        comps[0]->DrawMesh();

        depthMapFB->Unbind();
    }

	void OnUpdate(Timestep ts) override {
        // reset viewport
        int width = Application::Get().GetWindow().GetWidth();
        int height = Application::Get().GetWindow().GetHeight();
        Renderer::SetViewport(0, 0, width, height);
        Renderer::Clear();

        // 2. render scene as normal using the generated depth/shadow map  
        shader->Bind();
        shader->SetMat4("projection", GetCamera().GetProjMatrix());
        shader->SetMat4("view", GetCamera().GetViewMatrix());
        // set light uniforms
        shader->SetFloat3("viewPos", GetCamera().m_Position);
        shader->SetFloat3("lightPos", lightPos);
        shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
        woodTexture->Bind(0);
        depthMap->Bind(1);
        // floor
        auto model = glm::mat4(1.0f);
        shader->SetMat4("model", model);
        planeVA->Bind();
        Renderer::DrawTriangles(planeVA, DrawTriangleConfig(6, 0));

        // mesh
        comps[0]->m_Material->m_Shader = shader;
        comps[0]->m_ConfigMatCallback = [this](Material& _mat) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
            model = glm::scale(model, glm::vec3(0.1f));
            _mat.SetMat4("model", model);
        };
    }
};