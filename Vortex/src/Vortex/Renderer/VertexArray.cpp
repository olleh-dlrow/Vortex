#include "vtpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Vortex
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
        }

        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
