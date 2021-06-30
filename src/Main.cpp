#include "pch.h"

using namespace engine;

int main()
{
	// 800x600 window called "Title" with a solid blue background
	gfx::OpenGLInstance gl = gfx::init(800, 600, "ACM Game Engine Dev Workshop Series", { .clear = {.b = 1.f } });


	float vertices[] =
	{
		// bottom left
		-.5f, -.5f, 1.f, 0.f, 0.f,
		// bottom right
		 .5f, -.5f, 0.f, 1.f, 0.f,
		// top right
		 .5f,  .5f, 0.f, 0.f, 1.f,
		// top left
		-.5f,  .5f, 1.f, 1.f, 1.f
	};
	gfx::RenderObject ro(math::arrlen(vertices), vertices, { 2, 3 }, s_IndicesPerQuad, s_VerticesPerQuad, s_IndexOffsets);

	const gfx::Renderer renderer;
	gfx::Shader shader("res/shader.glsl");

	float offset = 0.f, increment = .0005f;
	// game loop
	while (gl.IsRunning())
	{
		// clear the screen at the start of each frame
		renderer.Clear();

		if (math::abs(offset) >= 1.f)
			increment *= -1;
		offset += increment;
		shader.SetUniform2f("u_Camera", offset, offset);

		// draw everything
		renderer.Draw(ro, shader);

		renderer.Render(gl);
	}


	// clean up
	gfx::end(gl);
	return 0;
}