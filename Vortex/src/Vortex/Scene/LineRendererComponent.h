#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Vortex/Core/Core.h"
#include "Component.h"
#include "Vortex/Core/Timestep.h"


namespace Vortex
{
	template<typename BU> class Batch;
	class LinePoint2;
	class Material;

	class LineRendererComponent : public Component
	{
	public:
		LineRendererComponent();

		virtual void PostUpdate(Timestep ts) override;

		void Flush();

		// Draw lines immediately
		void DrawLines(const std::vector<glm::vec3> points);

	public:
		float								m_Width = 1.0f;
		glm::vec4							m_Color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
	
	protected:
		Ref<Material>						m_Material;
		static Scope < Batch<LinePoint2> >	s_Batch;
	};
}