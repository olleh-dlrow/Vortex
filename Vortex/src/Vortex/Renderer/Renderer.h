#pragma once

#include "Vortex/Renderer/RenderCommand.h"

#include "Vortex/Renderer/OrthographicCamera.h"
#include "Vortex/Renderer/Shader.h"

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

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        // Stored ViewProjectionMatrix
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> s_SceneData;
    };
}
