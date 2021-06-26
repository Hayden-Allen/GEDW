#pragma once

namespace gfx
{
	// VBO
	class VertexBuffer : public Buffer<GL_ARRAY_BUFFER>
	{
	public:
		VertexBuffer(uint count, const float* const data) :
			Buffer<GL_ARRAY_BUFFER>(count)
		{
			printf("Create VB %u\n", m_Id);
			Write(count * sizeof(float), data);
		}
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) = delete;
		~VertexBuffer()
		{
			printf("Delete VB %u\n", m_Id);
		}
	};
}
