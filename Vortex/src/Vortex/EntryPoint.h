#pragma once

#ifdef VT_PLATFORM_WINDOWS

extern Vortex::Application *Vortex::CreateApplication();

int main(int argc, char **argv) {
    Vortex::Log::Init();

    auto app = Vortex::CreateApplication();
    app->Run();
    delete app;
}

#endif