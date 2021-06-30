#pragma once

namespace gfx
{
	// IBO for use with glDrawElements
	class IndexBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER>
	{
	public:
		// take in a VAO and create as many indices as needed to draw the whole thing
		IndexBuffer(const VertexArray& va, uint indicesPerPrimitive, uint verticesPerPrimitive, const uint* const offsets) :
			Buffer<GL_ELEMENT_ARRAY_BUFFER>(va.GetVertexCount() * indicesPerPrimitive / verticesPerPrimitive)
		{
			printf("Create IB %u\n", m_Id);

			uint* temp = new uint[m_Count];
			for (uint i = 0; i < m_Count; i++)
				// (current index offset) + (current quad) * (number of unique indices per quad)
				temp[i] = offsets[i % indicesPerPrimitive] + i / indicesPerPrimitive * verticesPerPrimitive;

			Write(m_Count * sizeof(uint), temp);
			delete[] temp;
		}
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) = delete;
		~IndexBuffer()
		{
			printf("Delete IB %u\n", m_Id);
		}
	};
}
