#pragma once

#include<glm/glm.hpp>

namespace Vortex {

    class Camera;
    class VertexArray;
    class Shader;
    // https://stackoverflow.com/questions/1021793/how-do-i-forward-declare-an-inner-class
    class RendererAPI;

    struct DrawTriangleConfig;
    struct DrawPointConfig;
    struct DrawLineConfig;

    enum class DepthPassCond
    {
        NEVER,
        LESS,
        EQUAL,
        LEQUAL,
        GREATER,
        NOTEQUAL,
        GEQUAL,
        ALWAYS
    };

    enum class BlendFactor
    {
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA
    };

    enum class RDBufferMode
    {
        NONE,
        FRONT,
        BACK
    };

    enum class BitField
    {
        NONE,
        DEPTH_BUFFER_BIT,
        COLOR_BUFFER_BIT,
        DEPTH_COLOR_BUFFER_BIT
    };

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void SetDepthTest(bool enable);
        static void SetDrawBufferMode(RDBufferMode mode);
        static void SetReadBufferMode(RDBufferMode mode);
        static void SetCullFace(bool enable);
        static void SetDepthPassCond(DepthPassCond cond);

        static void SetBlend(bool enable);
        static void SetBlendFunc(BlendFactor src, BlendFactor dst);

        static void OnWindowResize(uint32_t width, uint32_t height);

        // camera: the main camera that defines view and projection matrix
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
        
        static void Clear(BitField mask = BitField::DEPTH_COLOR_BUFFER_BIT);
               
        static void DrawIndexedTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr);
     
        static void DrawTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr);

        static void DrawTriangles(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, DrawTriangleConfig attr);

        static void DrawPoints(const std::vector<glm::vec3>&    positions,
                               float                            size,
                               const glm::vec4&                 color);

        static void DrawPoints(const std::vector<glm::vec3>&    positions,
                               float                            size,
                               const std::vector<glm::vec4>&    colors);

        static void DrawLines(const Ref<VertexArray>& vertexArray, DrawLineConfig attr);

        static void DrawLines(const std::vector<glm::vec3> points, float width = 1.0f, const glm::vec3& color = glm::vec3(1.0f, 0.5f, 0.2f));

        static void DrawLine(const std::pair<glm::vec3, glm::vec3>& ends, float width=1.0f, const glm::vec3& color = glm::vec3(1.0f, 0.5f, 0.2f));
        
        static void DrawPoints(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, DrawPointConfig attr);

        // OBSOLETE
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
        {
            VT_CORE_ASSERT(0, "OBSOLETE");
            // s_RendererAPI->DrawIndexed(vertexArray, count);
        }
        // OBSOLETE
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader)
        {
            VT_CORE_ASSERT(0, "OBSOLETE");
            //vertexArray->Bind();
            //shader->Bind();
            //s_RendererAPI->DrawIndexed(vertexArray, 0);
        }
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
