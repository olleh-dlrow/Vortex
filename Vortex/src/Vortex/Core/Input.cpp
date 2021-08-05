#include "vtpch.h"
#include "Vortex/Core/Input.h"

#ifdef VT_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace Vortex {

    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
#ifdef VT_PLATFORM_WINDOWS
        return CreateScope<WindowsInput>();
#else
        VT_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
}
