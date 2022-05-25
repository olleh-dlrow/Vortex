#pragma once

#include <glm/glm.hpp>

#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Geo/GeoAttribute.h"

namespace Vortex 
{
    // application interface gived by certain renderer,
    // This determines which type of instance you will get through Create function in renderer
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, OpenGL = 1
        };
    public:
        virtual void Init() = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
        virtual void DrawTriangleStrip(const Ref<VertexArray>& vertexArray, TriangleAttribute attr) = 0;
        virtual void DrawTriangles(const Ref<VertexArray>& vertexArray, TriangleAttribute attr) = 0;


        virtual void DrawLines(const Ref<VertexArray>& vertexArray, LineAttribute attr) = 0;
        virtual void DrawPoints(const Ref<VertexArray>& vertexArray, PointAttribute attr) = 0;

        inline static API GetAPI() { return s_API; }
        static Scope<RendererAPI> Create();
    private:
        static API s_API;
    };
}
