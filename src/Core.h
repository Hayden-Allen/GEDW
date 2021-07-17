#pragma once

namespace engine
{
	constexpr static uint s_VerticesPerQuad = 4, s_IndicesPerQuad = 6, s_FloatsPerVertex = 6, s_FloatsPerQuad = s_FloatsPerVertex * s_VerticesPerQuad, s_FloatsPerDynamicVertex = 6, s_FloatsPerDynamic = s_FloatsPerDynamicVertex * s_VerticesPerQuad;
	constexpr static uint s_IndexOffsets[s_IndicesPerQuad] = { 0, 1, 2, 0, 2, 3 };
	constexpr static float s_CornerPoints[] = { 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f };


	struct EngineInstance
	{
		// store a pointer now so that resize functionality works with our window user pointer
		gfx::OpenGLInstance* gl;

		bool IsRunning() const { return gl->IsRunning(); }
		bool IsKeyPressed(uint key) const { return gl->IsKeyPressed(key); }
		bool IsMouseLeft() const { return gl->IsMouseLeft(); }
		bool IsMouseRight() const { return gl->IsMouseRight(); }
		math::Vec2<float> GetCursorPos() const
		{
			auto temp = gl->GetCursorPos();
			return { temp.x, temp.y };
		}
		math::Vec2<float> GetScroll() const
		{
			auto temp = gl->scroll;
			return { temp.x, temp.y };
		}
		void SetClearColor(float r, float g, float b) { gl->SetClearColor(r, g, b); }
		void SetPixelSize(float size) { gl->SetPixelSize(size); }
	};


	static EngineInstance init(uint width, uint height, const char* title, const gfx::WindowOptions& options = {})
	{
		return { gfx::init(width, height, title, options) };
	}
	static void end(EngineInstance& instance)
	{
		gfx::end(*instance.gl);
		// our OpenGLInstance was dynamically allocated by gfx::init, so we need to free it when we're done
		delete instance.gl;
	}
}
