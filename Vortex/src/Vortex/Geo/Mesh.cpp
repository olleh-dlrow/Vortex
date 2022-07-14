#include "vtpch.h"
#include "Mesh.h"


namespace Vortex
{
	Mesh::Mesh()
	{

	}

	Mesh::Mesh(const std::string& name):m_Name(name){}

	std::string Mesh::StatisticVertex()
	{
		// statistic the range of vertices
		int range = 10;
		// left_border, right_border, cnt
		std::vector< std::tuple<glm::vec3, glm::vec3, glm::vec3>  > bars(range);
		constexpr float fmin = std::numeric_limits<float>::min(), fmax = std::numeric_limits<float>::max();
		// get min, max
		glm::vec3 minV{fmax, fmax, fmax}, maxV{fmin, fmin, fmin};
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				minV[j] = std::min(m_Vertices[i].position[j], minV[j]);
				maxV[j] = std::max(m_Vertices[i].position[j], maxV[j]);
			}
		}
		// set range
		glm::vec3 s = minV, step = (maxV - minV) / (float)range;
		for (int i = 0; i < range; i++)
		{
			std::get<0>(bars[i]) = s;
			std::get<1>(bars[i]) = s + step;
			std::get<2>(bars[i]) = glm::vec3(0);
			s += step;
		}

		// statistic
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			// which bar in x, y, z
			glm::vec3 idx = (m_Vertices[i].position - minV) / step;
			idx = glm::clamp(idx, glm::vec3(0), glm::vec3(range - 1, range - 1, range - 1));
			std::get<2>(bars[idx[0]])[0]++;
			std::get<2>(bars[idx[1]])[1]++;
			std::get<2>(bars[idx[2]])[2]++;
		}

		// output 
		std::ostringstream builder;

		builder << "Mesh Vertex Statistics: " << m_Name << "\n";
		builder << "min: " << minV[0] << " " << minV[1] << " " << minV[2] << "\n";
		builder << "max: " << maxV[0] << " " << maxV[1] << " " << maxV[2] << "\n";


		auto func = [&](const char* str, int idx) {
			builder << str;
			for (int i = 0; i < bars.size(); i++)
			{
				builder << "(" << std::get<0>(bars[i])[idx] << ", " << std::get<1>(bars[i])[idx] << "): " << std::get<2>(bars[i])[idx] << "\n";
			}
		};

		func("X\n", 0);
		func("Y\n", 1);
		func("Z\n", 2);

		return builder.str();
	}
	std::string Mesh::StatisticIndex()
	{
		return std::string();
	}
}