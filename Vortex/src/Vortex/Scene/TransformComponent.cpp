#include "vtpch.h"
#include "TransformComponent.h"

Vortex::TransformComponent::TransformComponent()
	:m_Position(0, 0, 0), m_Scale(1, 1, 1), m_Quaternion(1, 0, 0, 0)
{
	
}


void Vortex::TransformComponent::OnUpdate(Timestep ts)
{
	
}
