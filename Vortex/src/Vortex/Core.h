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

#ifdef VT_DEBUG
    #define VT_ENABLE_ASSERTS
#endif

#ifdef VT_ENABLE_ASSERTS
    /*
    tips:
    __debugbreak is a break point.
    */
    #define VT_ASSERT(x, ...) {if(!(x)) {VT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
    #define VT_CORE_ASSERT(x, ...) {if(!(x)) {VT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
#else
    #define VT_ASSERT(x, ...)
    #define VT_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
