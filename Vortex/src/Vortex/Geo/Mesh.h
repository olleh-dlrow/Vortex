#pragma once

#include <vector>
#include "Vortex/Renderer/Vertex.h"

namespace Vortex
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(const std::string& name);

		std::string StatisticVertex();
		std::string StatisticIndex();

		std::string				m_Name;
		std::vector<MeshVertex> m_Vertices;
		std::vector<int>		m_Indices;
	};
}
