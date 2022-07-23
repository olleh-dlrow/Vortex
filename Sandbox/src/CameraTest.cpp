#include "CameraTest.h"

#include <Vortex.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Vortex/Renderer/Camera.h>
#include <Vortex/ImGui/ViewportWindow.h>
#include <imgui.h>

static float vertices[] = {
-0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
-0.5f,  0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,

-0.5f, -0.5f,  0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,
-0.5f, -0.5f,  0.5f,

-0.5f,  0.5f,  0.5f,
-0.5f,  0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,
-0.5f, -0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,

 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,

-0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f, -0.5f,  0.5f,
-0.5f, -0.5f,  0.5f,
-0.5f, -0.5f, -0.5f,

-0.5f,  0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,
-0.5f,  0.5f, -0.5f,
};
// world space positions of our cubes
static glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

static Vortex::Ref<Vortex::VertexArray> VA;
static Vortex::Ref<Vortex::Shader> shader;
static glm::vec3 color = glm::vec3(0.3f, 0.8f, 0.2f);


static Vortex::Ref<Vortex::ViewportWindow> vw;

CameraTest::CameraTest()
	:Layer("CameraTest")
{
    VA = Vortex::VertexArray::Create();
    Vortex::Ref<Vortex::VertexBuffer> vertexBuffer = Vortex::VertexBuffer::Create(vertices, sizeof(vertices));
    Vortex::BufferLayout layout = {
        { Vortex::ShaderDataType::Float3, "a_Position" },
    };
    vertexBuffer->SetLayout(layout);
    VA->AddVertexBuffer(vertexBuffer);

    shader = Vortex::Shader::Create("assets/shaders/Cube.glsl");

    uint32_t width = Vortex::Application::Get().GetWindow().GetWidth();
    uint32_t height = Vortex::Application::Get().GetWindow().GetHeight();


    Vortex::Ref<Vortex::Camera> cam = Vortex::CreateRef<Vortex::Camera>(Vortex::OrthoParam(), glm::vec3(0.0f, 0.0f, 3.0f),
        static_cast<float>(Vortex::Application::Get().GetWindow().GetWidth()));
    vw = Vortex::CreateRef<Vortex::ViewportWindow>("vw1", cam);
}

void CameraTest::OnAttach()
{
}

void CameraTest::OnDetach()
{
}

void CameraTest::OnUpdate(Vortex::Timestep ts)
{
    Vortex::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    // clear glfw background and imgui 
    Vortex::Renderer::Clear();

    vw->Begin();
    vw->OnUpdate(ts);

    Vortex::Camera* cam = vw->GetCamera();
    Vortex::Renderer::BeginScene(*cam);
    VA->Bind();
    shader->Bind();
    shader->SetFloat4("u_Color", glm::vec4(color, 1.0f));
    shader->SetMat4("u_ViewProjection", vw->GetCamera()->GetViewProjMatrix());

    for (int i = 0; i < 10; i++)
    {
        glm::mat4 modelMat = glm::identity<glm::mat4>();
        modelMat = glm::translate(modelMat, cubePositions[i]);
        float angle = 20.0f * i;
        modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->SetMat4("u_Transform", modelMat);
        Vortex::DrawTriangleConfig attr(36, 0, Vortex::PolygonMode::LINE);
        Vortex::Renderer::DrawTriangles(VA, shader, attr);
    }

    Vortex::Renderer::EndScene();
    vw->End();
}

void CameraTest::OnImGuiRender()
{
    vw->OnImGuiRender();
    auto cam = vw->GetCamera();
    cam->RenderConfigGUI();
}

void CameraTest::OnEvent(Vortex::Event& e)
{
    vw->OnEvent(e);
}
