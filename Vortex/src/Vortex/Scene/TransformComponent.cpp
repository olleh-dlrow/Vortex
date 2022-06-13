#include "vtpch.h"
#include "TransformComponent.h"

Vortex::TransformComponent::TransformComponent()
	:m_Position(0, 0, 0), m_Scale(1, 1, 1)
{
	
}

void Vortex::TransformComponent::Init()
{
	VT_CORE_INFO("Init Transform");
}

void Vortex::TransformComponent::OnUpdate(Timestep ts)
{
	
}
