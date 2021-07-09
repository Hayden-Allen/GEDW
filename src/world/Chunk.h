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


		void Merge(ChunkConstructor other, const math::Vec2<float>& offset)
		{
			for (Tile& tile : other.tiles)
				tile.pos += pos + offset;
			tiles.reserve(tiles.size() + other.tiles.size());
			tiles.insert(tiles.end(), other.tiles.begin(), other.tiles.end());
		}
	};

	class Chunk
	{
	public:
		Chunk(const ChunkConstructor& constructor);
		Chunk(const Chunk& other) = delete;
		Chunk(Chunk&& other) noexcept :
			m_Pos(other.m_Pos),
			m_SpriteGroups(std::move(other.m_SpriteGroups))
		{}


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
