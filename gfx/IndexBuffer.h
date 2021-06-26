#pragma once

namespace gfx
{
	// IBO for use with glDrawElements
	class IndexBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER>
	{
	public:
		// take in a VAO and create as many indices as needed to draw the whole thing
		IndexBuffer(const VertexArray& va) :
			Buffer<GL_ELEMENT_ARRAY_BUFFER>(va.GetVertexCount() * s_IndicesPerQuad / s_VerticesPerQuad)
		{
			printf("Create IB %u\n", m_Id);

			uint* temp = new uint[m_Count];
			for (uint i = 0; i < m_Count; i++)
				// (current index offset) + (current quad) * (number of unique indices per quad)
				temp[i] = s_Offsets[i % 6] + i / s_IndicesPerQuad * s_VerticesPerQuad;

			Write(m_Count * sizeof(uint), temp);
			delete[] temp;
		}
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) = delete;
		~IndexBuffer()
		{
			printf("Delete IB %u\n", m_Id);
		}
	private:
		// these shouldn't be hard coded in our graphics library, but we'll leave them for now
		constexpr static uint s_IndicesPerQuad = 6, s_VerticesPerQuad = 4;
		constexpr static uint s_Offsets[] = { 0, 1, 2, 0, 2, 3 };
	};
}
