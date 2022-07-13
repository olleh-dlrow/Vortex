#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Vortex
{
	// position, color, texcoord, normal
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

	// position, color
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

	// position
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

	// position, normal, color, texCoord, tangent, biTangent
	struct Vertex3
	{
		Vertex3() = default;
		
		glm::f32vec3 position;
		glm::f32vec3 normal;
		glm::f32vec4 color;
		glm::f32vec2 texCoord;
		glm::f32vec3 tangent;	// u vector
		glm::f32vec3 biTangent; // v vector
	};

	struct MeshVertex
	{
		MeshVertex() = default;

		glm::f32vec3 position;
		glm::f32vec3 normal;
		glm::f32vec4 color;
		glm::f32vec2 texCoord;
		glm::f32vec3 tangent;	// u vector
		glm::f32vec3 biTangent; // v vector
	};
};
