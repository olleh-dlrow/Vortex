#include "vtpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Vortex/Core/Application.h"
#include <glad/glad.h>

namespace Vortex 
{
    /////////////////////////////////////////////////////////////////////////////
    // VertexBuffer /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    /////////////////////////////////////////////////////////////////////////////
    // IndexBuffer //////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count)
        :m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);

        // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
        // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGLIndexBuffer::SetData(const void* data, uint32_t count)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data);
    }

    /////////////////////////////////////////////////////////////////////////////
    // RenderBuffer //////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    OpenGLRenderBuffer::OpenGLRenderBuffer(uint32_t width, uint32_t height, bool MSAAOpened)
        :m_MSAAOpened(MSAAOpened)
    {
        glGenRenderbuffers(1, &m_RendererID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
        if (!MSAAOpened)
        {
            //use a single renderbuffer object for both a depth AND stencil buffer.
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); 
        }
        else
        {
            // TEST MSAA
            int nSamples = Application::Get().GetWindow().GetGraphicsContext().GetMSAANSamples();
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, nSamples, GL_DEPTH24_STENCIL8, width, height);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }   

    OpenGLRenderBuffer::~OpenGLRenderBuffer()
    {
        glDeleteRenderbuffers(1, &m_RendererID);
    }

    void OpenGLRenderBuffer::Bind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    }

    void OpenGLRenderBuffer::Unbind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    /////////////////////////////////////////////////////////////////////////////
    // FrameBuffer //////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height, bool MSAAOpened)
        :m_Width(width), m_Height(height), m_MSAAOpened(MSAAOpened)
    {
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        
        m_Tex2d = OpenGLTexture2D::Create(width, height, MSAAOpened);
        
        if (MSAAOpened)
        {
            // TEST MSAA
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_Tex2d->GetID(), 0);
        }
        else
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Tex2d->GetID(), 0);
        }
    }
    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
    }
    void OpenGLFrameBuffer::Bind(FrameBufferState state) const
    {
        switch (state)
        {
        case FrameBufferState::BOTH:
            glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
            break;
        case FrameBufferState::READ:
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
            break;
        case FrameBufferState::DRAW:
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID);
            break;
        default:
            break;
        }
    }
    void OpenGLFrameBuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::AttachRenderBuffer(const Ref<RenderBuffer>& rb) const
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb->GetID());
    }
    bool OpenGLFrameBuffer::CheckStatus() const
    {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
    void OpenGLFrameBuffer::BlitImpl(FrameBuffer& src, FrameBuffer& dst)
    {
        src.Bind(FrameBufferState::READ);
        dst.Bind(FrameBufferState::DRAW);
        glBlitFramebuffer(0, 0, src.GetWidth(), src.GetHeight(), 0, 0, dst.GetWidth(), dst.GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
}
