#include "vtpch.h"
#include "GraphicsContext.h"

#include "Vortex/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Vortex
{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }
        VT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
