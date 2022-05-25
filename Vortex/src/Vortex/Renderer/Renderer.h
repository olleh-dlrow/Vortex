#pragma once

#include "Vortex/Renderer/RenderCommand.h"

#include "Vortex/Renderer/OrthographicCamera.h"
#include "Vortex/Renderer/Camera.h"
#include "Vortex/Renderer/Shader.h"
#include "Vortex/Geo/GeoAttribute.h"

namespace Vortex {

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);

        // camera: the main camera that defines view and projection matrix
        static void BeginScene(OrthographicCamera& camera);
        static void BeginScene(Camera& camera);
        static void EndScene();

        // OBSOLETE
        // bind shader and vertexArray to renderer, then draw index
        // transform: the matrix that transform object from local space to world space
        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, 
            const glm::mat4& transform = glm::mat4(1.0f));

        // OBSOLETE
        static void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        static void SetClearColor(const glm::vec4& color);
        
        static void Clear();
               
        static void DrawTriangleStrip(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, TriangleAttribute attr);
     
        static void DrawTriangles(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, TriangleAttribute attr);

        static void DrawLines(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, LineAttribute attr);
        
        static void DrawPoints(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, PointAttribute attr);

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
