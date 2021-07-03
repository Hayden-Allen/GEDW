#pragma once

namespace gfx
{
	struct TextureOptions
	{
		GLenum min = GL_LINEAR, mag = GL_LINEAR, s = GL_CLAMP, t = GL_CLAMP;
		GLenum format = GL_RGBA, internalFormat = GL_RGBA8, type = GL_UNSIGNED_BYTE;
	};

	class Texture : public GLObject
	{
	public:
		Texture(uchar* data, uint width, uint height, uint frames, const TextureOptions& options = {})
		{
			glGenTextures(1, &m_Id);
			Bind();

			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, options.min);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, options.mag);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, options.s);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, options.t);

			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, options.internalFormat, width, height, frames, 0, options.format, options.type, data);
		}


		void Bind() const override
		{
			Bind(0);
		}
		void Bind(uint slot) const
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D_ARRAY, m_Id);
		}
		void Unbind() const override
		{
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}
	};
}
