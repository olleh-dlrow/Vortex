#include "vtpch.h"
#include "Vortex/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Vortex/Renderer/GraphicsContext.h"
#include "Vortex/Renderer/RendererAPI.h"
#include "Vortex/Renderer/Camera.h"
#include "Vortex/Geo/DrawGeoConfig.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/Buffer.h"

namespace Vortex {

    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
    Scope<RendererAPI> Renderer::s_RendererAPI = RendererAPI::Create();
    void Renderer::Init()
    {
        s_RendererAPI->Init();
    }

    void Renderer::Shutdown()
    {

    }

    void Renderer::SetDepthTest(bool enable)
    {
        s_RendererAPI->SetDepthTest(enable);
    }

    void Renderer::SetDepthPassCond(DepthPassCond cond)
    {
        s_RendererAPI->SetDepthPassCond(cond);
    }

    void Renderer::SetBlend(bool enable)
    {
        s_RendererAPI->SetBlend(enable);
    }

    void Renderer::SetBlendFunc(BlendFactor src, BlendFactor dst)
    {
        s_RendererAPI->SetBlendFunc(src, dst);
    }

    // OBSOLETE
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(Camera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetProjMatrix() * camera.GetViewMatrix();
    }

    void Renderer::EndScene()
    {
    }

    // OBSOLETE
    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        VT_CORE_ASSERT(0, "OBSOLETE");
        shader->Bind();
        
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);

        vertexArray->Bind();
        DrawIndexed(vertexArray);
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
    {
        shader->Bind();
        vertexArray->Bind();
        DrawIndexed(vertexArray);
    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        s_RendererAPI->SetClearColor(color);
    }

    void Renderer::Clear()
    {
        s_RendererAPI->Clear();
    }

    void Renderer::DrawIndexedTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr)
    {
        s_RendererAPI->DrawIndexedTriangles(vertexArray, attr);
    }

    void Renderer::DrawTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr)
    {
        s_RendererAPI->DrawTriangles(vertexArray, attr);
    }

    void Renderer::DrawLines(const Ref<VertexArray>& vertexArray, DrawLineConfig attr)
    {
        s_RendererAPI->DrawLines(vertexArray, attr);
    }

    // OBSOLETE
    void Renderer::DrawLines(const std::vector<glm::vec3> points, float width, const glm::vec3& color)
    {
        VT_CORE_ASSERT(0, "OBSOLETE");
        static bool initialized = false;
        static uint32_t maxPointCount = 1023;

        static Ref<VertexArray> VA;
        static Ref<VertexBuffer> VB;
        static BufferLayout layout = { {Vortex::ShaderDataType::Float3, "a_Position"} };
        static Ref<Shader> shader = Shader::Create("assets/shaders/Line.glsl");

        uint32_t pointCnt = (uint32_t)points.size();
        if (pointCnt > maxPointCount) 
        {
            VT_ASSERT(maxPointCount != 0xffffffff, "MaxPointCount Arrived!");
            maxPointCount = (maxPointCount << 1) + 1;
            initialized = false;
        }

        if (!initialized)
        {
            VA = VertexArray::Create();
            VB = VertexBuffer::Create(maxPointCount * sizeof(glm::vec3));
            VB->SetLayout(layout);
            VA->AddVertexBuffer(VB);
            initialized = true;
        }

        DrawLineConfig attr(pointCnt, width, color);
        VA->Bind();
        VB->SetData(&points[0], (uint32_t)pointCnt * sizeof(glm::vec3));
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetFloat4("u_Color", attr.color);
        s_RendererAPI->DrawLines(VA, attr);
    }

    // OBSOLETE
    void Renderer::DrawLine(const std::pair<glm::vec3, glm::vec3>& ends, float width, const glm::vec3& color)
    {
        VT_CORE_ASSERT(0, "OBSOLETE");
        static bool initialized = false;
        const int VBSize = 2 * sizeof(glm::vec3);
        static Ref<VertexArray> VA = VertexArray::Create();
        static Ref<VertexBuffer> VB = VertexBuffer::Create(VBSize);
        static BufferLayout layout = { {Vortex::ShaderDataType::Float3, "a_Position"} };
        static Ref<Shader> shader = Shader::Create("assets/shaders/Line.glsl");

        if (!initialized)
        {
            VB->SetLayout(layout);
            VA->AddVertexBuffer(VB);
            initialized = true;
        }

        DrawLineConfig attr(2, width, color);

        VA->Bind();
        glm::vec3 endsArr[2] = {ends.first, ends.second};
        VB->SetData(endsArr, VBSize);

        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetFloat4("u_Color", attr.color);
        s_RendererAPI->DrawLines(VA, attr);
    }

    void Renderer::DrawPoints(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, DrawPointConfig attr)
    {
        vertexArray->Bind();
        shader->Bind();
        if (attr.size > 0.0f)
        {
            shader->SetFloat("u_PointSize", attr.size);
        }
        shader->SetFloat4("u_Color", attr.color);
        s_RendererAPI->DrawPoints(vertexArray, attr);
    }

    // OBSOLETE
    void Renderer::DrawTriangles(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, DrawTriangleConfig attr)
    {
        vertexArray->Bind();
        shader->Bind();
        s_RendererAPI->DrawTriangles(vertexArray, attr);
    }

    // OBSOLETE
    void Renderer::DrawPoints(const std::vector<glm::vec3>& positions, float size, const glm::vec4& color)
    {
        VT_CORE_ASSERT(0, "OBSOLETE");
    }

    // OBSOLETE
    void Renderer::DrawPoints(const std::vector<glm::vec3>& positions, float size, const std::vector<glm::vec4>& colors)
    {
        VT_CORE_ASSERT(0, "OBSOLETE");
    }
}
