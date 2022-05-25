#pragma once

#include "Vortex.h"
#include "Vortex/ImGui/ViewportWindow.h"

using Vortex::Ref;
using Vortex::CreateRef;
using Vortex::ViewportWindow;
using Vortex::VertexArray;
using Vortex::VertexBuffer;
using Vortex::BufferLayout;
using Vortex::Camera;
using Vortex::OrthoParam;
using Vortex::Renderer;
using Vortex::PointAttribute;
using Vortex::IndexBuffer;

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
        Vortex::Renderer::BeginScene(*cam);
        VA->Bind();
        shader->Bind();
        shader->SetFloat4("u_Color", glm::vec4(color, 1.0f));
        shader->SetMat4("u_ViewProjection", cam->GetProjMatrix() * cam->GetViewMatrix());
        shader->SetMat4("u_Transform", trans);
        Renderer::DrawPoints(VA, shader, PointAttribute(4, 10.0f));

        VA->Bind();
        triShader->Bind();
        triShader->SetFloat4("u_Color", glm::vec4(color, 1.0f));
        triShader->SetMat4("u_ViewProjection", cam->GetProjMatrix() * cam->GetViewMatrix());
        triShader->SetMat4("u_Transform", trans);
        Renderer::DrawTriangleStrip(VA, triShader, Vortex::TriangleAttribute(4));
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
        glm::vec4 tmp = cam->GetProjMatrix() * cam->GetViewMatrix() * glm::vec4(0, 0, 0, 1);
        ImGui::InputFloat4("Point", glm::value_ptr(tmp));
        ImGui::End();
    }
    inline void OnEvent(Vortex::Event& e) override
    {
        m_ViewportWindow->OnEvent(e);
    }
private:
    Ref<ViewportWindow> m_ViewportWindow;

};

