#include "vtpch.h"
#include "Vortex/Renderer/RenderCommand.h"

namespace Vortex
{
    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
