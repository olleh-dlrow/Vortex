#include "vtpch.h"
#include "TransformComponent.h"
#include <imgui.h>

namespace Vortex 
{
	TransformComponent::TransformComponent()
		:m_Position(0, 0, 0), m_Scale(1, 1, 1), m_Rotation(0, 0, 0), m_Quaternion(1, 0, 0, 0)
	{

	}


	void TransformComponent::OnUpdate(Timestep ts)
	{

	}

	glm::mat4 TransformComponent::GetModelMatrix() const 
	{
		// scale -> rotate -> translate
		return glm::translate(glm::mat4(1), m_Position) * glm::mat4(m_Quaternion) * glm::scale(glm::mat4(1), m_Scale);		
	}

	void TransformComponent::RenderConfigGUI() 
	{
		ImGui::DragFloat3("Position", (float*)&m_Position, 0.1f);
		if (ImGui::DragFloat3("Rotation", (float*)&m_Rotation), 0.1f) 
		{
			
			m_Quaternion = glm::quat(glm::radians(m_Rotation));
		}
		ImGui::DragFloat3("Scale", (float*)&m_Scale, 0.1f);
	}
}

