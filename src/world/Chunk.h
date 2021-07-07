#pragma once
#include "pch.h"
#include "world/SpriteGroup.h"

namespace engine
{
	class Renderer;

	struct ChunkConstructor
	{
		math::Vec2<float> pos = { 0.f, 0.f };
		std::vector<Tile> tiles;
	};

	class Chunk
	{
	public:
		Chunk(const ChunkConstructor& constructor);
		Chunk(const Chunk& other) = delete;
		Chunk(Chunk&& other) = delete;


		void Draw(Renderer& renderer) const
		{
			for (auto& group : m_SpriteGroups)
				group.Draw(renderer);
		}
	private:
		math::Vec2<float> m_Pos;
		std::vector<SpriteGroup> m_SpriteGroups;
	};
}
