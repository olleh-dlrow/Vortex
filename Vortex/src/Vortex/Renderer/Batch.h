#pragma once

#include"Vortex/Core/Core.h"

#include"Vertex.h"
#include"Renderer.h"
#include"BatchUnit.h"

namespace Vortex
{
	template<typename BU>
	class Batch
	{
		friend class LineRendererComponent;
		friend class PointRendererComponent;
		friend class Renderer;
	public:
		using V = typename BU::VertexType;

		Batch(int maxUnitCount = 10000)
			:m_MaxUnitCount(maxUnitCount), m_FreeVertexBufferBaseIndex(0)
		{
			Init();
		}

		Batch(const Ref<Shader>& shader, int maxUnitCount = 10000)
			:m_Shader(shader), m_MaxUnitCount(maxUnitCount), m_FreeVertexBufferBaseIndex(0)
		{
			Init();
		}

		~Batch() {}

		bool TryAddBatchUnit(const BU& unit)
		{
			if (m_FreeVertexBufferBaseIndex >= m_TempVertexBuffer.size())
			{
				return false;
			}
			for (int i = 0; i < BU::GetVertexCount(); i++)
			{
				AddVertexToCPU(unit.m_Vertices[i]);
			}
			return true;
		}

		int GetTempBufferBatchUnitCount() const { return m_FreeVertexBufferBaseIndex / BU::GetVertexCount(); }
		int GetTempBufferVertexCount() const { return m_FreeVertexBufferBaseIndex; }
		int GetTempBufferIndexCount() const { return GetTempBufferBatchUnitCount() * BU::GetIndexCount(); }

		virtual void Flush(const glm::mat4& VP)
		{
			// vertex buffer is empty, don't flush repeatly!
			if (m_FreeVertexBufferBaseIndex == 0)return;
			//VT_CORE_INFO("index: {}", m_FreeVertexBufferBaseIndex);
			m_VertexBuffer->SetData(&m_TempVertexBuffer[0], m_FreeVertexBufferBaseIndex * sizeof(V));

			m_VertexArray->Bind();
			m_Shader->Bind();
			m_Shader->SetMat4("u_ViewProjection", VP);
		
			Renderer::DrawIndexedTriangles(m_VertexArray, DrawTriangleConfig(m_FreeVertexBufferBaseIndex, GetTempBufferIndexCount()));

			m_FreeVertexBufferBaseIndex = 0;
		}

	protected:
		void Init()
		{
			// create vertex array
			m_VertexArray = VertexArray::Create();

			// create vertex buffer and set size(byte)
			m_VertexBuffer = VertexBuffer::Create(m_MaxVertexCount * sizeof(V));
			// init temp vertex buffer
			m_TempVertexBuffer = std::vector<V>(m_MaxVertexCount);
			// get layout of this vertex
			BufferLayout layout = V::GetLayout();
			m_VertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);

			if constexpr (BU::GetIndexCount() != 0)
			{
				// init IndexBuffer
				m_IndexBuffer = IndexBuffer::Create(m_MaxIndexCount);
				std::vector<uint32_t> tempIndexBuffer(m_MaxIndexCount);
				auto indices = BU::GetIndices();
				constexpr int stride = BU::GetIndexCount();

				int vertCnt = BU::GetVertexCount();
				for (int i = 0; i < m_MaxIndexCount / stride; i++)
				{
					for (int j = 0; j < stride; j++)
					{
						tempIndexBuffer[i * stride + j] = i * vertCnt + indices[j];
					}
				}
				m_IndexBuffer->SetData(&tempIndexBuffer[0], m_MaxIndexCount);

				m_VertexArray->SetIndexBuffer(m_IndexBuffer);
			}
		}

		void AddVertexToCPU(const V& vertex)
		{
			m_TempVertexBuffer[m_FreeVertexBufferBaseIndex++] = vertex;
		}

		Ref<VertexArray>		m_VertexArray;
		Ref<VertexBuffer>		m_VertexBuffer;
		Ref<IndexBuffer>		m_IndexBuffer;
		Ref<Shader>				m_Shader;

		std::vector<V>			m_TempVertexBuffer;
		int						m_FreeVertexBufferBaseIndex;

		const int				m_MaxUnitCount;
		const int				m_MaxVertexCount = m_MaxUnitCount * BU::GetVertexCount();
		const int				m_MaxIndexCount  = m_MaxUnitCount * BU::GetIndexCount();
	};
};