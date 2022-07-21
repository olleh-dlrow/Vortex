#pragma once

#include "EditorLayer.h"
#include <Vortex/Scene/SceneImporter.h>
#include <Vortex/Scene/MeshComponent.h>
#include <Vortex/Scene/MeshRendererComponent.h>
#include <Vortex/Renderer/Batch.h>
#include <GLFW/glfw3.h>

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

// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

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

Ref<Shader> shader;

int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;

class PBRLightTest : public EditorLayer
{
public:
    std::vector<MeshRendererComponent*> comps;

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
            mat.SetFloat3("albedo", { 0.5f, 0.0f, 0.0f });
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
                    glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
                    newPos = lightPositions[i];
                    mat.SetFloat3("lightPositions[" + std::to_string(i) + "]", newPos);
                    mat.SetFloat3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

                    //model = glm::mat4(1.0f);
                    //model = glm::translate(model, newPos);
                    //model = glm::scale(model, glm::vec3(0.5f));
                    //shader->SetMat4("model", model);
                    //renderSphere();
                }
                
            };

            mr->m_ConfigMatCallback = callback;
            // add to comps list
            comps.push_back(mr);
        }
    }

    PBRLightTest()
    {
        // set camera position and mode
        Camera& cam = GetCamera();
        cam.SetProjectionMode(false);
        cam.m_Position = glm::vec3(0, 2.9, 5);
        cam.m_MovementSpeed = 2.0f;
        cam.m_Pitch = -26;
        cam.ReCalculateCameraVectors();

        std::string filename = "assets/models/utah-teapot-obj/utah-teapot.obj";
        LoadMeshes(filename);

        shader = Shader::Create("assets/shaders/PBRLightTest.glsl");
    }

    virtual void OnAttach() override
    {
        Vortex::Application::Get().GetWindow().GetGraphicsContext().SetMSAA(true);
    }

    virtual void OnUpdate(Vortex::Timestep ts) override
    {
        EditorLayer::OnUpdate(ts);

        //Camera& camera = GetCamera();
        //shader->Bind();
        //shader->SetFloat3("albedo", { 0.5f, 0.0f, 0.0f });
        //shader->SetFloat("ao", 1.0f);
        //glm::mat4 view = camera.GetViewMatrix();
        //shader->SetMat4("view", view);
        //shader->SetFloat3("camPos", camera.m_Position);
        //shader->SetMat4("projection", camera.GetProjMatrix());

        //// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
        //glm::mat4 model = glm::mat4(1.0f);
        //for (int row = 0; row < nrRows; ++row)
        //{
        //    shader->SetFloat("metallic", (float)row / (float)nrRows);
        //    for (int col = 0; col < nrColumns; ++col)
        //    {
        //        // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
        //        // on direct lighting.
        //        shader->SetFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

        //        model = glm::mat4(1.0f);
        //        model = glm::translate(model, glm::vec3(
        //            (col - (nrColumns / 2)) * spacing,
        //            (row - (nrRows / 2)) * spacing,
        //            0.0f
        //        ));
        //        shader->SetMat4("model", model);
        //        renderSphere();
        //    }
        //}

        //// render light source (simply re-render sphere at light positions)
        //// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
        //// keeps the codeprint small.
        //for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        //{
        //    glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        //    newPos = lightPositions[i];
        //    shader->SetFloat3("lightPositions[" + std::to_string(i) + "]", newPos);
        //    shader->SetFloat3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, newPos);
        //    model = glm::scale(model, glm::vec3(0.5f));
        //    shader->SetMat4("model", model);
        //    renderSphere();
        //}
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