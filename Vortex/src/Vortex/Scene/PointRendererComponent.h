#pragma once

#include "Component.h"
#include "Vortex/Renderer/Batch.h"

namespace Vortex 
{
	class PointRendererComponent : public Component
	{
	public:
		PointRendererComponent();

		virtual void Init() override;

		virtual void OnUpdate(Timestep ts) override;

		virtual void AfterUpdate(Timestep ts) override;

		void Flush();

		void DrawPoints(const std::vector<glm::vec3>& positions, float size, const std::vector<glm::vec4>& colors);

	private:
		Ref<Shader>					m_Shader;

		static Ref< Batch<Quad1> >	s_Batch;
	};
}

