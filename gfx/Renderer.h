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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		template<GLenum VA, GLenum IB>
		void Draw(const VertexArray<VA>& va, const IndexBuffer<IB>& ib, const Texture* const* const textures, uint count, const Shader& shader) const
		{
			DrawBase(textures, count, ib.GetCount(), shader, va, ib);
		}
		template<GLenum VA, GLenum IB>
		void Draw(const RenderObject<VA, IB>& obj, const Texture* const* const textures, uint count, const Shader& shader) const
		{
			DrawBase(textures, count, obj.GetIndexCount(), shader, obj);
		}
		void Render(const OpenGLInstance& gl) const
		{
			glfwSwapBuffers(gl.window);
			glfwPollEvents();
		}
	private:
		template<typename ... Args>
		void DrawBase(const Texture* const* const textures, uint textureCount, uint indexCount, const Shader& shader, const Args& ... args) const
		{
			(args.Bind(), ...);
			shader.Bind();
			for (uint i = 0; i < textureCount; i++)
				if(textures[i])
					textures[i]->Bind(i);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
		}
	};
}
