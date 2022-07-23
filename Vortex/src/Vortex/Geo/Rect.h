#pragma once

namespace Vortex
{
	struct RectInt
	{
		RectInt() {}
		RectInt(int _x, int _y, int w, int h)
			:x(_x), y(_y), width(w), height(h)
		{}
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	};
}