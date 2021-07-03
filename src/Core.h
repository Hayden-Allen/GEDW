#pragma once

namespace engine
{
	constexpr static uint s_VerticesPerQuad = 4, s_IndicesPerQuad = 6;
	constexpr static uint s_IndexOffsets[s_IndicesPerQuad] = { 0, 1, 2, 0, 2, 3 };


	struct EngineInstance
	{
		gfx::OpenGLInstance gl;

		bool IsRunning() const { return gl.IsRunning(); }
	};


	static EngineInstance init(uint width, uint height, const char* title, const gfx::WindowOptions& options = {})
	{
		return { gfx::init(width, height, title, options) };
	}
	static void end(EngineInstance& instance)
	{
		gfx::end(instance.gl);
	}
}
