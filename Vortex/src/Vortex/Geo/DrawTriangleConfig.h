#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace Vortex
{
	struct DrawTriangleConfig
	{
		DrawTriangleConfig() {}
		DrawTriangleConfig(int verticesCount, int indicesCount, uint32_t m = GL_FILL)
			:polygonMode(m), vertCnt(verticesCount), indexCnt(indicesCount)
		{

		}
		GLenum	polygonMode;
		GLsizei vertCnt;
		GLsizei indexCnt;
	};
}
