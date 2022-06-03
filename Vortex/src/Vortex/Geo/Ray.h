#pragma once

#include<glm/glm.hpp>

namespace Vortex
{
	struct Ray
	{
		Ray():origin(glm::vec3(0,0,0)), direction(glm::vec3(0,0,-1)) {}
		Ray(const glm::vec3& o, const glm::vec3& normDir)
			:origin(o), direction(normDir) {}
		glm::vec3 origin;
		// normalized direction
		glm::vec3 direction;
	};
};
