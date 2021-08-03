#pragma once

namespace Vortex
{
    // The context of graphics which initialize graphics and swap buffers
    class GraphicsContext 
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static Scope<GraphicsContext> Create(void* window);
    };
}