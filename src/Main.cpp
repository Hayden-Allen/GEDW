#include "pch.h"
#include "graphics/Sprite.h"

using namespace engine;

int main()
{
	EngineInstance engine = init(800, 600, "ACM Game Engine Dev Workshop Series", { .resizable = true, .pixelSize = 2.f, .clear = {.b = 1.f } });

	const gfx::Renderer renderer;
	gfx::Shader shader("res/shader_texture.glsl");


	Sprite sprite("res/test.bmp", 2, 500);

	// how many tiles we want in the x and y axes, total tile count
	constexpr uint tileWidth = 100, tileHeight = 100, tileCount = tileWidth * tileHeight;
	// number of floats for each tile, number of floats for whole grid
	constexpr uint floatsPerTile = s_VerticesPerQuad * s_FloatsPerVertex, floatCount = tileCount * floatsPerTile;
	// position/texture coord offsets
	// read as {0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}
	// the same counter-clockwise from bottom left configuration we've been using
	constexpr float offsets[] = { 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f };

	// our actual vertex data that will be sent to the GPU
	float* vertices = new float[floatCount];
	for (uint i = 0; i < tileCount; i++)
	{
		// starting index of current tile in the vertex array
		const uint offset = i * floatsPerTile;
		// x and y coords within our grid of current tile 
		const uint x = i % tileWidth, y = i / tileWidth;
		// create 4 vertices for each tile
		for (uint j = 0; j < s_VerticesPerQuad; j++)
		{
			// starting index of current vertex in the vertex array
			const uint vertexOffset = offset + j * s_FloatsPerVertex;
			// x, y
			// this just uses offsets array to put each corner of the tile in the correct place
			// base coordinates are x and y (defined above), the position within our grid of the current tile
			vertices[vertexOffset + 0] = (x + offsets[j * 2 + 0]) * sprite.GetWidth();
			vertices[vertexOffset + 1] = (y + offsets[j * 2 + 1]) * sprite.GetHeight();
			// s, t
			// offset array can be used directly for texture indices, since our Sprites always use their full texture
			vertices[vertexOffset + 2] = offsets[j * 2 + 0];
			vertices[vertexOffset + 3] = offsets[j * 2 + 1];
			// hard code texture index as 0 for now (we aren't batching anything yet)
			vertices[vertexOffset + 4] = 0.f;
		}
	}
	// send data to GPU then delete it from the CPU
	gfx::RenderObject ro(floatCount, vertices, { 2, 2, 1 }, s_IndicesPerQuad, s_VerticesPerQuad, s_IndexOffsets);
	delete[] vertices;


	// game loop
	while (engine.IsRunning())
	{
		// clear the screen at the start of each frame
		renderer.Clear();

		// get current time in ms
		float time = CAST(float, glfwGetTime() * 1000.f);
		// switch our sprite's frame if necessary
		sprite.Update(time);
		// update frame on the gpu
		int frame = sprite.GetCurrentFrame();
		shader.SetUniform1iv("u_TextureFrames", 1, &frame);
		// update scale in case our window was resized since last frame
		shader.SetUniform2f("u_Scale", engine.gl->spwidth, engine.gl->spheight);

		// draw everything
		renderer.Draw(ro, *sprite.GetTexture(), shader);

		renderer.Render(*engine.gl);
	}

	// clean up
	end(engine);
	return 0;
}