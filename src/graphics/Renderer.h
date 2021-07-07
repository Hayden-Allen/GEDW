#pragma once
#include "pch.h"

namespace engine
{
	class Sprite;

	class Renderer
	{
	public:
		Renderer(const char* fp, EngineInstance* engine);
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) = delete;
		~Renderer()
		{
			delete[] m_TextureFrames;
			delete[] m_TextureBuffer;
		}


		void Clear();
		void Draw(const gfx::RenderObject& ob, const std::vector<Sprite*>& sprites);
		void SetCamera(const math::Vec2<float>& camera)
		{
			m_Shaders.SetUniform2f("u_Camera", camera.x, camera.y);
		}
		float GetFrameDelta() const
		{
			// return in seconds for convenience
			return m_FrameDelta / 1000.f;
		}
		float GetTime() const
		{
			return m_LastTime;
		}
		void Render() const
		{
			// reset scroll state manually before we poll events because the scroll callback is not triggered when you stop scrolling
			m_Engine->gl->scroll = { 0.f, 0.f };
			m_Renderer.Render(*m_Engine->gl);
		}
	private:
		// For now, we're only using a single shader. Eventually we'll be using multiple, and we'll want to assign to the same uniforms with them at the same time.
		struct Shaders
		{
			gfx::Shader statics;


			void SetUniform1iv(const char* name, uint count, const int* const data)
			{
				statics.SetUniform1iv(name, count, data);
			}
			void SetUniform1f(const char* name, float f)
			{
				statics.SetUniform1f(name, f);
			}
			void SetUniform2f(const char* name, float x, float y)
			{
				statics.SetUniform2f(name, x, y);
			}
		};


		Shaders m_Shaders;
		gfx::Renderer m_Renderer;
		// Store a pointer to our EngineInstance. I can't think of a scenario where you would use a Renderer with multiple EngineInstances (or even create multiple EngineInstances in the first place), so this should be fine to do. Just saves us from having to pass it as a parameter to some of our member functions.
		EngineInstance* m_Engine;
		// buffers allocated in the constructor and used every frame to send data to the GPU. Dynamic allocation is slow, so it's best to do this once and store it then do it again every frame.
		int* m_TextureFrames;
		gfx::Texture** m_TextureBuffer;
		// time at the start of last frame (ms), time between start of last frame and start of current frame (ms), average fps of our program since it started running
		float m_LastTime, m_FrameDelta, m_AvgFPS;
	};
}
