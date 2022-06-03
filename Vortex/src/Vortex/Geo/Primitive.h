#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Vortex
{
	class Primitive
	{
	public:
		Primitive() {}
		Primitive(const std::vector<glm::f32vec3>& v, const std::vector<glm::i32vec3>& i)
			:vertices(v), indices(i) {}
		
		std::vector<glm::f32vec3> vertices;
		std::vector<glm::i32vec3> indices;

		const static Primitive Square;
	};
}
