#pragma once

namespace Vortex
{
    // The context of graphics which initialize graphics and swap buffers
    class GraphicsContext 
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        void SetMSAA(bool open);
        bool GetMSAA() const { return m_MSAAOpened; }
        int  GetMSAANSamples() const { return m_nSamples; }
        void SetMSAANSamples(int nSamples);

        static Scope<GraphicsContext> Create(void* window);
    protected:
        int         m_nSamples = 4;
        bool        m_MSAAOpened;       // may use event to broadcast the change of msaa
    };
}