#include "vtpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Vortex {

    void Application::Run() {
        WindowResizeEvent e(1200, 800);
        VT_TRACE(e);
        VT_INFO("hello, now I can load Vortex.dll automatically!!");
        //std::cout << e << std::endl;
        while(true);
    }
}
