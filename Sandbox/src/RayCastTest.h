#pragma once

#include <imgui.h>
#include <Vortex.h>
#include <Vortex/ImGui/ViewportWindow.h>
#include <Vortex/Geo/Ray.h>

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

static float vertices[] = {
 0.5f,  0.5f, 0.0f,  // top right
 0.5f, -0.5f, 0.0f,  // bottom right
-0.5f, -0.5f, 0.0f,  // bottom left
-0.5f,  0.5f, 0.0f,   // top left 
// 0.0f,  0.0f, 0.0f   // top left  
};
static unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};


static Vortex::Ref<Vortex::VertexArray> VA;
static Vortex::Ref<Vortex::Shader> shader;
static Vortex::Ref<Vortex::Shader> triShader;
static glm::vec3 color = glm::vec3(0.3f, 0.8f, 0.2f);
static ImVec2 worldPos;
static float ptSz = 10.0f;

class RayCastTest : public Vortex::Layer
{
public:
    RayCastTest()
    {
        VA = Vortex::VertexArray::Create();
        Vortex::Ref<Vortex::VertexBuffer> vertexBuffer = Vortex::VertexBuffer::Create(vertices, sizeof(vertices));
        Vortex::BufferLayout layout = {
            { Vortex::ShaderDataType::Float3, "a_Position" },
        };
        vertexBuffer->SetLayout(layout);
        VA->AddVertexBuffer(vertexBuffer);

        Ref<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(int));
        VA->SetIndexBuffer(IB);

        shader = Vortex::Shader::Create("assets/shaders/Point.glsl");
        triShader = Vortex::Shader::Create("assets/shaders/Cube.glsl");
        Ref<Camera> cam = CreateRef<Camera>(OrthoParam(), glm::vec3(0, 0, 5.0f));
        m_ViewportWindow = CreateRef<ViewportWindow>("RayCastTest", cam);

    }
    virtual ~RayCastTest() = default;

    inline virtual void OnAttach() override
    {

    }
    inline virtual void OnDetach() override
    {

    }


    inline void OnUpdate(Vortex::Timestep ts) override
    {
        m_ViewportWindow->OnUpdate(ts);

        // background
        Vortex::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        // clear glfw background and imgui 
        Vortex::Renderer::Clear();

        // start render in viewport window
        m_ViewportWindow->Begin();

        glm::mat4 trans = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, 0));
        Vortex::Camera* cam = m_ViewportWindow->GetCamera().get();

        // cast ray
        ImVec2 scrPos = ImGui::GetMousePos();
        ImVec2 normPos = m_ViewportWindow->ConvertToNormalizedPos(scrPos);
        Ray r;
        cam->CastRay(glm::vec2(normPos.x, normPos.y), r);
        worldPos = ImVec2(r.origin.x, r.origin.y);

        Vortex::Renderer::BeginScene(*cam);

        VA->Bind();
        triShader->Bind();
        if (worldPos.x >= -0.5f && worldPos.x <= 0.5f &&
            worldPos.y >= -0.5f && worldPos.y <= 0.5f) {
            
            triShader->SetFloat4("u_Color", glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
        }
        else {
            triShader->SetFloat4("u_Color", glm::vec4(color, 1.0f));
        }
        triShader->SetMat4("u_ViewProjection", cam->GetProjMatrix() * cam->GetViewMatrix());
        triShader->SetMat4("u_Transform", trans);
        Renderer::DrawIndexedTriangles(VA, triShader, Vortex::DrawTriangleConfig(4, 6));
        
        Vortex::Renderer::EndScene();

        m_ViewportWindow->End();
    }

    inline virtual void OnImGuiRender() override
    {
        m_ViewportWindow->OnImGuiRender();
        m_ViewportWindow->GetCamera()->CameraDebug();
        ImGui::ShowDemoWindow();

        Vortex::Camera* cam = m_ViewportWindow->GetCamera().get();
        ImGui::Begin("Debug");
   
        ImVec2 scrPos = ImGui::GetMousePos();
        ImVec2 winPos = m_ViewportWindow->ConvertToWinPos(scrPos);
        ImGui::DragFloat2("WinPos", (float*)&winPos);
        ImGui::DragFloat2("WorldPos", (float*)&worldPos);
        ImGui::DragFloat("Point Size", &ptSz);
        ImGui::ColorEdit3("Line Color", glm::value_ptr(color));
        
        ImGui::End();
    }
    inline void OnEvent(Vortex::Event& e) override
    {
        m_ViewportWindow->OnEvent(e);
    }
private:
    Ref<ViewportWindow> m_ViewportWindow;

};

