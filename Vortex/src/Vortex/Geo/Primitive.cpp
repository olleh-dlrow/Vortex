#include"vtpch.h"
#include"Primitive.h"

namespace Vortex 
{
	const Primitive Primitive::Square = Primitive(
		{
			{ 0.5f,  0.5f, 0.0f },
			{ 0.5f, -0.5f, 0.0f },
			{-0.5f, -0.5f, 0.0f },
			{-0.5f,  0.5f, 0.0f }
		},
		{
			{0, 1, 3},
			{1, 2, 3}
		});
};