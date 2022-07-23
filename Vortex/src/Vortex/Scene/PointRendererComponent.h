#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Vortex/Core/Core.h"
#include "Component.h"

namespace Vortex 
{
	template<typename BU> class Batch;
	class Quad1;
	class Material;

	class PointRendererComponent : public Component
	{
	public:
		PointRendererComponent();

		virtual void OnUpdate(Timestep ts) override;

		virtual void PostUpdate(Timestep ts) override;

		void Flush();

		void DrawPoints(const std::vector<glm::vec3>& positions, float size, const std::vector<glm::vec4>& colors);

		void DrawPoints(const std::vector<glm::vec3>& positions, float size, glm::vec4 color);
		
		void DrawPoint(glm::vec3 position, float size, glm::vec4 color);

		constexpr static float			UNIT_POINT_SIZE = 0.1f;
	private:

		Ref<Material>					m_Material;

		static Scope< Batch<Quad1> >	s_Batch;
	};
}

