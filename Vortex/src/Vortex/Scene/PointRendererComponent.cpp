#include"vtpch.h"

#include"PointRendererComponent.h"
#include"Scene.h"

namespace Vortex
{
	Scope<Batch<Quad1>> PointRendererComponent::s_Batch = nullptr;

	PointRendererComponent::PointRendererComponent()
		:m_Shader(Shader::Create("assets/shaders/Vertex1.glsl"))
	{
		if (s_Batch == nullptr)
		{
			// maybe move to the initial process in Scene
			s_Batch = CreateScope<Batch<Quad1>>();
		}
	}
	void PointRendererComponent::Init()
	{
	}
	void PointRendererComponent::OnUpdate(Timestep ts)
	{

	}

	void PointRendererComponent::AfterUpdate(Timestep ts)
	{
		Flush();
	}
	void PointRendererComponent::Flush()
	{
		// vertex buffer is empty, don't flush repeatly!
		if (s_Batch->m_FreeVertexBufferBaseIndex == 0)return;
		s_Batch->m_VertexBuffer->SetData(&s_Batch->m_TempVertexBuffer[0], s_Batch->m_FreeVertexBufferBaseIndex * sizeof(Quad1));

		Camera* cam = GetEntity()->GetScene()->GetInnerCamera();

		s_Batch->m_VertexArray->Bind();
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", cam->GetViewProjMatrix());

		Renderer::DrawIndexedTriangles(s_Batch->m_VertexArray, DrawTriangleConfig(s_Batch->m_FreeVertexBufferBaseIndex, s_Batch->GetTempBufferIndexCount()));

		s_Batch->m_FreeVertexBufferBaseIndex = 0;
	}
	void PointRendererComponent::DrawPoints(const std::vector<glm::vec3>& positions, float size, const std::vector<glm::vec4>& colors)
	{
		int sz = (int)positions.size();
		for (int i = 0; i < sz; i++)
		{
			Quad1 quad(positions[i], glm::vec2(1, 1) * 0.1f * size, colors[i]);
			if (!s_Batch->TryAddBatchUnit(quad))
			{
				Flush();
				s_Batch->TryAddBatchUnit(quad);
			}
		}
	}
}