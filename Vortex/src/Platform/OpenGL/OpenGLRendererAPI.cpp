#include "vtpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/Buffer.h"

namespace Vortex
{
    GLenum ParseReadDrawBufferMode(RDBufferMode mode) 
    {
        switch (mode)
        {
        case Vortex::RDBufferMode::NONE:
            return GL_NONE;
        case Vortex::RDBufferMode::FRONT:
            return GL_FRONT;
        case Vortex::RDBufferMode::BACK:
            return GL_BACK;
        default:
            return GL_NONE;
        }
    }

    GLbitfield ParseBitField(BitField mask)
    {
        switch (mask)
        {
        case Vortex::BitField::NONE:
            return GL_NONE;
        case Vortex::BitField::DEPTH_BUFFER_BIT:
            return GL_DEPTH_BUFFER_BIT;
        case Vortex::BitField::COLOR_BUFFER_BIT:
            return GL_COLOR_BUFFER_BIT;
        case Vortex::BitField::DEPTH_COLOR_BUFFER_BIT:
            return GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
        default:
            return GL_NONE;
        }
    }

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

        // WARNING: default close it, otherwise some output less than 4D(eg. vec2 FragColor) will be overlapped!
        // open blend effect
        SetBlend(false);
        //SetBlendFunc(BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA);

        // This affected the mix of texture
        SetDepthTest(true);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear(BitField mask)
    {
        GLbitfield m = ParseBitField(mask);
        glClear(m);
    }

    void OpenGLRendererAPI::SetDepthTest(bool enable)
    {
        if (enable)
            glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetCullFace(bool enable)
    {
        if (enable)
            glEnable(GL_CULL_FACE);
        else glDisable(GL_CULL_FACE);
    }

    void OpenGLRendererAPI::SetDrawBufferMode(RDBufferMode mode)
    {
        GLenum m = ParseReadDrawBufferMode(mode);
        glDrawBuffer(m);
    }

    void OpenGLRendererAPI::SetReadBufferMode(RDBufferMode mode)
    {
        GLenum m = ParseReadDrawBufferMode(mode);
        glReadBuffer(m);
    }

    void OpenGLRendererAPI::SetBlend(bool enable)
    {
        if (enable)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    void OpenGLRendererAPI::SetDepthPassCond(DepthPassCond cond)
    {
        uint32_t glCond = 0;
        switch (cond)
        {
        case Vortex::DepthPassCond::NEVER:
            glCond = GL_NEVER;
            break;
        case Vortex::DepthPassCond::LESS:
            glCond = GL_LESS;
            break;
        case Vortex::DepthPassCond::EQUAL:
            glCond = GL_EQUAL;
            break;
        case Vortex::DepthPassCond::LEQUAL:
            glCond = GL_LEQUAL;
            break;
        case Vortex::DepthPassCond::GREATER:
            glCond = GL_GREATER;
            break;
        case Vortex::DepthPassCond::NOTEQUAL:
            glCond = GL_NOTEQUAL;
            break;
        case Vortex::DepthPassCond::GEQUAL:
            glCond = GL_GEQUAL;
            break;
        case Vortex::DepthPassCond::ALWAYS:
            glCond = GL_ALWAYS;
            break;
        default:
            VT_CORE_ASSERT(0, "Unkown depth pass condition");
            break;
        }
        glDepthFunc(glCond);
    }

    GLenum ParsePolygonMode(PolygonMode m)
    {
        switch (m)
        {
        case Vortex::PolygonMode::POINT:
            return GL_POINT;
        case Vortex::PolygonMode::LINE:
            return GL_LINE;
        case Vortex::PolygonMode::FILL:
            return GL_FILL;
        default:
            return GL_NONE;
        }
    }

    GLenum ParsePrimitiveType(PrimitiveType type)
    {
        switch (type)
        {
        case Vortex::PrimitiveType::POINTS:
            return GL_POINTS;
        case Vortex::PrimitiveType::LINE_STRIP:
            return GL_LINE_STRIP;
        case Vortex::PrimitiveType::LINE_LOOP:
            return GL_LINE_LOOP;
        case Vortex::PrimitiveType::LINES:
            return GL_LINES;
        case Vortex::PrimitiveType::TRIANGLES:
            return GL_TRIANGLES;
        case Vortex::PrimitiveType::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case Vortex::PrimitiveType::TRIANLGE_FAN:
            return GL_TRIANGLE_FAN;
        default:
            return GL_NONE;
        }
    }

    void OpenGLRendererAPI::DrawIndexedTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr)
    {
        glPolygonMode(GL_FRONT_AND_BACK, ParsePolygonMode(attr.polygonMode));
        glDrawElements(ParsePrimitiveType(attr.triangleMode), attr.indexCnt, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawTriangles(const Ref<VertexArray>& vertexArray, DrawTriangleConfig attr)
    {
        glPolygonMode(GL_FRONT_AND_BACK, ParsePolygonMode(attr.polygonMode));
        glDrawArrays(ParsePrimitiveType(attr.triangleMode), 0, attr.vertCnt);
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

    GLenum ParseBlendFactor(BlendFactor factor)
    {
        switch (factor)
        {
        case Vortex::BlendFactor::SRC_ALPHA:
            return GL_SRC_ALPHA;
        case Vortex::BlendFactor::ONE_MINUS_SRC_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        default:
            return GL_NONE;
        }
    }

    void OpenGLRendererAPI::SetBlendFunc(BlendFactor src, BlendFactor dst)
    {
        glBlendFunc(ParseBlendFactor(src), ParseBlendFactor(dst));
    }

}
