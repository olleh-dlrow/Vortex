#pragma once

#include"Vertex.h"

namespace Vortex
{
	template<typename T> class Batch;

	template<typename V, size_t VCNT, size_t ICNT>
	class BatchUnit
	{
	public:
		using VertexType = V;
		constexpr static int GetVertexCount() { return static_cast<int>(VCNT); }
		constexpr static int GetIndexCount() { return static_cast<int>(ICNT); }
	protected:
		std::array<V, VCNT>			m_Vertices;
	};




	template<typename V>
	class LinePoint : public BatchUnit<V, 1, 0>
	{
	public:

	};

	class LinePoint2 : public LinePoint<Vertex2>
	{
		friend class Batch<LinePoint2>;
	public:
		LinePoint2(const glm::vec3& p)
		{
			m_Vertices[0].position = p;
		}
	};





	template<typename V>
	class Triangle : public BatchUnit<V, 3, 0>
	{
	public:

	};

	class Triangle1 : public Triangle<Vertex1>
	{
		friend class Batch<Triangle1>;
	public:
		Triangle1(std::array<glm::vec3, 3> vertPos,
			      std::array<glm::vec4, 3> cols)
		{
			for (int i = 0; i < 3; i++)
			{
				m_Vertices[i].position = vertPos[i];
				m_Vertices[i].color = cols[i];
			}
		}
	};

	class Triangle3 : public Triangle<Vertex3>
	{
		friend class Batch<Triangle3>;
	public:
		
	};

	template<typename V>
	class Quad : public BatchUnit<V, 4, 6>
	{
	public:
		constexpr static std::array<uint32_t, 6> GetIndices()
		{
			return std::array<uint32_t, 6>({ 0, 1, 3, 1, 2, 3 });
		}
		constexpr static std::array<glm::f32vec3, 4> GetVertexPositions()
		{
			return std::array<glm::f32vec3, 4>({
				glm::f32vec3({ 0.5f,  0.5f, 0.0f }),
				glm::f32vec3({ 0.5f, -0.5f, 0.0f }),
				glm::f32vec3({-0.5f, -0.5f, 0.0f }),
				glm::f32vec3({-0.5f,  0.5f, 0.0f })
			});
		}
	};

	class Quad1 : public Quad<Vertex1>
	{
		friend class Batch<Quad1>;
	public:
		Quad1()
		{

		}
		Quad1(const glm::vec3& position,
			  const glm::vec2& size,
			  const glm::vec4& color)
		{
			for (int i = 0; i < 4; i++)
			{
				glm::mat4 I = glm::identity<glm::mat4>();
				glm::vec3 pos = glm::translate(I, position) * glm::scale(I, glm::vec3(size, 1)) * glm::vec4(GetVertexPositions()[i], 1);
				m_Vertices[i].position = pos;
				m_Vertices[i].color = color;
			}
		}
		constexpr static std::array<uint32_t, 6> GetIndices()
		{
			return Quad<Vertex1>::GetIndices();
		}
	};
};