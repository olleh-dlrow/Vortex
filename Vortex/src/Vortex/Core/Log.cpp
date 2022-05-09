#include "vtpch.h"
#include "Vortex/Core/Log.h"

namespace Vortex {
    Ref<spdlog::logger> Log::s_CoreLogger = nullptr;
    Ref<spdlog::logger> Log::s_ClientLogger = nullptr;

    void Log::Init() {
        // https://github.com/gabime/spdlog/wiki/4.-Sinks
        std::vector<spdlog::sink_ptr> logSinks;
        // write to stdout
        logSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
        // write to Vortex.log
        logSinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("Vortex.log", true));

		// format output, see: 
        // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
        // logSinks[0]->set_pattern("%^[%T] [%l] %n: %v%$");
        // logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        // Logger with range on sinks
        s_CoreLogger = CreateRef<spdlog::logger>("VORTEX", logSinks.begin(), logSinks.end());
        // globally register the loggers so the can be accessed using spdlog::get(logger_name)
        spdlog::register_logger(s_CoreLogger);
        // output all logs whose previledge is higher than trace(0)
        s_CoreLogger->set_level(spdlog::level::trace);
        // better performance
        s_CoreLogger->flush_on(spdlog::level::trace);

        s_ClientLogger = CreateRef<spdlog::logger>("SANDBOX", logSinks.begin(), logSinks.end());
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);
    }
}
