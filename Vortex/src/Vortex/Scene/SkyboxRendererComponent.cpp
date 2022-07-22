#include "vtpch.h"
#include "SkyboxRendererComponent.h"
#include "Scene.h"
#include "Vortex/Geo/DrawGeoConfig.h"
#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/Camera.h"
#include "Vortex/Renderer/Buffer.h"

#include "Vortex/Renderer/Texture.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/Shader.h"

namespace Vortex
{
    SkyboxRendererComponent::SkyboxRendererComponent()
    {
        static float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        m_Shader = Shader::Create("assets/shaders/Skybox.glsl");
        m_VertexArray = VertexArray::Create();
        m_VertexArray->Bind();
        m_VertexBuffer = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
        m_VertexBuffer->SetLayout(Vortex::BufferLayout{
            { Vortex::ShaderDataType::Float3, "aPos"}
            });
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    }
    void SkyboxRendererComponent::OnUpdate(Timestep ts)
	{
     
	}
	void SkyboxRendererComponent::PostUpdate(Timestep ts)
	{
        Draw();
	}
	void SkyboxRendererComponent::SetCubemapTextures(const std::vector<std::string>& facesPath)
	{
        m_Cubemap = Cubemap::Create(facesPath);
	}
	void SkyboxRendererComponent::Draw()
	{
        Camera* camera = GetEntity()->GetScene()->GetInnerCamera();

        m_Shader->Bind();
        // skybox
        Renderer::SetDepthPassCond(DepthPassCond::LEQUAL);
        auto view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
        m_Shader->SetMat4("view", view);
        m_Shader->SetMat4("projection", camera->GetProjMatrix());
        m_Shader->SetInt("skybox", 0);
        m_VertexArray->Bind();
        // skybox cube
        m_Cubemap->Bind();
        Renderer::DrawTriangles(m_VertexArray, Vortex::DrawTriangleConfig(36, 0));
        Renderer::SetDepthPassCond(DepthPassCond::LESS);
	}
}

