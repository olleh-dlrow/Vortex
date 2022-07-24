#pragma once

#include <glm/glm.hpp>

#include "Vortex/Geo/DrawGeoConfig.h"

namespace Vortex 
{
    class VertexArray;
    enum class DepthPassCond;
    enum class BlendFactor;

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
        virtual void SetDepthTest(bool enable) = 0;
        virtual void SetDepthPassCond(DepthPassCond cond) = 0;
        virtual void SetBlend(bool enable) = 0;
        virtual void SetBlendFunc(BlendFactor src, BlendFactor dst) = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
        virtual void DrawIndexedTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr) = 0;
        virtual void DrawTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr) = 0;


        virtual void DrawLines(const Ref<VertexArray>& vertexArray, DrawLineConfig attr) = 0;
        virtual void DrawPoints(const Ref<VertexArray>& vertexArray, DrawPointConfig attr) = 0;

        inline static API GetAPI() { return s_API; }
        static Scope<RendererAPI> Create();
    private:
        static API s_API;
    };
}
