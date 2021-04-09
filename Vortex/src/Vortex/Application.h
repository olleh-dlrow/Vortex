#pragma once

#include "Core.h"

namespace Vortex {

    class VORTEX_API Application
    {
    public:
        void Run();
    };

    //to be defined in client
    Application *CreateApplication();
}