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
using Vortex::Quad5;

using Vortex::Texture;
using Vortex::Texture2D;
using Vortex::Mesh;
using Vortex::MeshComponent;
using Vortex::MeshRendererComponent;
using Vortex::Material;
using Vortex::Scene;
using Vortex::Entity;
using Vortex::Cubemap;
using Vortex::FrameBuffer;
using Vortex::RenderBuffer;

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
Ref<Texture2D> hdrTexture;
Ref<Cubemap> envCubemap;
Ref<Cubemap> irradianceMap;
Ref<Cubemap> prefilterMap;
Ref<Texture2D> brdfLUTTexture;

// framebuffer, renderbuffer
Ref<FrameBuffer> captureFB;
Ref<RenderBuffer> captureRB;

// cube
Ref<VertexArray> cubeVA = nullptr;
Ref<VertexBuffer> cubeVB;

// quad
Ref<VertexArray> quadVA = nullptr;
Ref<VertexBuffer> quadVB;

class IBLTest : public EditorLayer
{
public:
    std::vector<MeshRendererComponent*> comps;
    Vortex::SkyboxRendererComponent* sbr;

    void RenderCube()
    {
        if (cubeVA == nullptr)
        {
            cubeVA = VertexArray::Create();
            cubeVA->Bind();
            cubeVB = VertexBuffer::Create(Cube4::GetVertices(), Cube4::GetVerticesSize());
            cubeVB->SetLayout(Cube4::VertexType::GetLayout());
            cubeVA->AddVertexBuffer(cubeVB);
            cubeVA->Unbind();
            cubeVB->Unbind();
        }
        cubeVA->Bind();
        Renderer::DrawTriangles(cubeVA, Vortex::DrawTriangleConfig(36, 0));
        cubeVA->Unbind();
    }

    void RenderQuad()
    {
        if (quadVA == nullptr)
        {
            quadVA = VertexArray::Create();
            quadVA->Bind();
            quadVB = VertexBuffer::Create(Quad5::GetVertices(), Quad5::GetVerticesSize());
            quadVB->SetLayout(Quad5::VertexType::GetLayout());
            quadVA->AddVertexBuffer(quadVB);
            quadVA->Unbind();
            quadVB->Unbind();
        }
        quadVA->Bind();
        Renderer::DrawTriangles(quadVA, Vortex::DrawTriangleConfig(4, 0, Vortex::PolygonMode::FILL, Vortex::PrimitiveType::TRIANGLE_STRIP));
        quadVA->Unbind();
    }

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

            mats[i] = pbrMat;

            // set all meshes' material the same in convenience to adjust parameters
            mr->m_Material = mats[i];

            // init mat
            auto& mat = *mats[i];

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
        pbrMat = CreateRef<Material>("pbrMat");
        equirectangularToCubemapMat = CreateRef<Material>("equirectangularToCubemapMat");
        irradianceMat = CreateRef<Material>("irradianceMat");
        prefilterMat = CreateRef<Material>("prefilterMat");
        brdfMat = CreateRef<Material>("brdfMat");
        backgroundMat = CreateRef<Material>("backgroundMat");

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

