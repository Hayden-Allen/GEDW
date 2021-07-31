#include "pch.h"
#include "World.h"
#include "graphics/Renderer.h"
#include "world/Camera.h"
#include "world/dynamic/Dynamic.h"
#include "graphics/Sprite.h"
#include "graphics/SpriteBank.h"
#include "world/Map.h"
#include "world/dynamic/DynamicList.h"
#include "script/Script.h"
#include "dynamic/Character.h"
#include "world/dynamic/DynamicBank.h"

namespace engine
{
	World::World(EngineInstance* const instance, const char* fp) :
		m_SpriteBank(new SpriteBank()),
		m_Map(new Map(fp, *m_SpriteBank, *instance)),
		m_DynamicList(new DynamicList()),
		m_Engine(instance),
		m_DynamicBank(new DynamicBank())
	{}
	World::~World()
	{
		delete m_SpriteBank;
		delete m_Map;
		delete m_DynamicList;
		delete m_DynamicBank;
	}



	void World::Draw(Renderer& renderer, Camera& cam, const Dynamic* const player)
	{
		ScriptRuntime rt = { &renderer, this };
		m_DynamicList->Update(m_Map->GetCurrentQuadTree(), rt);

		cam.RunScripts(rt);
		renderer.SetCamera(cam);

		m_Map->Draw(renderer, player);
		m_DynamicList->Draw(renderer);
	}
	Sprite* World::PutSprite(const char* fp, uint frames, uint time)
	{
		return m_SpriteBank->Put(fp, frames, time);
	}
	Dynamic* World::CreateDynamic(const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state)
	{
		return new Dynamic({}, m_Map->GetCurrentQuadTree(), *m_DynamicList, pos, vel, speed, states, state);
	}
	void World::CreateDynamicTemplate(const std::string& name, const std::unordered_map<std::string, Script*>& scripts, const std::unordered_map<std::string, Sprite*>& states, const std::string& state, float speed)
	{
		m_DynamicBank->Put(name, scripts, states, state, speed);
	}
	Dynamic* World::CreateDynamic(const std::string& name, bool add)
	{
		const DynamicTemplate* const temp = m_DynamicBank->Get(name);
		if (!temp)
			return nullptr;
		return new Dynamic(m_Map->GetCurrentQuadTree(), *m_DynamicList, *temp, add);
	}
	Character* const World::CreateCharacter(const char* fp, const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state)
	{
		return new Character(fp, m_Map->GetCurrentQuadTree(), *m_DynamicList, pos, vel, speed, states, state);
	}
}
