#pragma once

#include <glad/glad.h>
#include <cstdint>
#include "glm/glm.hpp"

namespace Vortex
{
	struct PointAttribute
	{
		PointAttribute(uint32_t cnt, float sz = -1.0f, const glm::vec3& c=glm::vec3(1.0f, 0.5f, 0.2f), uint32_t m = GL_POINTS)
			:mode(m), size(sz), count(cnt), color(c)
		{

		}
		GLenum mode;
		GLfloat size;	// < 0 mean no size
		uint32_t count;
		glm::vec3 color;
	};
}

