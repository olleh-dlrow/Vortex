#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace Vortex
{
	struct DrawTriangleConfig
	{
		DrawTriangleConfig() {}
		DrawTriangleConfig(int verticesCount, int indicesCount, uint32_t _polygonMode = GL_FILL, GLenum triMode = GL_TRIANGLES)
			:triangleMode(triMode), polygonMode(_polygonMode), vertCnt(verticesCount), indexCnt(indicesCount)
		{

		}
		GLenum  triangleMode = GL_TRIANGLES;
		GLenum	polygonMode  = GL_FILL;
		GLsizei vertCnt = 0;
		GLsizei indexCnt = 0;
	};
}
