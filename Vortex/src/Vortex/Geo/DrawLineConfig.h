#pragma once

#include <glad/glad.h>
#include <cstdint>
#include "glm/glm.hpp"

namespace Vortex
{
	struct DrawLineConfig
	{
		DrawLineConfig(uint32_t cnt, float w = 1.0f, const glm::vec3& c=glm::vec3(1.0f, 0.5f, 0.2f), uint32_t m = GL_LINE_STRIP)
			:mode(m), width(w), pointCount(cnt), color(glm::vec4(c, 1.0f))
		{

		}

		DrawLineConfig(uint32_t cnt, float w = 1.0f, const glm::vec4& c = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f), uint32_t m = GL_LINE_STRIP)
			:mode(m), width(w), pointCount(cnt), color(c)
		{

		}

		GLenum mode;
		GLfloat width;
		uint32_t pointCount;
		glm::vec4 color;
	};
}

