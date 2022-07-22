#pragma once

#include <functional>
#include "Vortex/Core/Core.h"
#include "Component.h"
#include "Vortex/Geo/DrawGeoConfig.h"

namespace Vortex
{
	class MeshComponent;
	class Material;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

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