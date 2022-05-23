#pragma once

#include "Vortex/Renderer/RenderCommand.h"

#include "Vortex/Renderer/OrthographicCamera.h"
#include "Vortex/Renderer/Shader.h"
#include "Vortex/Geo/LineAttribute.h"
#include "Vortex/Geo/PointAttribute.h"

namespace Vortex {

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);

        // camera: the main camera that defines view and projection matrix
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        // bind shader and vertexArray to renderer, then draw index
        // transform: the matrix that transform object from local space to world space
        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, 
            const glm::mat4& transform = glm::mat4(1.0f));

        // OBSOLETE
        static void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }
        
        // OBSOLETE
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
        {
            s_RendererAPI->DrawIndexed(vertexArray, count);
        }
        // OBSOLETE
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader)
        {
            vertexArray->Bind();
            shader->Bind();
            s_RendererAPI->DrawIndexed(vertexArray, 0);
        }
        
        inline static void DrawTriangleStrip(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, TriangleAttribute attr)
        {
            vertexArray->Bind();
            shader->Bind();
            s_RendererAPI->DrawTriangleStrip(vertexArray, attr);
        }
        inline static void DrawLines(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, LineAttribute attr)
        {
            vertexArray->Bind();
            shader->Bind();
            shader->SetFloat4("u_Color", glm::vec4(attr.color, 1.0f));
            s_RendererAPI->DrawLines(vertexArray, attr);
        }
        inline static void DrawPoints(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, PointAttribute attr)
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
        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        // Stored ViewProjectionMatrix
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> s_SceneData;
        static Scope<RendererAPI> s_RendererAPI;
    };
}
