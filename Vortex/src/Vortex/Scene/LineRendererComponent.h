#pragma once

#include "Vortex/Core/Core.h"
#include "Component.h"
#include "Vortex/Renderer/Shader.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/Buffer.h"
#include "Vortex/Renderer/Batch.h"

namespace Vortex
{
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
		Ref<Shader>							m_Shader;
		static Scope < Batch<LinePoint2> >	s_Batch;
	};
}