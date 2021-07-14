#include "pch.h"
#include "Renderer.h"

namespace engine
{
	Renderer::Renderer(const char* fp, EngineInstance* engine) :
		m_Shaders{ fp },
		m_Engine(engine),
		m_TextureFrames(new int[gfx::getMaxTextureUnits()]),
		m_TextureBuffer(new gfx::Texture*[gfx::getMaxTextureUnits()]),
		m_LastTime(0.f),
		m_FrameDelta(0.f),
		m_AvgFPS(0.f)
	{
		// We need to send data to the "m_Textures" array of our shader. It's a weird "sampler2DArray" type, but to us all it is is a list of integers
		uint count = gfx::getMaxTextureUnits();
		int* samplers = new int[count];
		// we aren't interested in doing any fancy trickery by binding texture slots to different points in our sampler2DArray, so we just set the value at each index to the index itself
		for (uint i = 0; i < count; i++)
			samplers[i] = i;
		m_Shaders.SetUniform1iv("u_Textures", count, samplers);
		delete[] samplers;
	}



	void Renderer::Clear()
	{
		m_Renderer.Clear();

		// time since our program started running (ms)
		float time = CAST(float, glfwGetTime()) * 1000.f;
		// time since start of last frame (ms)
		m_FrameDelta = time - m_LastTime;
		// next frame, the current frame will be the last frame
		m_LastTime = time;
		// fps we're running at THIS FRAME
		float fps = 1000.f / m_FrameDelta;
		// average fps since we started our program (you might want to edit these constants depending on your machine, mine are weighted for a fast GPU)
		m_AvgFPS = fps * .0005f + m_AvgFPS * .9995f;

		// let's display our fps as our window title
		char buf[100];
		// Using sprintf causes a compiler error in MSVC. Add _CRT_SECURE_NO_WARNINGS to GEDW > Properties > C/C++ > Preprocessor > Preprocessor Definitions. For our case, this is totally safe because we know the size of the string we're writing. sprintf_s would be better, but is a WinAPI function so doesn't exist on Macs. Mac users should have no issues with this.
		sprintf(buf, "%07.2f | %07.2f", fps, m_AvgFPS);
		glfwSetWindowTitle(m_Engine->gl->window, buf);

		// update our scale in case our window has resized since last frame
		m_Shaders.SetUniform2f("u_Scale", m_Engine->gl->spwidth, m_Engine->gl->spheight);

		// YOU'LL NEED TO ENABLE THIS IF USING THE CONCENTRIC RINGS LIGHTING ALGORITHM
		// m_Shaders.SetUniform1f("u_PixelSize", m_Engine->gl->psize);
	}
}
