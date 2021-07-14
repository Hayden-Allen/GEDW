#pragma once

namespace gfx
{
	// combines a VA with an IB for convenience
	template<GLenum VA, GLenum IB>
	class RenderObject
	{
	public:
		RenderObject(uint count, const float* const vertices, const std::initializer_list<uint>& list, uint indicesPerPrimitive, uint verticesPerPrimitive, const uint* const offsets) :
			m_VertexArray(count, vertices, list),
			m_IndexBuffer(m_VertexArray, indicesPerPrimitive, verticesPerPrimitive, offsets)
		{}
		RenderObject(const RenderObject& other) = delete;
		RenderObject(RenderObject&& other) = delete;


		void Bind() const
		{
			m_VertexArray.Bind();
			m_IndexBuffer.Bind();
		}
		uint GetIndexCount() const
		{
			return m_IndexBuffer.GetCount();
		}
		VertexArray<VA>& GetVertexArray()
		{
			return m_VertexArray;
		}
	private:
		VertexArray<VA> m_VertexArray;
		IndexBuffer<IB> m_IndexBuffer;
	};
}
