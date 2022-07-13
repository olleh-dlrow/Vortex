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

	protected:
		Ref<Mesh>	m_Mesh;
	};
}