#include "vtpch.h"
#include "Vortex/Core/Log.h"

namespace Vortex {
    Ref<spdlog::logger> Log::s_CoreLogger = nullptr;
    Ref<spdlog::logger> Log::s_ClientLogger = nullptr;


    void Log::Init() {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("Vortex.log", true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        s_CoreLogger = CreateRef<spdlog::logger>("VORTEX", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);

        s_ClientLogger = CreateRef<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);
    }
}
