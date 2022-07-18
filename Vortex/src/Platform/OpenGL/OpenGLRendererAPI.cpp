#include "vtpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Vortex
{
    void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char* message,
        const void* userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         VT_CORE_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM:       VT_CORE_ERROR(message); return;
        case GL_DEBUG_SEVERITY_LOW:          VT_CORE_WARN(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: VT_CORE_TRACE(message); return;
        }

        VT_CORE_ASSERT(false, "Unknown severity level!");
    }

    void OpenGLRendererAPI::Init()
    {
#ifdef VT_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
        // for intel graphics, glLineWidth is deprecated
        std::string rendererName = std::string((const char*)glGetString(GL_RENDERER));
        if (rendererName.find("Intel") != std::string::npos)
        {
            m_supportLineWidth = false;
        }

        // open blend effect
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // This affected the mix of texture
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::SetDepthTest(bool enable)
    {
        if (enable)
            glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::DrawIndexedTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr)
    {
        glPolygonMode(GL_FRONT_AND_BACK, attr.polygonMode);
        glDrawElements(attr.triangleMode, attr.indexCnt, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr)
    {
        glPolygonMode(GL_FRONT_AND_BACK, attr.polygonMode);
        glDrawArrays(attr.triangleMode, 0, attr.vertCnt);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, DrawLineConfig attr)
    {
        if (m_supportLineWidth)
        {
            if (attr.width > 0.0f)
                glLineWidth(attr.width);
            else
                glLineWidth(1.0f);
        }

        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_LINE_SMOOTH);
        glDrawArrays(attr.mode, 0, attr.pointCount);
    }

    void OpenGLRendererAPI::DrawPoints(const Ref<VertexArray>& vertexArray, DrawPointConfig attr)
    {
        if(attr.size > 0.0f) 
        {
            glEnable(GL_PROGRAM_POINT_SIZE);
        }
        else
        {
            glDisable(GL_PROGRAM_POINT_SIZE);
        }
        glDrawArrays(attr.mode, 0, attr.count);
    }

}
