#pragma once

#include "Vortex/Renderer/Buffer.h"

namespace Vortex 
{
    class Texture2D;

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        // only give a memory of size but not transport actual data
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        virtual const BufferLayout& GetLayout() const override {return m_Layout;}
        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t count);
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;

        virtual void SetData(const void* data, uint32_t count) override;

        virtual uint32_t GetCount() const { return m_Count; }
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };

    class OpenGLRenderBuffer : public RenderBuffer
    {
    public:
        OpenGLRenderBuffer(uint32_t width, uint32_t height, bool MSAAOpened = false);
        virtual ~OpenGLRenderBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;
        virtual uint32_t GetID() const override { return m_RendererID; }
        
    private:
        uint32_t m_RendererID;
    };

    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(uint32_t width, 
                          uint32_t height, 
                          bool MSAAOpened,
                          const std::vector<Ref<Texture2D>>& textures);
        virtual ~OpenGLFrameBuffer();

        virtual void Bind(FrameBufferState state) const override;
        virtual void Unbind() const override;
        virtual void AttachRenderBuffer(const Ref<RenderBuffer>& rb) const override;
        virtual bool CheckStatus() const override;
        virtual uint32_t GetWidth() const override { return m_Width; } 
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetTextureID(int index) const override;
        virtual uint32_t GetID() const override { return m_RendererID; }
        virtual Texture2D& GetInnerTexture(int index) override { return *(m_Textures[index]); }

        static void BlitImpl(FrameBuffer& src, FrameBuffer& dst);
    private:
        uint32_t                            m_RendererID;
        std::vector<Ref<Texture2D>>         m_Textures;
        uint32_t                            m_Width;
        uint32_t                            m_Height;
    };
}
