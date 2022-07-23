#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>
#include <Vortex/Scene/SkyboxRendererComponent.h>
#include <Vortex/Renderer/Texture.h>

using Vortex::Ref;
using Vortex::CreateRef;
using Vortex::ViewportWindow;
using Vortex::Camera;
using Vortex::Renderer;
using Vortex::Shader;
using Vortex::VertexArray;
using Vortex::VertexBuffer;
using Vortex::Cube4;

using Vortex::Texture;
using Vortex::Texture2D;
using Vortex::Mesh;
using Vortex::MeshComponent;
using Vortex::MeshRendererComponent;
using Vortex::Material;
using Vortex::Scene;
using Vortex::Entity;
using Vortex::Cubemap;

glm::vec3 lightPositions[] = {
    glm::vec3(-10.0f,  10.0f, 10.0f),
    glm::vec3(10.0f,  10.0f, 10.0f),
    glm::vec3(-10.0f, -10.0f, 10.0f),
    glm::vec3(10.0f, -10.0f, 10.0f),
};
glm::vec3 lightColors[] = {
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f)
};

glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 captureViews[] =
{
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};

int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;

// shaders
Ref<Material> pbrMat;
Ref<Material> equirectangularToCubemapMat;
Ref<Material> irradianceMat;
Ref<Material> prefilterMat;
Ref<Material> brdfMat;
Ref<Material> backgroundMat;

// textures
Ref<Texture> hdrTexture;
Ref<Cubemap> envCubemap;

class IBLTest : public EditorLayer
{
public:
    std::vector<MeshRendererComponent*> comps;
    Vortex::SkyboxRendererComponent* sbr;

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
            mats[i]->m_Shader = Shader::Create("assets/shaders/PBRLightTest.glsl");

            // set all meshes' material the same in convenience to adjust parameters
            mr->m_Material = mats[i];

            // init mat
            auto& mat = *mats[i];
            mat.SetFloat3("albedo", { 0.4f, 1.0f, 0.9f });
            mat.SetFloat("ao", 1.0f);
            mat.SetFloat("roughness", 0.3f);
            mat.SetFloat("metallic", 0.0f);

            // set config material callback
            auto callback = [this](Material& mat)
            {
                mat.SetFloat3("camPos", GetCamera().m_Position);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::scale(model, glm::vec3(0.1f));
                mat.SetMat4("model", model);
                mat.SetMat4("view", GetCamera().GetViewMatrix());
                mat.SetMat4("projection", GetCamera().GetProjMatrix());

                for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
                {
                    glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(Vortex::Time::GetSeconds() * 5.0) * 5.0, 0.0, 0.0);
                    newPos = lightPositions[i];
                    mat.SetFloat3("lightPositions[" + std::to_string(i) + "]", newPos);
                    mat.SetFloat3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
                }

            };

            mr->m_ConfigMatCallback = callback;
            // add to comps list
            comps.push_back(mr);
        }
    }

    void InitMaterials()
    {
        pbrMat->m_Shader = Shader::Create("assets/shaders/IBL/pbr.glsl");
        equirectangularToCubemapMat->m_Shader = Shader::Create("assets/shaders/IBL/equirectangular_to_cubemap.glsl");
        irradianceMat->m_Shader = Shader::Create("assets/shaders/IBL/irradiance_convolution.glsl");
        prefilterMat->m_Shader = Shader::Create("assets/shaders/IBL/prefilter.glsl");
        brdfMat->m_Shader = Shader::Create("assets/shaders/IBL/brdf.glsl");
        backgroundMat->m_Shader = Shader::Create("assets/shaders/IBL/background.glsl");

        // set gold material
        pbrMat->SetTexture("albedoMap", Texture2D::Create("assets/textures/gold/albedo.png"));
        pbrMat->SetTexture("normalMap", Texture2D::Create("assets/textures/gold/normal.png"));
        pbrMat->SetTexture("metallicMap", Texture2D::Create("assets/textures/gold/metallic.png"));
        pbrMat->SetTexture("roughnessMap", Texture2D::Create("assets/textures/gold/roughness.png"));
        pbrMat->SetTexture("aoMap", Texture2D::Create("assets/textures/gold/ao.png"));
    }

    void LoadHDREnvmap()
    {
        hdrTexture = Texture2D::Create("assets/textures/hdr/newport_loft.hdr", false, true);
        //envCubemap = Cubemap::Create();
    }

    IBLTest()
    {
        // set camera position and mode
        Camera& cam = GetCamera();
        cam.SetProjectionMode(false);
        cam.m_Position = glm::vec3(0, 0, 3);
        cam.m_MovementSpeed = 2.0f;
        //cam.m_Pitch = -26;
        //cam.ReCalculateCameraVectors();

        //InitMaterials();

        std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
        LoadMeshes(filename);

        auto scene = GetEditorScene();
        auto skyEnt = scene->AddEntity("skybox");
        sbr = skyEnt->AddComponent<Vortex::SkyboxRendererComponent>();
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

        uint32_t id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 512, 1024, 0, GL_RGB, GL_FLOAT, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 0, 1024, 0, GL_RGBA, GL_FLOAT, NULL);

        

        //glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
        //glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 1024, 1024, GL_TRUE);
    }

    virtual void OnAttach() override
    {
        Vortex::Application::Get().GetWindow().GetGraphicsContext().SetMSAA(true);
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
            ImGui::Text("Meshes");
            for (auto& c : comps)
            {
                bool v = c->GetEnableValue();
                if (ImGui::Checkbox(c->m_MeshComp->m_Mesh->m_Name.c_str(), &v))
                {
                    c->SetEnableValue(v);
                }
            }

            bool v = sbr->GetEnableValue();
            if (ImGui::Checkbox("Skybox", &v))
            {
                sbr->SetEnableValue(v);
            }

            auto getIdx = [&]()->int {
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