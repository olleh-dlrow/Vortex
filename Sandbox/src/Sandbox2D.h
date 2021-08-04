#pragma once

#include "Vortex.h"

class Sandbox2D: public Vortex::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Vortex::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Vortex::Event& e) override;
private:
    Vortex::OrthographicCameraController m_CameraController;

    // Temp
    Vortex::Ref<Vortex::VertexArray> m_SquareVA;
    Vortex::Ref<Vortex::Shader> m_FlatColorShader;

    Vortex::Ref<Vortex::Texture2D> m_CheckerboardTexture;

    struct ProfileResult
    {
        const char* Name;
        float Time;
    };

    std::vector<ProfileResult> m_ProfileResults;

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};