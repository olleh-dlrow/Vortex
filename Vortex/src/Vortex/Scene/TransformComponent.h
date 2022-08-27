#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Vortex
{
	class TransformComponent: public Component
	{
	public:
		TransformComponent();
		virtual void OnUpdate(Timestep ts) override;
		glm::mat4 GetModelMatrix() const;

		void RenderConfigGUI();
	private:
		glm::vec3		m_Position;
		glm::vec3		m_Scale;
		glm::vec3		m_Rotation;
		glm::quat		m_Quaternion;
	};
}
