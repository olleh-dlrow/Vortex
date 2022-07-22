#pragma once

#include "Vortex/Core/Core.h"
#include "Component.h"

namespace Vortex
{
	class Mesh;

	class MeshComponent : public Component
	{
	public:
		MeshComponent();
		MeshComponent(const Ref<Mesh>& mesh);

	public:
		Ref<Mesh>	m_Mesh;
	};
}