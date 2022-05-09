#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define VT_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#endif // End of platform detection


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

// bind a event callback function to a function pointer
#define VT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Vortex
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args &&...args)
	{
        // tips: C++11 lets us perform perfect forwarding, which means that
        // we can forward the parameters passed to a function template to
        // another function call inside it without losing their own
        // qualifiers (const-ref, ref, value, rvalue, etc.).
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T> 
	using Ref = std::shared_ptr<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args &&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
} // namespace Vortex
