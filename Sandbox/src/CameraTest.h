#pragma once

#include "Vortex.h"

class Camera;

class CameraTest : public Vortex::Layer
{
public:
    CameraTest();
    virtual ~CameraTest() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Vortex::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Vortex::Event& e) override;
private:
};
