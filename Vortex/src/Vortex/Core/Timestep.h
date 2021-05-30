#pragma once

namespace Vortex 
{
    class Timestep
    {
    public:
        Timestep(float time = 0.0f)
            :m_Time(time)
        {
            
        }

        //tips: implicit type conversion, which convert Timestep to float
        //eg: Timestep ts = new Timestep(10); float f = ts;  //f = 10.0f
        operator float() const { return m_Time; }

        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const { return m_Time * 1000.0f; }

    private:
        float m_Time;
    };
}
