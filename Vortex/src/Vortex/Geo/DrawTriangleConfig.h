#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace Vortex
{
	struct DrawTriangleConfig
	{
		DrawTriangleConfig(uint32_t verticesCount, uint32_t indicesCount, uint32_t m = GL_FILL)
			:polygonMode(m), vertCnt(verticesCount), indexCnt(indicesCount)
		{

		}
		GLenum polygonMode;
		uint32_t vertCnt;
		uint32_t indexCnt;
	};
}
