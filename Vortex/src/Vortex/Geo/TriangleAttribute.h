#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace Vortex
{
	struct TriangleAttribute
	{
		TriangleAttribute(uint32_t m = GL_FILL)
			:polygonMode(m)
		{

		}
		GLenum polygonMode;
	};
}