    void CreateHDREnvmap()
    {
        // pbr: setup framebuffer
        captureFB = FrameBuffer::Create();
        captureRB = RenderBuffer::Create(512, 512);
        captureFB->AttachRenderBuffer(captureRB);
  
        // pbr: load the HDR environment map
        hdrTexture = Texture2D::Create("assets/textures/hdr/newport_loft.hdr");
        glTextureParameteri(hdrTexture->GetID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // WARNING: this place should be GL_LINEAR, not GL_NEAREST!!!
        glTextureParameteri(hdrTexture->GetID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(hdrTexture->GetID(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(hdrTexture->GetID(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // pbr: setup cubemap to render to and attach to framebuffer
        envCubemap = Cubemap::Create(512, 512, "RGB16F");
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // pbr: convert HDR equirectangular environment map to cubemap equivalent
        equirectangularToCubemapMat->SetMat4("projection", captureProjection);
        equirectangularToCubemapMat->SetTexture("equirectangularMap", hdrTexture);

        Renderer::SetViewport(0, 0, 512, 512);
        captureFB->Bind();
        for (int i = 0; i < 6; i++)
        {
            equirectangularToCubemapMat->SetMat4("view", captureViews[i]);
            captureFB->AttachCubemap(*envCubemap, 0, i);
            Renderer::Clear();
            equirectangularToCubemapMat->ApplyProperties();
            RenderCube();
        }
        captureFB->Unbind();
        envCubemap->Bind();
        envCubemap->GenerateMipmaps();
    }

    void CreateIrradianceMap()
    {
        // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
        irradianceMap = Cubemap::Create(32, 32, "RGB16F");
        irradianceMap->Bind();
        // temporarily use the raw API
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        irradianceMap->Unbind();

        captureRB = RenderBuffer::Create(32, 32);
        captureFB->Bind();
        captureFB->AttachRenderBuffer(captureRB);

        // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
        irradianceMat->SetMat4("projection", captureProjection);
        irradianceMat->SetTexture("environmentMap", envCubemap);
        Renderer::SetViewport(0, 0, 32, 32);
        captureFB->Bind();
        for (int i = 0; i < 6; i++)
        {
            irradianceMat->SetMat4("view", captureViews[i]);
            captureFB->AttachCubemap(*irradianceMap, 0, i);
            Renderer::Clear();
            irradianceMat->ApplyProperties();
            RenderCube();
        }
        captureFB->Unbind();
    }

    void CreatePrefilterMap()
    {
        // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
        prefilterMap = Cubemap::Create(128, 128, "RGB16F");
        // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
        prefilterMap->GenerateMipmaps();
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
        prefilterMat->SetMat4("projection", captureProjection);
        prefilterMat->SetTexture("environmentMap", envCubemap);
        captureFB->Bind();
        int maxMipLevels = 5;
        for (int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = 128 * std::pow(0.5, mip);
            unsigned int mipHeight = 128 * std::pow(0.5, mip);
            captureRB = RenderBuffer::Create(mipWidth, mipHeight);
            captureFB->AttachRenderBuffer(captureRB);

            Renderer::SetViewport(0, 0, mipWidth, mipHeight);
            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterMat->SetFloat("roughness", roughness);
            for (int i = 0; i < 6; i++)
            {
                prefilterMat->SetMat4("view", captureViews[i]);
                captureFB->AttachCubemap(*prefilterMap, 0, i);
                Renderer::Clear();
                prefilterMat->ApplyProperties();
                RenderCube();
            }
        }
        captureFB->Unbind();
    }

    void Create2DLUT()
    {
        // pbr: generate a 2D LUT from the BRDF equations used.
        brdfLUTTexture = Texture2D::Create(512, 512, "RG16F");
        // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
        captureRB = RenderBuffer::Create(512, 512);
        captureFB->Bind();
        captureFB->AttachRenderBuffer(captureRB);
        captureFB->AttachTexture2D(*brdfLUTTexture, 0);

        Renderer::SetViewport(0, 0, 512, 512);
        brdfMat->ApplyProperties();
        Renderer::Clear();
        RenderQuad();
        captureFB->Unbind();
    }

    IBLTest()
    {
        // set camera position and mode
        Camera& cam = GetCamera();
        cam.SetProjectionMode(false);
        cam.m_Position = glm::vec3(0, 2.9, 5);
        cam.m_MovementSpeed = 2.0f;
        cam.m_Pitch = -26;
        cam.ReCalculateCameraVectors();

        InitMaterials();
        CreateHDREnvmap();
        CreateIrradianceMap();
        CreatePrefilterMap();
        Create2DLUT();

        std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
        LoadMeshes(filename);
    }

    virtual void OnAttach() override
    {
        
    }

    virtual void OnUpdate(Vortex::Timestep ts) override
    {
        EditorLayer::OnUpdate(ts);

        Renderer::SetDepthPassCond(Vortex::DepthPassCond::LEQUAL);
        backgroundMat->SetMat4("projection", GetCamera().GetProjMatrix());
        backgroundMat->SetMat4("view", GetCamera().GetViewMatrix());
        backgroundMat->SetTexture("environmentMap", envCubemap);
        backgroundMat->ApplyProperties();
        RenderCube();
        Renderer::SetDepthPassCond(Vortex::DepthPassCond::LESS);
    }

    virtual void PostUpdate(Vortex::Timestep ts) override
    {
        EditorLayer::PostUpdate(ts);
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