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

SkyboxRendererComponent* sbr;
std::vector<MeshRendererComponent*> comps;

class CubemapTest : public EditorLayer
{
public:
    CubemapTest()
    {
        std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";

        Vortex::SceneImporter sceneImporter(filename.c_str());
        VT_INFO(sceneImporter.GetNodesInfo());
        VT_INFO(sceneImporter.GetMeshesInfo());

        std::vector<Ref<Mesh>> meshes;
        std::vector <Ref<Material>> mats;
        sceneImporter.ImportMeshAndMat(meshes, mats);

        auto scene = GetEditorScene();
        auto skyEnt = scene->AddEntity("skybox");
        sbr = skyEnt->AddComponent<SkyboxRendererComponent>();

        // set camera position and mode
        Camera& cam = GetCamera();
        cam.SetProjectionMode(false);
        cam.m_Position = glm::vec3(0, 0, 10);
        cam.m_MovementSpeed = 2.0f;

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
            mats[i]->m_Shader = Shader::Create("assets/shaders/CubemapTest.glsl");
            // set all meshes' material the same in convenience to adjust parameters
            mr->m_Material = mats[i];

            // set config material callback
            auto callback = [this](Material& mat)
            {
                mat.SetFloat3("cameraPos", GetCamera().m_Position);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::scale(model, glm::vec3(0.1f));
                mat.SetMat4("model", model);
                mat.SetMat4("view", GetCamera().GetViewMatrix());
                mat.SetMat4("projection", GetCamera().GetProjMatrix());
                mat.SetInt("skybox", 0);
                sbr->GetCubemap().Bind();
            };

            mr->m_ConfigMatCallback = callback;
            // add to comps list
            comps.push_back(mr);
        }

        std::vector<std::string> faces
        {
            "assets/textures/skycube_1/skyrender0002.bmp",
            "assets/textures/skycube_1/skyrender0005.bmp",
            "assets/textures/skycube_1/skyrender0003.bmp",
            "assets/textures/skycube_1/skyrender0006.bmp",
            "assets/textures/skycube_1/skyrender0001.bmp",
            "assets/textures/skycube_1/skyrender0004.bmp",
        };

        sbr->SetCubemapTextures(faces);
    }

    virtual void OnUpdate(Vortex::Timestep ts) override
    {
        EditorLayer::OnUpdate(ts);
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