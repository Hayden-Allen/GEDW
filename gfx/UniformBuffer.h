#pragma once

namespace gfx
{
	template<GLenum USAGE>
	class UniformBuffer : public Buffer<float, GL_UNIFORM_BUFFER, USAGE>
	{
	public:
		UniformBuffer(uint count, const float* data, uint slot) :
			Buffer<float, GL_UNIFORM_BUFFER, USAGE>(count),
			m_Slot(slot)
		{
			this->Write(count * sizeof(float), data);
		}
		UniformBuffer(const UniformBuffer& other) = delete;
		UniformBuffer(UniformBuffer&& other) = delete;
		~UniformBuffer()
		{
			printf("Delete UB %u\n", this->m_Id);
		}


		void Bind() const override
		{
			Buffer<float, GL_UNIFORM_BUFFER, USAGE>::Bind();
			// give the GPU access to our whole buffer (elements 0 through m_Count)
			glBindBufferRange(GL_UNIFORM_BUFFER, m_Slot, this->m_Id, 0, this->m_Count * sizeof(float));
		}
		uint GetSlot() const
		{
			return m_Slot;
		}
	private:
		uint m_Slot;
	};
}
