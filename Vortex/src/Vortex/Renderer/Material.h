#pragma once

#include<unordered_map>
#include"Shader.h"
#include"Vortex/Core/Core.h"
#include"Texture.h"

namespace Vortex
{
	class Material
	{
	public:
		Material();

		Ref<Shader>									m_Shader;
		std::unordered_map<std::string, TexPair>	m_Textures;
	};
}