#include "pch.h"
#include "Sprite.h"

namespace engine
{
	Sprite::Sprite(const std::string& fp, uint frames, uint frameTime) :
		m_Width(0),
		m_Height(0),
		m_Frames(frames),
		m_CurrentFrame(0),
		m_TimePerFrame(frameTime),
		m_LastFrameSwitch(0.f),
		m_Texture(nullptr)
	{

	}



	void Sprite::Update(float time)
	{
		if (m_Frames > 1 && time - m_LastFrameSwitch >= m_TimePerFrame)
		{
			m_LastFrameSwitch = time;
			m_CurrentFrame = (m_CurrentFrame + 1) % m_Frames;
		}
	}
}
