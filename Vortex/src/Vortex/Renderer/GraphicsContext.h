#pragma once

namespace Vortex
{
    // The context of graphics which initialize graphics and swap buffers
    class GraphicsContext 
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetMSAA(bool open) = 0;
        virtual bool GetMSAA() const = 0;
        virtual int  GetMSAANSamples() const { return 4; }

        static Scope<GraphicsContext> Create(void* window);
    };
}