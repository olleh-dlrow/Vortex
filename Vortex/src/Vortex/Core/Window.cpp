#include "vtpch.h"
#include "Vortex/Core/Window.h"

#ifdef VT_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Vortex
{

    Scope<Window> Window::Create(const WindowProps& props)
    {
#ifdef VT_PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(props);
#else
        VT_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }

}