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

/*
log has effects when current level >= setted level

#define SPDLOG_LEVEL_TRACE 0
#define SPDLOG_LEVEL_DEBUG 1
#define SPDLOG_LEVEL_INFO 2
#define SPDLOG_LEVEL_WARN 3
#define SPDLOG_LEVEL_ERROR 4
#define SPDLOG_LEVEL_CRITICAL 5
#define SPDLOG_LEVEL_OFF 6
*/

//Core log macros
#define VT_CORE_ERROR(...)      SPDLOG_LOGGER_ERROR(Vortex::Log::GetCoreLogger(), __VA_ARGS__)
#define VT_CORE_WARN(...)       SPDLOG_LOGGER_WARN(Vortex::Log::GetCoreLogger(), __VA_ARGS__)
#define VT_CORE_INFO(...)       SPDLOG_LOGGER_INFO(Vortex::Log::GetCoreLogger(), __VA_ARGS__)
#define VT_CORE_TRACE(...)      SPDLOG_LOGGER_TRACE(Vortex::Log::GetCoreLogger(), __VA_ARGS__)
#define VT_CORE_CRITICAL(...)   SPDLOG_LOGGER_CRITICAL(Vortex::Log::GetCoreLogger(), __VA_ARGS__)


//Client log macros
#define VT_ERROR(...)       SPDLOG_LOGGER_ERROR(Vortex::Log::GetClientLogger(), __VA_ARGS__)
#define VT_WARN(...)        SPDLOG_LOGGER_WARN (Vortex::Log::GetClientLogger(), __VA_ARGS__)
#define VT_INFO(...)        SPDLOG_LOGGER_INFO (Vortex::Log::GetClientLogger(), __VA_ARGS__)
#define VT_TRACE(...)       SPDLOG_LOGGER_TRACE(Vortex::Log::GetClientLogger(), __VA_ARGS__)
#define VT_CRITICAL(...)    SPDLOG_LOGGER_CRITICAL(Vortex::Log::GetClientLogger(), __VA_ARGS__)
