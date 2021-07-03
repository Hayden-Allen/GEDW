#include "pch.h"

using namespace engine;

int main()
{
	// 800x600 window called "Title" with a solid blue background
	EngineInstance engine = init(800, 600, "ACM Game Engine Dev Workshop Series", { .clear = {.b = 1.f } });

	float vertices[] =
	{
		// bottom left
		-.5f, -.5f, 0.f, 0.f, 0.f,
		// bottom right
		 .5f, -.5f, 1.f, 0.f, 0.f,
		// top right
		 .5f,  .5f, 1.f, 1.f, 0.f,
		// top left
		-.5f,  .5f, 0.f, 1.f, 0.f
	};
	gfx::RenderObject ro(math::arrlen(vertices), vertices, { 2, 2, 1 }, s_IndicesPerQuad, s_VerticesPerQuad, s_IndexOffsets);

	const gfx::Renderer renderer;
	gfx::Shader shader("res/shader_texture.glsl");


	uint textureUnits = CAST(uint, gfx::getMaxTextureUnits());
	printf("%d\n", textureUnits);
	int* samplers = new int[textureUnits];
	for (uint i = 0; i < textureUnits; i++)
		samplers[i] = i;
	shader.SetUniform1iv("u_Textures", textureUnits, samplers);
	for (uint i = 0; i < textureUnits; i++)
		samplers[i] = 0;
	shader.SetUniform1iv("u_TextureFrames", textureUnits, samplers);


	constexpr uint dataWidth = 16, dataHeight = 16, dataFrames = 2;
	constexpr uint dataFrameSize = dataWidth * dataHeight * 4, dataSize = dataFrameSize * dataFrames;

	uchar data[dataSize] = { 0 };
	for (uint i = 0; i < dataWidth * dataHeight; i++)
	{
		data[i * 4 + 0] = 255;
		data[i * 4 + 3] = 255;
		data[dataFrameSize + i * 4 + 1] = 255;
		data[dataFrameSize + i * 4 + 3] = 255;
	}
	gfx::Texture tex(data, dataWidth, dataHeight, dataFrames, { .min = GL_NEAREST, .mag = GL_NEAREST });


	double updateTime = 0;
	int frame = 0;
	// game loop
	while (engine.IsRunning())
	{
		// clear the screen at the start of each frame
		renderer.Clear();

		double time = glfwGetTime();
		if (time - updateTime > 1)
		{
			samplers[0] = frame;
			frame = (frame + 1) % dataFrames;
			shader.SetUniform1iv("u_TextureFrames", textureUnits, samplers);
			updateTime = time;
		}

		// draw everything
		renderer.Draw(ro, tex, shader);

		renderer.Render(engine.gl);
	}


	delete[] samplers;
	// clean up
	end(engine);
	return 0;
}