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

        virtual void DrawTriangleStrip(const Ref<VertexArray>& vertexArray, TriangleAttribute attr) override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

        virtual void DrawLines(const Ref<VertexArray>& vertexArray, LineAttribute attr) override;
        virtual void DrawPoints(const Ref<VertexArray>& vertexArray, PointAttribute attr) override;
    };
}
