#pragma once

#include "Vortex.h"
#include <glad/glad.h>

class Bezier: public Vortex::Layer
{
public:
    Bezier();
    virtual ~Bezier() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Vortex::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Vortex::Event& e) override;
private:
    Vortex::OrthographicCameraController m_CameraController;
    GLfloat ctrlpoints[4][3] = {
        { -4, -4, 0 }, { -2, 4, 0 }, { 2, -4, 0 }, { 4, 4, 0 }
    };
};
