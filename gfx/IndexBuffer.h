#pragma once

namespace gfx
{
	// IBO for use with glDrawElements
	template<GLenum USAGE>
	class IndexBuffer : public Buffer<uint, GL_ELEMENT_ARRAY_BUFFER, USAGE>
	{
	public:
		// take in a VAO and create as many indices as needed to draw the whole thing
		template<GLenum VA>
		IndexBuffer(const VertexArray<VA>& va, uint indicesPerPrimitive, uint verticesPerPrimitive, const uint* const offsets) :
			Buffer<uint, GL_ELEMENT_ARRAY_BUFFER, USAGE>(va.GetVertexCount() * indicesPerPrimitive / verticesPerPrimitive)
		{
			printf("Create IB %u\n", this->m_Id);

			uint* temp = new uint[this->m_Count];
			for (uint i = 0; i < this->m_Count; i++)
				// (current index offset) + (current quad) * (number of unique indices per quad)
				temp[i] = offsets[i % indicesPerPrimitive] + i / indicesPerPrimitive * verticesPerPrimitive;

			this->Write(this->m_Count * sizeof(uint), temp);
			delete[] temp;
		}
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) = delete;
		~IndexBuffer()
		{
			printf("Delete IB %u\n", this->m_Id);
		}
	};
}
