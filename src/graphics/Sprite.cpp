#include "pch.h"
#include "Sprite.h"
#include "io/InputFile.h"

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
		if (frames == 0)
		{
			printf("Sprite must have at least 1 frame\n");
			return;
		}

		if (fp.compare(fp.size() - 4, 4, ".bmp") != 0)
		{
			printf("Only .bmp files are supported\n");
			return;
		}

		InputFile in(fp.c_str());

		uchar info[s_BitmapInfoSize];
		in.Read(info, s_BitmapInfoSize);

		m_Width = PUN(uint, info[s_BitmapInfoWidth]);
		m_Height = PUN(uint, info[s_BitmapInfoHeight]) / m_Frames;
		

		const uint size = ((m_Width / s_BitmapWidthStep) + 1) * s_BitmapWidthStep * s_BytesPerPixel * m_Height * m_Frames;
		uchar* data = new uchar[size];

		in.Read(data, size);
		m_Texture = new gfx::Texture(data, m_Width, m_Height, m_Frames, { .min = GL_NEAREST, .mag = GL_NEAREST, .format = GL_BGR });

		delete[] data;
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
