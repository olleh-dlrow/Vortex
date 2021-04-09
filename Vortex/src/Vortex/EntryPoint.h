#pragma once


#ifdef VT_PLATFORM_WINDOWS

extern Vortex::Application *Vortex::CreateApplication();

int main(int argc, char **argv) {
    Vortex::Log::Init();
    VT_CORE_WARN("Initial");
    int a = 5;
    VT_TRACE("Hello {0}", a);

    auto app = Vortex::CreateApplication();
    app->Run();
    delete app;
}

#endif