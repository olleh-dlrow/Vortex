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
		constexpr static uint32_t GetVerticesSize() { return GetVertexCount() * sizeof(VertexType); }
	protected:
		std::array<V, VCNT>			m_Vertices;
	};

	/////////////////////////////////////////////////////////////////////////////
	// LinePoint //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	template<typename V>
	class LinePoint : public BatchUnit<V, 1, 0>
	{
	public:

	};

	class LinePoint2 : public LinePoint<Vertex2>
	{
		friend class Batch<LinePoint2>;
	public:
		LinePoint2() {}
		LinePoint2(const glm::vec3& p)
		{
			m_Vertices[0].position = p;
		}
	};


	/////////////////////////////////////////////////////////////////////////////
	// Triangle //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
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


	/////////////////////////////////////////////////////////////////////////////
	// Quad //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
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

	/////////////////////////////////////////////////////////////////////////////
	// Cube //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	template<typename V>
	class Cube : public BatchUnit<V, 36, 0>
	{
	public:

	};

	//class Cube3 : public Cube<Vertex3>
	//{
	//	friend class Batch<Cube3>;
	//public:
	//	Cube3()
	//	{

	//	}
	//	Cube3(const glm::vec3& position,
	//		  const glm::vec3& normal,
	//		  const glm::vec4& color,
	//		  const glm::vec2& texCoord,
	//		  const glm::vec3& tangent,
	//		  const glm::vec3& biTangent
	//		)
	//	{

	//	}
	//};

	class Cube4 : public Cube<Vertex4>
	{
		friend class Batch<Cube4>;
	public:
		Cube4()
		{

		}
		Cube4(const glm::vec3& position)
		{
			for (int i = 0; i < GetVertexCount(); i++)
			{
				constexpr int floatCnt = 8;
				float* vertices = GetVertices();
				glm::vec3 pos{vertices[i * floatCnt + 0], vertices[i * floatCnt + 1], vertices[i * floatCnt + 2] };
				glm::mat4 I = glm::identity<glm::mat4>();
				m_Vertices[i].position = glm::translate(I, position) * glm::vec4(pos, 1);
				m_Vertices[i].normal = glm::vec3{ vertices[i * floatCnt + 3], vertices[i * floatCnt + 4], vertices[i * floatCnt + 5] };
				m_Vertices[i].texCoord = glm::vec2{ vertices[i * floatCnt + 6], vertices[i * floatCnt + 7] };
			}
		}

		static float* GetVertices()
		{
			static float vertices[] = {
				// Back face
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
				0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
				-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
				// Front face
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
				0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
				-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
				// Left face
				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
				-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
				-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
				// Right face
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
				0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
				// Bottom face
				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
				0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
				-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
				// Top face
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
				0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
			};
			return vertices;
		}
	};
};