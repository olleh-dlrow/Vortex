#include "vtpch.h"
#include "MeshComponent.h"
#include "Vortex/Geo/Mesh.h"

namespace Vortex
{
	MeshComponent::MeshComponent()
		:m_Mesh(nullptr)
	{

	}
	MeshComponent::MeshComponent(const Ref<Mesh>& mesh)
		:m_Mesh(mesh)
	{

	}
}


