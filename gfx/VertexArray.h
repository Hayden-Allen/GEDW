#pragma once

namespace gfx
{
	// VAO - stores a VBO and a description of that VBO
	template<GLenum USAGE>
	class VertexArray : public GLObject
	{
	public:
		VertexArray(uint count, const std::initializer_list<uint>& list) :
			m_Buffer(count),
			m_VertexSize(std::reduce(list.begin(), list.end(), 0, std::plus<uint>()))
		{
			Init(list);
		}
		VertexArray(uint count, const float* const vertices, const std::initializer_list<uint>& list) :
			m_Buffer(count, vertices),
			// the sum of all elements in list
			m_VertexSize(std::reduce(list.begin(), list.end(), 0, std::plus<uint>()))
		{
			Init(list);
		}
		VertexArray(const VertexArray& other) = delete;
		VertexArray(VertexArray&& other) = delete;
		~VertexArray()
		{
			printf("Delete VA %u\n", m_Id);
			glDeleteVertexArrays(1, &m_Id);
		}


		void Bind() const override
		{
			glBindVertexArray(m_Id);
		}
		void Unbind() const override
		{
			glBindVertexArray(0);
		}
		// number of vertices in the VBO
		uint GetVertexCount() const
		{
			// (number of elements) / (number of elements per vertex)
			return m_Buffer.GetCount() / m_VertexSize;
		}
		VertexBuffer<USAGE>& GetBuffer()
		{
			return m_Buffer;
		}
	private:
		VertexBuffer<USAGE> m_Buffer;
		uint m_VertexSize;



		void Init(const std::initializer_list<uint>& list)
		{
			glGenVertexArrays(1, &m_Id);
			printf("Create VA %u\n", m_Id);

			Bind();
			m_Buffer.Bind();

			// byte size of a vertex
			const uint stride = m_VertexSize * sizeof(float);
			// number of elements that have been processed, attrib array index
			uint offset = 0, i = 0;
			// for each element `n` in `list`
			for (uint n : list)
			{
				// enable next attrib array
				glEnableVertexAttribArray(i);
				// current group is comprised of `n` floats
				glVertexAttribPointer(i, n, GL_FLOAT, GL_FALSE, stride, (const void*)(offset * sizeof(float)));
				// add current number of elements
				offset += n;
				// increment attrib array index
				i++;
			}
		}
	};
}
