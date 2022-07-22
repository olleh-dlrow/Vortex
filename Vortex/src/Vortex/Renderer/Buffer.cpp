#include "vtpch.h"
#include "Vortex/Renderer/Buffer.h"

#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Vortex 
{
    // only give a memory of size but not transport actual data
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t count)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(count);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    // indices: the data of indices stored in buffer
    // count: the count of indices (not size!)
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<RenderBuffer> RenderBuffer::Create(uint32_t width, uint32_t height, bool MSAAOpened)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLRenderBuffer>(width, height, MSAAOpened);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height, bool MSAAOpened, 
                                const std::vector<Ref<Texture2D>>& textures)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFrameBuffer>(width, height, MSAAOpened, textures);
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    void FrameBuffer::Blit(FrameBuffer& src, FrameBuffer& dst)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:    
            VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return;
        case RendererAPI::API::OpenGL:
            OpenGLFrameBuffer::BlitImpl(src, dst);
            return;
        }
        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
    }
}
