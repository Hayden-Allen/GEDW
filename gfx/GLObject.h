#pragma once

namespace gfx
{
	// base class for most of our OpenGL wrapper classes
	class GLObject
	{
	public:
		// default constructor used because m_Id will be assigned to in different ways
		GLObject() : m_Id(0) {}
		GLObject(const GLObject& other) = delete;
		GLObject(GLObject&& other) = delete;


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		uint GetId() const
		{
			return m_Id;
		}
	protected:
		// generic OpenGL "pointer"
		uint m_Id;
	};
}
