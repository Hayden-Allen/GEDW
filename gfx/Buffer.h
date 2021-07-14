#pragma once

namespace gfx
{
	// base class for vertex, index, and uniform buffers
	// TARGET is GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
	// or GL_UNIFORM_BUFFER accordingly
	template<typename T, GLenum TARGET, GLenum USAGE>
	class Buffer : public GLObject
	{
	public:
		void Bind() const override
		{
			glBindBuffer(TARGET, m_Id);
		}
		void Unbind() const override
		{
			glBindBuffer(TARGET, 0);
		}
		uint GetCount() const
		{
			return m_Count;
		}
		void Update(uint count, const T* const data, uint offset)
		{
			Bind();
			glBufferSubData(TARGET, offset * sizeof(T), count * sizeof(T), data);
		}
	protected:
		// number of elements in this buffer
		uint m_Count;


		Buffer(uint count) :
			m_Count(count)
		{
			glGenBuffers(1, &m_Id);
		}
		Buffer(const Buffer& other) = delete;
		Buffer(Buffer&& other) = delete;
		virtual ~Buffer()
		{
			glDeleteBuffers(1, &m_Id);
		}


		// Write data into the buffer. Note that this should be used for
		// INITIAL WRITE ONLY; glBufferData allocates a new memory block, so
		// glBufferSubData should be used for updating existing data (we'll
		// write a different method for this eventually)
		void Write(uint size, const T* const data)
		{
			Bind();
			glBufferData(TARGET, size, data, USAGE);
		}
	};
}
