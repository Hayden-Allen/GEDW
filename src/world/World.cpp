#include "pch.h"
#include "World.h"
#include "graphics/Renderer.h"
#include "world/Camera.h"
#include "world/dynamic/Dynamic.h"
#include "graphics/Sprite.h"
#include "graphics/SpriteBank.h"
#include "world/Map.h"
#include "world/dynamic/DynamicList.h"

namespace engine
{
	World::World(EngineInstance& instance, const char* fp) :
		m_SpriteBank(new SpriteBank()),
		m_Map(new Map(fp, *m_SpriteBank, instance)),
		m_DynamicList(new DynamicList())
	{}
	World::~World()
	{
		delete m_SpriteBank;
		delete m_Map;
		delete m_DynamicList;
	}



	void World::Draw(Renderer& renderer, Camera& cam, const Dynamic* const player)
	{
		m_DynamicList->Update(m_Map->GetCurrentQuadTree(), renderer.GetFrameDelta());

		cam.Update();
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
		return new Dynamic(m_Map->GetCurrentQuadTree(), *m_DynamicList, pos, vel, speed, states, state);
	}
}
