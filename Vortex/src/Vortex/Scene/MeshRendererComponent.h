#pragma once

#include "Vortex/Core/Core.h"
#include "Component.h"
#include "MeshComponent.h"
#include "Vortex/Renderer/Material.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/Renderer.h"

namespace Vortex
{
	using CfgMatFn = std::function<void(Material& mat)>;

	class MeshRendererComponent : public Component
	{
	public:
		MeshRendererComponent() = default;

		void OnUpdate(Timestep ts) override;
		void PostUpdate(Timestep ts) override;


		// set mesh, and update the size of buffer
		void SetMeshComponent(MeshComponent* comp);

	protected:
		void DrawMesh();
		void ResetMesh(MeshComponent* comp);

	public:
		Ref<Material>		m_Material;
		CfgMatFn			m_ConfigMatCallback;
		DrawTriangleConfig	m_DrawCfg;
		MeshComponent*		m_MeshComp;
	protected:
		Ref<VertexArray>	m_VertexArray;
		Ref<VertexBuffer>	m_VertexBuffer;
		Ref<IndexBuffer>	m_IndexBuffer;

		int					m_VertexBufferSize;
	};
}