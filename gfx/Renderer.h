#pragma once

namespace gfx
{
	class Renderer
	{
	public:
		Renderer() {}
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) = delete;


		void Clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void Draw(const RenderObject& obj, const Shader& shader) const
		{
			shader.Bind();
			obj.Bind();
			glDrawElements(GL_TRIANGLES, obj.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		void Draw(const RenderObject& obj, const Texture& texture, const Shader& shader) const
		{
			obj.Bind();
			texture.Bind();
			shader.Bind();
			glDrawElements(GL_TRIANGLES, obj.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		void Draw(const RenderObject& obj, const Texture*const*const textures, uint count, const Shader& shader) const
		{
			obj.Bind();
			for (uint i = 0; i < count; i++)
				textures[i]->Bind(i);
			shader.Bind();
			glDrawElements(GL_TRIANGLES, obj.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		void Render(const OpenGLInstance& gl) const
		{
			glfwSwapBuffers(gl.window);
			glfwPollEvents();
		}
	};
}