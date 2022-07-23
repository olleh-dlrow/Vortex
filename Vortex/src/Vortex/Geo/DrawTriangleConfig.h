#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace Vortex
{
	enum class PolygonMode
	{
		POINT,
		LINE,
		FILL
	};

	enum class PrimitiveType
	{
		POINTS,
		LINE_STRIP,
		LINE_LOOP,
		LINES,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANLGE_FAN
	};

	struct DrawTriangleConfig
	{
		DrawTriangleConfig() {}
		DrawTriangleConfig(int verticesCount, int indicesCount, 
			PolygonMode _polygonMode = PolygonMode::FILL, PrimitiveType triMode = PrimitiveType::TRIANGLES)
			:triangleMode(triMode), polygonMode(_polygonMode), vertCnt(verticesCount), indexCnt(indicesCount)
		{

		}
		PrimitiveType	triangleMode = PrimitiveType::TRIANGLES;
		PolygonMode		polygonMode  = PolygonMode::FILL;
		int				vertCnt		 = 0;
		int				indexCnt	 = 0;
	};
}
