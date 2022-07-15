#pragma once

#include "Vortex/Renderer/RendererAPI.h"

namespace Vortex
{
    class OpenGLRendererAPI: public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void SetDepthTest(bool enable) override;

        virtual void DrawIndexedTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr) override;
        virtual void DrawTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr) override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

        virtual void DrawLines(const Ref<VertexArray>& vertexArray, DrawLineConfig attr) override;
        virtual void DrawPoints(const Ref<VertexArray>& vertexArray, DrawPointConfig attr) override;

        bool m_supportLineWidth = true;
    };
}
