#pragma once
#include "pch.h"
#include "world/SpriteGroup.h"
#include "Light.h"

namespace engine
{
	class Renderer;

	struct ChunkConstructor
	{
		math::Vec2<float> pos = { 0, 0 };
		std::vector<Tile> tiles;
		std::vector<Light> lights;


		void Merge(ChunkConstructor other, const math::Vec2<float>& offset)
		{
			const math::Vec2<float> off = pos + offset;

			CopyVector(other.tiles, tiles, off);
			CopyVector(other.lights, lights, off);
		}
	private:
		template<typename T>
		void CopyVector(std::vector<T>& from, std::vector<T>& to, const math::Vec2<float>& offset)
		{
			for (T& t : from)
				t.pos += offset;
			to.reserve(to.size() + from.size());
			to.insert(to.end(), from.begin(), from.end());
		}
	};

	class Chunk
	{
	public:
		Chunk(const ChunkConstructor& constructor);
		Chunk(const Chunk& other) = delete;
		Chunk(Chunk&& other) noexcept :
			m_Pos(other.m_Pos),
			m_SpriteGroups(std::move(other.m_SpriteGroups)),
			m_Lights(other.m_Lights),
			m_LightCount(other.m_LightCount)
		{
			other.m_Lights = nullptr;
		}
		~Chunk()
		{
			delete m_Lights;
		}


		void Draw(Renderer& renderer) const;
	private:
		math::Vec2<float> m_Pos;
		std::vector<SpriteGroup> m_SpriteGroups;
		gfx::UniformBuffer* m_Lights;
		uint m_LightCount;
	};
}
