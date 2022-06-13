#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Vortex
{
	struct Vertex0
	{
		Vertex0() = default;
		Vertex0(const glm::f32vec3& p, 
			    const glm::f32vec4& c,
			    const glm::f32vec2& tex,
			    const glm::f32vec3& n)
		:position(p), color(c), texcoord(tex), normal(n) {}

		glm::f32vec3 position;
		glm::f32vec4 color;
		glm::f32vec2 texcoord;
		glm::f32vec3 normal;
	};

	struct Vertex1
	{
		Vertex1() = default;
		Vertex1(const glm::f32vec3& p, 
			    const glm::f32vec4& c)
		:position(p), color(c) {}

		glm::f32vec3 position;
		glm::f32vec4 color;

		static BufferLayout GetLayout()
		{
			return BufferLayout({
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}
			});
		}
	};

	struct Vertex2
	{
		Vertex2() = default;
		Vertex2(const glm::f32vec3& p)
			:position(p) {}

		glm::f32vec3 position;

		static BufferLayout GetLayout()
		{
			return BufferLayout({
				{ShaderDataType::Float3, "a_Position"},
				});
		}
	};
};
