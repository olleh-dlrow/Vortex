#include "vtpch.h"
#include "MeshRendererComponent.h"

namespace Vortex
{
	void MeshRendererComponent::SetMeshComponent(MeshComponent* comp)
	{
		ResetMesh(comp);
	}

	void MeshRendererComponent::OnUpdate(Timestep ts)
	{
		
	}

	void MeshRendererComponent::PostUpdate(Timestep ts)
	{
		DrawMesh();
	}

	void MeshRendererComponent::DrawMesh()
	{
		auto& mesh = m_MeshComp->m_Mesh;
		m_VertexBuffer->SetData(&mesh->m_Vertices[0], m_VertexBufferSize);
		m_VertexArray->Bind();
		
		m_Material->m_Shader->Bind();
		m_ConfigMatCallback(*m_Material.get());
		m_Material->ApplyProperties();

		Renderer::DrawIndexedTriangles(m_VertexArray, m_DrawCfg);
	}
	void MeshRendererComponent::ResetMesh(MeshComponent* comp)
	{
		m_MeshComp = comp;

		// create vertex array and bind(important!!)
		m_VertexArray = VertexArray::Create();
		m_VertexArray->Bind();

		// init vertex buffer
		m_VertexBufferSize = comp->m_Mesh->m_Vertices.size() * sizeof(MeshVertex);
		m_VertexBuffer = VertexBuffer::Create(m_VertexBufferSize);
		m_VertexBuffer->Bind();
		m_VertexBuffer->SetLayout(MeshVertex::GetLayout());

		// init index buffer
		m_IndexBuffer = IndexBuffer::Create(comp->m_Mesh->m_Indices.size());
		m_IndexBuffer->Bind();
		m_IndexBuffer->SetData(&comp->m_Mesh->m_Indices[0], comp->m_Mesh->m_Indices.size());
	
		// init vertex array
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// set draw config
		m_DrawCfg.vertCnt = comp->m_Mesh->m_Vertices.size();
		m_DrawCfg.indexCnt = comp->m_Mesh->m_Indices.size();
	}
}

