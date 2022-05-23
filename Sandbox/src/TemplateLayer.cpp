#include "TemplateLayer.h"

#include "Vortex/Geo/GeoAttribute.h"

#include "imgui/imgui.h"
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   gl_PointSize = 10.0;\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

glm::vec3 m_BgColor = { 0.1f, 0.1f, 0.1f };

Vortex::Ref<Vortex::Shader> squareShader;
Vortex::Ref<Vortex::VertexArray> squareVA;

Vortex::Ref<Vortex::Shader> lineShader;
Vortex::Ref<Vortex::VertexArray> lineVA;

Vortex::Ref<Vortex::Shader> pointShader;

static void InitSquare() {
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    // create VA, VB, IB
    squareVA = Vortex::VertexArray::Create();
    auto vertexBuffer = Vortex::VertexBuffer::Create(vertices, sizeof(vertices));
    Vortex::BufferLayout layout = { {Vortex::ShaderDataType::Float3, "a_Position"} };
    vertexBuffer->SetLayout(layout);
    squareVA->AddVertexBuffer(vertexBuffer);

    auto indexBuffer = Vortex::IndexBuffer::Create(indices, sizeof(indices) / sizeof(int));
    squareVA->SetIndexBuffer(indexBuffer);

    // create shader
    squareShader = Vortex::Shader::Create("Triangle", vertexShaderSource, fragmentShaderSource);
}

static void InitLine() {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    lineVA = Vortex::VertexArray::Create();
    auto vertexBuffer = Vortex::VertexBuffer::Create(vertices, sizeof(vertices));
    Vortex::BufferLayout layout = { {Vortex::ShaderDataType::Float3, "a_Position"} };
    vertexBuffer->SetLayout(layout);
    lineVA->AddVertexBuffer(vertexBuffer);

    // create shader
    lineShader = Vortex::Shader::Create("assets/shaders/Line.glsl");
}

TemplateLayer::TemplateLayer()
	:Layer("TemplateLayer")
{
    InitSquare();
    InitLine();
    pointShader = Vortex::Shader::Create("assets/shaders/Point.glsl");
}

void TemplateLayer::OnAttach()
{
}

void TemplateLayer::OnDetach()
{

}

float sz = 10.0f;
float width = 1.0f;

void TemplateLayer::OnUpdate(Vortex::Timestep ts)
{
	Vortex::Renderer::SetClearColor(glm::vec4(m_BgColor, 1.0f));
	Vortex::Renderer::Clear();

    Vortex::TriangleAttribute triAttr;
    Vortex::Renderer::DrawTriangleStrip(squareVA, squareShader, triAttr);

    Vortex::LineAttribute lineAttr(4, width, glm::vec3(0.5f, 0.9f, 0.4f));
    Vortex::Renderer::DrawLines(lineVA, lineShader, lineAttr);

    Vortex::PointAttribute pointAttr(4, sz);
    Vortex::Renderer::DrawPoints(lineVA, pointShader, pointAttr);
}

void TemplateLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Background Color", glm::value_ptr(m_BgColor));
    ImGui::DragFloat("Point Size", &sz, 0.1f, -1.0f, 400.0f);
    ImGui::DragFloat("Line Width", &width, 0.1f, -1.0f, 40.0f);
	ImGui::End();
}

void TemplateLayer::OnEvent(Vortex::Event& e)
{
}
