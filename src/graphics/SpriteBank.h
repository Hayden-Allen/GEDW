#pragma once
#include "pch.h"

namespace engine
{
	class Sprite;

	class SpriteBank
	{
	public:
		SpriteBank() {}
		SpriteBank(const SpriteBank& other) = delete;
		SpriteBank(SpriteBank&& other) = delete;
		~SpriteBank();


		Sprite* Put(const std::string& path, uint frames, uint time);
		Sprite* Get(const std::string& path) const;
	private:
		std::unordered_map<std::string, Sprite*> m_Sprites;
	};
}
