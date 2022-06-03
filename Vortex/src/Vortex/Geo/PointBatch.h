#pragma once

#include<vector>
#include<glm/glm.hpp>
#include"Primitive.h"

namespace Vortex
{
	class PointBatch
	{
	public:
		PointBatch(const glm::vec3& c = glm::vec3(0.2f, 0.8f, 0.3f), 
			       float sz = 0.15f, 
			       const Primitive& p=Primitive::Square)
			:m_Primitive(p), m_Color(c), m_PointSize(sz) {}
		~PointBatch() {}

		inline glm::vec3& GetPos(int index) { return m_Positions[index]; }
		inline int GetPrimitiveCount() const { return (int)m_Positions.size(); }
		inline void AddPoint(const glm::vec3& pos) { m_Positions.emplace_back(pos); }
		
		inline int GetPrimitiveVerticesCount() const { return (int)m_Primitive.vertices.size(); }
		inline int GetPrimitiveTriangleCount() const { return (int)m_Primitive.indices.size(); }

		inline int GetPrimitiveBytes() const { return (int)m_Primitive.vertices.size() * sizeof(glm::vec3); }
		inline int GetPrimitiveIndicesCount() const { return (int)m_Primitive.indices.size() * 3; }
		inline const glm::f32vec3& GetPrimitiveVertex(int index) const { return m_Primitive.vertices[index]; }
		inline const glm::i32vec3& GetPrimitiveIndex(int index) const { return m_Primitive.indices[index]; }
		inline int GetAllVerticesBytes() const { return GetPrimitiveBytes() * GetPrimitiveCount(); }
		inline int GetAllIndicesBytes() const { return GetPrimitiveIndicesCount() * GetPrimitiveCount() * sizeof(uint32_t); }
		inline int GetAllIndicesCount() const { return GetPrimitiveCount() * GetPrimitiveIndicesCount(); }

		inline float& GetPointSize() { return m_PointSize; }
		inline glm::vec3& GetPointColor() { return m_Color; }

	private:
		std::vector<glm::vec3> m_Positions;
		glm::vec3 m_Color;
		float m_PointSize;
		const Primitive& m_Primitive;
	};	
};
