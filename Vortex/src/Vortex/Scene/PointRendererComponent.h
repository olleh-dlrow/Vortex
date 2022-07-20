#pragma once

#include "Component.h"
#include "Vortex/Renderer/Batch.h"

namespace Vortex 
{
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

		Ref<Shader>						m_Shader;

		static Scope< Batch<Quad1> >	s_Batch;
	};
}

