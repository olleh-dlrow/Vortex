#include "vtpch.h"
#include "GraphicsContext.h"

#include "Vortex/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Vortex/Events/GraphicsEvent.h"
#include "Vortex/Core/Application.h"

namespace Vortex
{
    void GraphicsContext::SetMSAA(bool open)
    {
        m_MSAAOpened = open;
        if (m_MSAAOpened) {
            MSAAOpenedEvent e;
            Application::Get().OnEvent(e);
        }
        else
        {
            MSAAClosedEvent e;
            Application::Get().OnEvent(e);
        }
    }
    void GraphicsContext::SetMSAANSamples(int nSamples)
    {
        m_nSamples = nSamples;
        MSAANSamplesChangedEvent e;
        Application::Get().OnEvent(e);
    }
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
