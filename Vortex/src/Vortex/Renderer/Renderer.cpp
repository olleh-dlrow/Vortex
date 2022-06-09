#include "vtpch.h"
#include "Vortex/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Vortex/Renderer/Renderer2D.h"
#include "Vortex/Renderer/GraphicsContext.h"

namespace Vortex {

    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
    Scope<RendererAPI> Renderer::s_RendererAPI = RendererAPI::Create();

    void Renderer::Init()
    {
        s_RendererAPI->Init();
        RenderCommand::Init();
        Renderer2D::Init();

        // init all batches
        s_SceneData->PointBatch = CreateRef<Batch<Quad1>>(Shader::Create("assets/shaders/Vertex1.glsl"));
    }

    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
    }

    // OBSOLETE
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::BeginScene(Camera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetProjMatrix() * camera.GetViewMatrix();
    }

    void Renderer::EndScene()
    {
        // flush all batches
        s_SceneData->PointBatch->Flush(s_SceneData->ViewProjectionMatrix);
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
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

    void Renderer::DrawIndexedTriangles(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, DrawTriangleConfig attr)
    {
        s_RendererAPI->DrawIndexedTriangles(vertexArray, attr);
    }
    void Renderer::DrawLines(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, DrawLineConfig attr)
    {
        vertexArray->Bind();
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetFloat4("u_Color", glm::vec4(attr.color, 1.0f));
        s_RendererAPI->DrawLines(vertexArray, attr);
    }

    void Renderer::DrawLines(const std::vector<glm::vec3> points, float width, const glm::vec3& color)
    {
        static bool initialized = false;
        static uint32_t maxPointCount = 1023;

        static Ref<VertexArray> VA;
        static Ref<VertexBuffer> VB;
        static BufferLayout layout = { {Vortex::ShaderDataType::Float3, "a_Position"} };
        static Ref<Shader> shader = Shader::Create("assets/shaders/Line.glsl");

        uint32_t pointCnt = points.size();
        if (pointCnt > maxPointCount) 
        {
            VT_ASSERT(maxPointCount != 0xffffffff, "MaxPointCount Arrived!");
            maxPointCount = maxPointCount << 1 + 1;
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
        VB->SetData(&points[0], pointCnt * sizeof(glm::vec3));
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetFloat4("u_Color", glm::vec4(attr.color, 1.0f));
        s_RendererAPI->DrawLines(VA, attr);
    }

    void Renderer::DrawLine(const std::pair<glm::vec3, glm::vec3>& ends, float width, const glm::vec3& color)
    {
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
        shader->SetFloat4("u_Color", glm::vec4(attr.color, 1.0f));
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
        shader->SetFloat4("u_Color", glm::vec4(attr.color, 1.0f));
        s_RendererAPI->DrawPoints(vertexArray, attr);
    }

    void Renderer::DrawTriangles(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, DrawTriangleConfig attr)
    {
        vertexArray->Bind();
        shader->Bind();
        s_RendererAPI->DrawTriangles(vertexArray, attr);
    }

    void Renderer::DrawPoints(const std::vector<glm::vec3>& positions, float size, const glm::vec4& color)
    {
        auto batch = s_SceneData->PointBatch;
        auto VP = s_SceneData->ViewProjectionMatrix;
        int sz = (int)positions.size();
        for (int i = 0; i < sz; i++)
        {
            Quad1 quad(positions[i], glm::vec2(1, 1) * 0.1f * size, color);
            if (!batch->TryAddBatchUnit(quad))
            {
                batch->Flush(VP);
                batch->TryAddBatchUnit(quad);
            }
        }
    }

    void Renderer::DrawPoints(const std::vector<glm::vec3>& positions, float size, const std::vector<glm::vec4>& colors)
    {
        auto batch = s_SceneData->PointBatch;
        auto VP = s_SceneData->ViewProjectionMatrix;
        int sz = (int)positions.size();
        for (int i = 0; i < sz; i++)
        {
            Quad1 quad(positions[i], glm::vec2(1, 1) * 0.1f * size, colors[i]);
            if (!batch->TryAddBatchUnit(quad))
            {
                batch->Flush(VP);
                batch->TryAddBatchUnit(quad);
            }
        }
    }
}
