#pragma once

namespace gfx
{
	class UniformBuffer : public Buffer<GL_UNIFORM_BUFFER>
	{
	public:
		UniformBuffer(uint count, const float* data, uint slot) :
			Buffer<GL_UNIFORM_BUFFER>(count),
			m_Slot(slot)
		{
			Write(count * sizeof(float), data);
		}
		UniformBuffer(const UniformBuffer& other) = delete;
		UniformBuffer(UniformBuffer&& other) = delete;
		~UniformBuffer()
		{
			printf("Delete UB %u\n", m_Id);
		}


		void Bind() const override
		{
			Buffer<GL_UNIFORM_BUFFER>::Bind();
			// give the GPU access to our whole buffer (elements 0 through m_Count)
			glBindBufferRange(GL_UNIFORM_BUFFER, m_Slot, m_Id, 0, m_Count * sizeof(float));
		}
		uint GetSlot() const
		{
			return m_Slot;
		}
	private:
		uint m_Slot;
	};
}
