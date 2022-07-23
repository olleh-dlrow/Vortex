#pragma once

#include <string>

#include "Vortex/Core/Core.h"
#include "Component.h"

namespace Vortex
{
	class VertexArray;
	class VertexBuffer;
	class Material;
	class Cubemap;

	class SkyboxRendererComponent : public Component
	{
	public:
		SkyboxRendererComponent();

		void OnUpdate(Timestep ts) override;
		void PostUpdate(Timestep ts) override;
	
		void SetCubemapTextures(const std::vector<std::string>& facesPath);
		Cubemap& GetCubemap() { return *m_Cubemap; }
		void Draw();
	protected:
		Ref<Cubemap>		m_Cubemap;
		Ref<VertexArray>	m_VertexArray;
		Ref<VertexBuffer>	m_VertexBuffer;
		Ref<Material>		m_Material;
	};
}
