#include "vtpch.h"
#include "LineRendererComponent.h"
#include "Vortex/Geo/DrawGeoConfig.h"
#include "Scene.h"
#include "Vortex/Renderer/BatchUnit.h"

namespace Vortex
{
    Scope < Batch<LinePoint2> >  LineRendererComponent::s_Batch = nullptr;

	LineRendererComponent::LineRendererComponent()
		:m_Shader(Shader::Create("assets/shaders/Line.glsl"))
	{
        if (s_Batch == nullptr)
        {
            s_Batch = CreateScope< Batch<LinePoint2> >();
        }
	}
    void LineRendererComponent::Init()
    {
    }
    void LineRendererComponent::PostUpdate(Timestep ts)
    {

    }
    void LineRendererComponent::Flush()
    {
        // vertex buffer is empty, don't flush repeatly!
        if (s_Batch->m_FreeVertexBufferBaseIndex == 0)return;
        s_Batch->m_VertexBuffer->SetData(&s_Batch->m_TempVertexBuffer[0], s_Batch->m_FreeVertexBufferBaseIndex * sizeof(LinePoint2));

        Camera* cam = GetEntity()->GetScene()->GetInnerCamera();

        s_Batch->m_VertexArray->Bind();
        m_Shader->Bind();
        m_Shader->SetMat4("u_ViewProjection", cam->GetViewProjMatrix());
        m_Shader->SetFloat4("u_Color", m_Color);

        DrawLineConfig attr(s_Batch->GetTempBufferBatchUnitCount(), m_Width, m_Color);
        Renderer::DrawLines(s_Batch->m_VertexArray, attr);

        s_Batch->m_FreeVertexBufferBaseIndex = 0;
    }
	void LineRendererComponent::DrawLines(const std::vector<glm::vec3> points)
	{
        int sz = (int)points.size();
        for (int i = 0; i < sz; i++)
        {
            LinePoint2 lineP(points[i]);
            if (!s_Batch->TryAddBatchUnit(lineP))
            {
                Flush();
                // ensure the line strips are constant!
                VT_CORE_ASSERT(i > 0, "i <= 0!");
                s_Batch->TryAddBatchUnit(LinePoint2(points[i - 1]));
                s_Batch->TryAddBatchUnit(lineP);
            }
        }
        Flush();
	}

}
