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
        OpenGLRenderBuffer(uint32_t width, uint32_t height);
        OpenGLRenderBuffer(uint32_t width, uint32_t height, int nSamples);
        virtual ~OpenGLRenderBuffer();

        virtual void SetStorage(uint32_t width, uint32_t height) override;
        virtual void Bind() const;
        virtual void Unbind() const;
        virtual uint32_t GetID() const override { return m_RendererID; }
        
    private:
        uint32_t m_RendererID;
    };

    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer();
        virtual ~OpenGLFrameBuffer();

        virtual void Bind(FrameBufferState state) const override;
        virtual void Unbind() const override;
        virtual void AttachRenderBuffer(const Ref<RenderBuffer>& rb) const override;
        virtual void AttachTexture2D(Texture2D& tex2D, int attachIndex, int mipLevel) override;
        virtual void AttachCubemap(Cubemap& cubemap, int attachIndex, int faceIndex, int mipLevel) override;
        virtual void AttachDepthBuffer(Texture2D& tex2D, int mipLevel = 0) override;
        virtual bool CheckStatus() const override;
        virtual uint32_t GetID() const override { return m_RendererID; }

        static void BlitImpl(FrameBuffer& src, RectInt srcRect, FrameBuffer& dst, RectInt dstRect);
    private:
        uint32_t                            m_RendererID;
    };
}
