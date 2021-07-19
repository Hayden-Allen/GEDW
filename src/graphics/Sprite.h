#pragma once
#include "pch.h"

namespace engine
{
	class Sprite
	{
	public:
		Sprite(const std::string& fp, uint frames, uint frameTime);
		Sprite(const Sprite& other) = delete;
		Sprite(Sprite&& other) = delete;
		~Sprite()
		{
			delete m_Texture;
		}


		void Update(float time);
		gfx::Texture* GetTexture()
		{
			return m_Texture;
		}
		uint GetCurrentFrame() const
		{
			return m_CurrentFrame;
		}
		uint GetWidth() const
		{
			return m_Width;
		}
		uint GetHeight() const
		{
			return m_Height;
		}
		math::Vec2<float> GetDims() const
		{
			return { 1.f * m_Width, 1.f * m_Height };
		}
	private:
		constexpr static uint s_BitmapInfoSize = 54, s_BitmapInfoWidth = 18, s_BitmapInfoHeight = 22, s_BytesPerPixel = 3, s_BitmapWidthStep = 4;
		uint m_Width, m_Height, m_Frames, m_CurrentFrame, m_TimePerFrame;
		float m_LastFrameSwitch;
		gfx::Texture* m_Texture;
	};
}
