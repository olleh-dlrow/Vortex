#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace Vortex
{
	class TransformComponent: public Component
	{
	public:
		TransformComponent();
		virtual void Init() override;
		virtual void OnUpdate(Timestep ts) override;

		glm::vec3& GetPosition() { return m_Position; }
		glm::vec3& GetScale() { return m_Scale; }
	private:
		glm::vec3		m_Position;
		glm::vec3		m_Scale;
	};
}
