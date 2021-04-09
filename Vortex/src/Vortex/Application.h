#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Vortex {

    class VORTEX_API Application
    {
    public:
        void Run();
    };

    //to be defined in client
    Application *CreateApplication();
}