#pragma once

#include "Vortex/Core/Core.h"
#include "Component.h"
#include "MeshComponent.h"
#include "Vortex/Renderer/Material.h"
#include "Vortex/Renderer/VertexArray.h"

namespace Vortex
{
	class MeshRendererComponent : public Component
	{
	public:
		MeshRendererComponent();

		// set mesh, and update the size of buffer
		void SetMeshComponent(MeshComponent* comp);
		void DrawMesh();
		Ref<Material>& MeshMat() { return m_Material; };

	protected:
		MeshComponent*		m_MeshComp;
		Ref<Material>		m_Material;
		Ref<VertexArray>	m_VertexArray;
		Ref<VertexBuffer>	m_VertexBuffer;
		Ref<IndexBuffer>	m_IndexBuffer;
	};
}