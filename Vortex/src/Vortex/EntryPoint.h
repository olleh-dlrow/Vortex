#pragma once
#include"Application.h"

#ifdef VT_PLATFORM_WINDOWS

extern Vortex::Application *Vortex::CreateApplication();

int main(int argc, char **argv) {
    auto app = Vortex::CreateApplication();
    app->Run();
    delete app;
}

#endif