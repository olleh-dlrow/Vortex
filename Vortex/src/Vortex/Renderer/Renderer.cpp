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
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
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
}
