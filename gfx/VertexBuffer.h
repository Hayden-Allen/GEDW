#pragma once

namespace gfx
{
	// VBO
	template<GLenum USAGE>
	class VertexBuffer : public Buffer<float, GL_ARRAY_BUFFER, USAGE>
	{
	public:
		VertexBuffer(uint count) :
			Buffer<float, GL_ARRAY_BUFFER, USAGE>(count)
		{
			printf("Create VB %u\n", this->m_Id);
			float* temp = new float[count];
			for (uint i = 0; i < count; i++)
				temp[i] = 0.f;
			this->Write(count * sizeof(float), temp);
			delete[] temp;
		}
		VertexBuffer(uint count, const float* const data) :
			Buffer<float, GL_ARRAY_BUFFER, USAGE>(count)
		{
			printf("Create VB %u\n", this->m_Id);
			this->Write(count * sizeof(float), data);
		}
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) = delete;
		~VertexBuffer()
		{
			printf("Delete VB %u\n", this->m_Id);
		}
	};
}
