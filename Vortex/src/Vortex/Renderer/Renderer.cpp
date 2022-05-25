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

    void Renderer::DrawTriangleStrip(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, TriangleAttribute attr)
    {
        vertexArray->Bind();
        shader->Bind();
        s_RendererAPI->DrawTriangleStrip(vertexArray, attr);
    }
    void Renderer::DrawLines(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, LineAttribute attr)
    {
        vertexArray->Bind();
        shader->Bind();
        shader->SetFloat4("u_Color", glm::vec4(attr.color, 1.0f));
        s_RendererAPI->DrawLines(vertexArray, attr);
    }
    void Renderer::DrawPoints(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, PointAttribute attr)
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

    void Renderer::DrawTriangles(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, TriangleAttribute attr)
    {
        vertexArray->Bind();
        shader->Bind();
        s_RendererAPI->DrawTriangles(vertexArray, attr);
    }
}
