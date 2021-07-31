#pragma once
#include "pch.h"

namespace engine
{
	struct EngineInstance;
	class Renderer;
	class Camera;
	class Dynamic;
	class Sprite;
	class SpriteBank;
	class Map;
	class DynamicList;
	class Character;
	class Script;
	class DynamicBank;

	class World
	{
	public:
		friend class Script;


		World(EngineInstance* const instance, const char* fp);
		World(const World& other) = delete;
		World(World&& other) = delete;
		~World();


		void Draw(Renderer& renderer, Camera& cam, const Dynamic* const player);
		Sprite* PutSprite(const char* fp, uint frames, uint time);
		Dynamic* CreateDynamic(const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state);
		void CreateDynamicTemplate(const std::string& name, const std::unordered_map<std::string, Script*>& scripts, const std::unordered_map<std::string, Sprite*>& states, const std::string& state, float speed);
		Dynamic* CreateDynamic(const std::string& name, bool add);
		Character* const CreateCharacter(const char* fp, const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state);
	private:
		SpriteBank* m_SpriteBank;
		Map* m_Map;
		DynamicList* m_DynamicList;
		EngineInstance* m_Engine;
		DynamicBank* m_DynamicBank;
	};
}
