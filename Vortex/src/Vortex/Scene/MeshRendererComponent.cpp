#include "vtpch.h"
#include "MeshRendererComponent.h"

namespace Vortex
{
	MeshRendererComponent::MeshRendererComponent(MeshComponent* comp)
	{
		ResetMesh(comp);
	}
	void MeshRendererComponent::SetMeshComponent(MeshComponent* comp)
	{
		ResetMesh(comp);
	}
	void MeshRendererComponent::DrawMesh()
	{
		auto& mesh = m_MeshComp->m_Mesh;
		m_VertexBuffer->SetData(&mesh->m_Vertices[0], m_VertexBufferSize);
		m_VertexArray->Bind();
		
		m_Material->m_Shader->Bind();
		m_ConfigMatCallback(*m_Material.get());
		m_Material->BindTextures();

		DrawTriangleConfig attr(mesh->m_Vertices.size(), mesh->m_Indices.size(), GL_LINE);
		Renderer::DrawIndexedTriangles(m_VertexArray, attr);
	}
	void MeshRendererComponent::ResetMesh(MeshComponent* comp)
	{
		m_MeshComp = comp;
		// init vertex buffer
		m_VertexBufferSize = comp->m_Mesh->m_Vertices.size() * sizeof(MeshVertex);
		m_VertexBuffer = VertexBuffer::Create(m_VertexBufferSize);
		m_VertexBuffer->SetLayout(MeshVertex::GetLayout());

		// init index buffer
		m_IndexBuffer = IndexBuffer::Create(comp->m_Mesh->m_Indices.size());
		m_IndexBuffer->SetData(&comp->m_Mesh->m_Indices[0], comp->m_Mesh->m_Indices.size());
	
		// init vertex array
		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
}

