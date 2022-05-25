#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace Vortex
{
	struct TriangleAttribute
	{
		TriangleAttribute(uint32_t cnt, uint32_t m = GL_FILL)
			:polygonMode(m), pointCnt(cnt)
		{

		}
		GLenum polygonMode;
		uint32_t pointCnt;
	};
}
