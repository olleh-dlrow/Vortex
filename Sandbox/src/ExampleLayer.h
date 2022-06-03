#pragma once

#include <Vortex.h>

class ExampleLayer : public Vortex::Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Vortex::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Vortex::Event& e) override;
private:
    Vortex::ShaderLibrary m_ShaderLibrary;
    Vortex::Ref<Vortex::Shader> m_Shader;
    Vortex::Ref<Vortex::VertexArray> m_VertexArray;

    Vortex::Ref<Vortex::Shader> m_FlatColorShader;
    Vortex::Ref<Vortex::VertexArray> m_SquareVA;

    Vortex::Ref<Vortex::Texture2D> m_Texture, m_ChernoLogoTexture;

    Vortex::OrthographicCameraController m_CameraController;
    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};