#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>
#include <Vortex/Scene/SkyboxRendererComponent.h>
#include <Vortex/Renderer/Texture.h>
#include <Vortex/Renderer/VertexArray.h>
using namespace Vortex;

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void RenderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

class PointShadowTest : public EditorLayer {
public:
    std::vector<MeshRendererComponent*> comps;
    Ref<Shader> woodShader;
    Ref<Shader> simpleDepthShader;
    Ref<Texture2D> woodTexture;
    Ref<FrameBuffer> depthMapFB;
    Ref<Cubemap> depthCubemap;
    glm::vec3 lightPos;

    int shadowWidth = 1024, shadowHeight = 1024;

    //void RenderCube()
    //{
    //    static Ref<VertexArray> cubeVA = nullptr;
    //    static Ref<VertexBuffer> cubeVB = nullptr;
    //    if (cubeVA == nullptr)
    //    {
    //        cubeVA = VertexArray::Create();
    //        cubeVA->Bind();
    //        cubeVB = VertexBuffer::Create(Cube4::GetVertices(), Cube4::GetVerticesSize());
    //        cubeVB->SetLayout(Cube4::VertexType::GetLayout());
    //        cubeVA->AddVertexBuffer(cubeVB);
    //        cubeVA->Unbind();
    //        cubeVB->Unbind();
    //    }
    //    cubeVA->Bind();
    //    Renderer::DrawTriangles(cubeVA, Vortex::DrawTriangleConfig(36, 0));
    //    //cubeVA->Unbind();
    //}

    void RenderScene(Shader& shader) 
    {
        // room
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(5.0f));
        shader.SetMat4("model", model);
        Renderer::SetCullFace(false);
        shader.SetInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
        RenderCube();
        shader.SetInt("reverse_normals", 0); // and of course disable it
        Renderer::SetCullFace(true);
        // cubes
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.SetMat4("model", model);
        RenderCube();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.75f));
        shader.SetMat4("model", model);
        RenderCube();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.SetMat4("model", model);
        RenderCube();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.SetMat4("model", model);
        RenderCube();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(0.75f));
        shader.SetMat4("model", model);
        RenderCube();
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

            // set all meshes' material the same in convenience to adjust parameters
            mr->m_Material = mats[i];
            // add to comps list
            comps.push_back(mr);
        }
    }

    PointShadowTest() 
    {
        // set camera position and mode
        Camera& cam = GetCamera();
        cam.SetProjectionMode(false);
        cam.m_Position = glm::vec3(0, 2.9, 5);
        cam.m_MovementSpeed = 2.0f;
        cam.m_Pitch = -26;
        cam.ReCalculateCameraVectors();

        // open depth test
        Renderer::SetDepthTest(true);
        Renderer::SetCullFace(true);

        // init shader
        woodShader = Shader::Create("assets/shaders/point_shadows.glsl");
        simpleDepthShader = Shader::Create("assets/shaders/point_shadow_depth.glsl");

        // load texture
        woodTexture = Texture2D::Create("assets/textures/wood.png");

        // set depth map framebuffer
        depthMapFB = FrameBuffer::Create();
        depthCubemap = Cubemap::Create(shadowWidth, shadowHeight, "DEPTH");
        depthCubemap->SetTextureFilterMode(TextureFilterOperation::MINIFY, TextureFilterMode::NEAREST);
        depthCubemap->SetTextureFilterMode(TextureFilterOperation::MAGNIFY, TextureFilterMode::NEAREST);

        depthCubemap->SetTextureWrapMode(TextureWrapAxis::S, TextureWrapMode::CLAMP_TO_EDGE);
        depthCubemap->SetTextureWrapMode(TextureWrapAxis::T, TextureWrapMode::CLAMP_TO_EDGE);
        depthCubemap->SetTextureWrapMode(TextureWrapAxis::R, TextureWrapMode::CLAMP_TO_EDGE);
        depthCubemap->ApplySettings();

        // attach depth buffer
        depthMapFB->Bind();
        depthMapFB->AttachDepthBuffer(*depthCubemap);
        Renderer::SetDrawBufferMode(RDBufferMode::NONE);
        Renderer::SetReadBufferMode(RDBufferMode::NONE);
        depthMapFB->Unbind();

        // set shader
        woodShader->Bind();
        woodShader->SetInt("diffuseTexture", 0);
        woodShader->SetInt("depthMap", 1);

        // lighting info
        lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
        
        //std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
        //LoadMeshes(filename);
    }

    void OnUpdate(Timestep ts) override 
    {
        EditorLayer::OnUpdate(ts);

        lightPos.z = sin(Time::GetSeconds() * 0.5) * 3.0;

        Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Renderer::Clear();
        
        // 0. create depth cubemap transformation matrices
        // -----------------------------------------------
        float near_plane = 1.0f;
        float far_plane = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)shadowWidth / (float)shadowHeight, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        
        // 1. render scene to depth cubemap
        depthMapFB->Bind();
        Renderer::SetViewport(0, 0, shadowWidth, shadowHeight);
        Renderer::Clear(BitField::DEPTH_BUFFER_BIT);
        simpleDepthShader->Bind();
        for (unsigned int i = 0; i < 6; ++i)
            simpleDepthShader->SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        simpleDepthShader->SetFloat("far_plane", far_plane);
        simpleDepthShader->SetFloat3("lightPos", lightPos);
        RenderScene(*simpleDepthShader);
        //comps[0]->m_Material->m_Shader = simpleDepthShader;
        //comps[0]->m_ConfigMatCallback = [this](Material& _mat) {
        //    auto model = glm::mat4(1.0f);
        //    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
        //    model = glm::scale(model, glm::vec3(0.1f));
        //    _mat.SetMat4("model", model);
        //};
        //comps[0]->DrawMesh();
        depthMapFB->Unbind();

        m_ViewportWindow->BindTargetFrameBuffer();
        // 2. render scene as normal 
        int width = Application::Get().GetWindow().GetWidth();
        int height = Application::Get().GetWindow().GetHeight();
        Renderer::SetViewport(0, 0, width, height);
        Renderer::Clear();

        woodShader->Bind();
        woodShader->SetMat4("projection", GetCamera().GetProjMatrix());
        woodShader->SetMat4("view", GetCamera().GetViewMatrix());
        // set lighting uniforms
        woodShader->SetFloat3("lightPos", lightPos);
        woodShader->SetFloat3("viewPos", GetCamera().m_Position);
        bool shadows = true;
        woodShader->SetInt("shadows", shadows); // enable/disable shadows by pressing 'SPACE'
        woodShader->SetFloat("far_plane", far_plane);
        woodTexture->Bind(0);
        depthCubemap->Bind(1);
        
        RenderScene(*woodShader);
        //comps[0]->m_Material->m_Shader = woodShader;
        //comps[0]->m_ConfigMatCallback = [this](Material& _mat) {
        //    auto model = glm::mat4(1.0f);
        //    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
        //    model = glm::scale(model, glm::vec3(0.1f));
        //    _mat.SetMat4("model", model);
        //};
    }
};