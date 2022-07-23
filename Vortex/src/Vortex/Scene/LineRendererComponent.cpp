#include "vtpch.h"
#include "LineRendererComponent.h"
#include "Vortex/Geo/DrawGeoConfig.h"
#include "Scene.h"
#include "Vortex/Renderer/BatchUnit.h"
#include "Vortex/Renderer/Camera.h"
#include "Vortex/Renderer/Renderer.h"

#include "Vortex/Renderer/Shader.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/Buffer.h"
#include "Vortex/Renderer/Batch.h"
#include "Vortex/Renderer/Material.h"

namespace Vortex
{
    Scope < Batch<LinePoint2> >  LineRendererComponent::s_Batch = nullptr;

	LineRendererComponent::LineRendererComponent()
		:m_Material(CreateRef<Material>("LineRendererMat"))
	{
        m_Material->m_Shader = Shader::Create("assets/shaders/Line.glsl");
        if (s_Batch == nullptr)
        {
            s_Batch = CreateScope< Batch<LinePoint2> >();
        }
	}

    void LineRendererComponent::PostUpdate(Timestep ts)
    {

    }
    void LineRendererComponent::Flush()
    {
        // vertex buffer is empty, don't flush repeatly!
        if (s_Batch->m_FreeVertexBufferBaseIndex == 0)return;
        s_Batch->m_VertexBuffer->SetData(&s_Batch->m_TempVertexBuffer[0], s_Batch->m_FreeVertexBufferBaseIndex * sizeof(LinePoint2::VertexType));

        Camera* cam = GetEntity()->GetScene()->GetInnerCamera();

        s_Batch->m_VertexArray->Bind();
        m_Material->SetMat4("u_ViewProjection", cam->GetViewProjMatrix());
        m_Material->SetFloat4("u_Color", m_Color);
        m_Material->ApplyProperties();

        DrawTriangleConfig attr(s_Batch->GetTempBufferVertexCount(), 0, PolygonMode::FILL, PrimitiveType::TRIANGLE_STRIP);
        Renderer::DrawTriangles(s_Batch->m_VertexArray, attr);
        s_Batch->m_FreeVertexBufferBaseIndex = 0;
    }
	void LineRendererComponent::DrawLines(const std::vector<glm::vec3> points)
	{
        uint32_t len = points.size();
        if (len == 1)return;

        uint32_t vbSize = len * 2 * sizeof(glm::vec3);
        // calculate draw positions
        float width = m_Width * 0.01f;
        LinePoint2 lp, llp;
        for (int i = 0; i < len; i++)
        {
            glm::vec2 lastNorm(0), nextNorm(0);
            if (i != 0)
            {
                glm::vec2 dir = points[i] - points[i - 1];
                lastNorm = glm::vec2(-dir.y, dir.x);
                lastNorm = glm::normalize(lastNorm);
            }
            if (i != len - 1)
            {
                glm::vec2 dir = points[i + 1] - points[i];
                nextNorm = glm::vec2(-dir.y, dir.x);
                nextNorm = glm::normalize(nextNorm);
            }
            glm::vec2 avgNorm = glm::normalize(lastNorm + nextNorm) * width;

            LinePoint2 linePs[2] = {
                points[i] + glm::vec3(avgNorm, 0),
                points[i] + glm::vec3(-avgNorm, 0)
            };

            for (int j = 0; j < 2; j++)
            {
                if (!s_Batch->TryAddBatchUnit(linePs[j]))
                {
                    Flush();
                    s_Batch->TryAddBatchUnit(llp);
                    s_Batch->TryAddBatchUnit(lp);
                    s_Batch->TryAddBatchUnit(linePs[j]);
                }
                llp = lp;
                lp = linePs[j];
            }
        }
        Flush();
	}

}
