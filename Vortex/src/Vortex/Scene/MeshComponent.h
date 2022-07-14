#pragma once

#include "Vortex/Core/Core.h"
#include "Component.h"
#include "Vortex/Geo/Mesh.h"

namespace Vortex
{
	class MeshComponent : public Component
	{
	public:
		MeshComponent();
		MeshComponent(const Ref<Mesh>& mesh);

	public:
		Ref<Mesh>	m_Mesh;
	};
}