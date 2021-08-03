#include "vtpch.h"
#include "Vortex/Core/Log.h"

namespace Vortex {
    Ref<spdlog::logger> Log::s_CoreLogger = nullptr;
    Ref<spdlog::logger> Log::s_ClientLogger = nullptr;


    void Log::Init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("VORTEX");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
        VT_CORE_INFO("Init Log: core-trace client-trace");
    }
}
