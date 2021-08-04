#pragma once
#include "Vortex/Core/Core.h"

#ifdef VT_PLATFORM_WINDOWS

extern Vortex::Application *Vortex::CreateApplication();

int main(int argc, char **argv) {
    Vortex::Log::Init();

    VT_PROFILE_BEGIN_SESSION("StartUp", "VortexProfile-Startup.json");
    auto app = Vortex::CreateApplication();
    VT_PROFILE_END_SESSION();

    VT_PROFILE_BEGIN_SESSION("Runtime", "VortexProfile-Runtime.json");
    app->Run();
    VT_PROFILE_END_SESSION();

    VT_PROFILE_BEGIN_SESSION("StartUp", "VortexProfile-Shutdown.json");
    delete app;
    VT_PROFILE_END_SESSION();
}

#endif