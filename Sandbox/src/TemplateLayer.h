#pragma once

#include <Vortex.h>

class TemplateLayer : public Vortex::Layer
{
public:
    TemplateLayer();
    virtual ~TemplateLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Vortex::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Vortex::Event & e) override;
};
