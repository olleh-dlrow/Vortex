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

        void SetHDR(bool open);
        bool GetHDR() const { return m_HDROpened; }
        void SetHDRExposure(float exposure);
        float GetHDRExposure() const { return m_Exposure; }

        void SetGamma(bool open) { m_GammaOpened = open; }
        bool GetGamma() const { return m_GammaOpened; }

        static Scope<GraphicsContext> Create(void* window);
    protected:
        bool        m_MSAAOpened;       // may use event to broadcast the change of msaa
        int         m_nSamples = 4;

        bool        m_HDROpened;
        float       m_Exposure = 1.0f;

        bool        m_GammaOpened;
    };
}