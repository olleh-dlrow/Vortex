#pragma once

#include "Vortex/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Vortex{
    class Log
    {
    public:
        static void Init();

        // used in Vortex 
        inline static Ref<spdlog::logger>& GetCoreLogger(){return s_CoreLogger;}
        // used in Sandbox
        inline static Ref<spdlog::logger>& GetClientLogger(){return s_ClientLogger;}

    private:
            /*
            tips:
            this should be defined in source file.
            */
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
}

//Core log macros
#define VT_CORE_ERROR(...) ::Vortex::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VT_CORE_WARN(...)  ::Vortex::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VT_CORE_INFO(...)  ::Vortex::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VT_CORE_TRACE(...) ::Vortex::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VT_CORE_CRITICAL(...) ::Vortex::Log::GetCoreLogger()->critical(__VA_ARGS__)


//Client log macros
#define VT_ERROR(...) ::Vortex::Log::GetClientLogger()->error(__VA_ARGS__)
#define VT_WARN(...)  ::Vortex::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VT_INFO(...)  ::Vortex::Log::GetClientLogger()->info(__VA_ARGS__)
#define VT_TRACE(...) ::Vortex::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VT_CRITICAL(...) ::Vortex::Log::GetClientLogger()->critical(__VA_ARGS__)