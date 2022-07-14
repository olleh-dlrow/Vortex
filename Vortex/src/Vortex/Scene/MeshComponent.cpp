#include "vtpch.h"
#include "MeshComponent.h"

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


