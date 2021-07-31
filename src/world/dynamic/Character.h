#pragma once
#include "Dynamic.h"
#include "script/Script.h"

namespace engine
{
	class Character : public Dynamic
	{
	public:
		Character(const char* fp, QTNode* const root, DynamicList& list, const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state) :
			Dynamic({ { "move", new Script(fp) } }, root, list, pos, vel, speed, states, state)
		{}
	};
}
