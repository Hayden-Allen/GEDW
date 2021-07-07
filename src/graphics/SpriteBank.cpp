#include "pch.h"
#include "SpriteBank.h"
#include "Sprite.h"

namespace engine
{
	SpriteBank::~SpriteBank()
	{
		for (auto& pair : m_Sprites)
			delete pair.second;
	}



	Sprite* SpriteBank::Put(const std::string& path, uint frames, uint time)
	{
		Sprite* existing = Get(path);
		if (existing)
			return existing;

		Sprite* temp = new Sprite(path, frames, time);
		m_Sprites.emplace(path, temp);
		return temp;
	}
	Sprite* SpriteBank::Get(const std::string& path) const
	{
		auto result = m_Sprites.find(path);
		if (result == m_Sprites.end())
			return nullptr;
		return result->second;
	}
}
