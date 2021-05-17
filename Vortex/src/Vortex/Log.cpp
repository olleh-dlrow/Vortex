#include "vtpch.h"
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include "Log.h"

namespace Vortex {
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init() {
        spdlog::set_pattern("%^[%T]%@ %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("VORTEX");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
        VT_CORE_INFO("Log Init.");
        
        spdlog::set_level(spdlog::level::debug);
        SPDLOG_LOGGER_DEBUG(s_CoreLogger, "Log Init.");
    }
}
