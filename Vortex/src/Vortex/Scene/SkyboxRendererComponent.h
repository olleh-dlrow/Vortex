#pragma once

#include "Vortex/Core/Core.h"
#include "Component.h"
#include "Vortex/Renderer/Texture.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/Shader.h"

namespace Vortex
{
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
		Ref<Shader>			m_Shader;
	};
}
