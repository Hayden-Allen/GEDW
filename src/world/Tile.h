#pragma once
#include "pch.h"

namespace engine
{
	class Sprite;

	struct Tile
	{
		Sprite* sprite = nullptr;
		math::Vec2<float> pos = { 0.f, 0.f };
		uint layer = 0;
		bool rigid = false;
	};
}
