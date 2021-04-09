#pragma once

#ifdef VT_PLATFORM_WINDOWS
    #ifdef VT_BUILD_DLL
    #define VORTEX_API __declspec(dllexport)
    #else
    #define VORTEX_API __declspec(dllimport)
    #endif
#else
    #error Vortex only supports for windows!
#endif

#define BIT(x) (1 << x)