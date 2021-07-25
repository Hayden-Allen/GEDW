#pragma once
#include "pch.h"
#include "world/SpriteGroup.h"
#include "Light.h"
#include "Hitbox.h"

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
			m_QuadTree(other.m_QuadTree),
			m_Hitboxes(std::move(other.m_Hitboxes)),
			m_Pos(other.m_Pos),
			m_Dim(other.m_Dim),
			m_SpriteGroups(std::move(other.m_SpriteGroups)),
			m_Lights(other.m_Lights),
			m_LightCount(other.m_LightCount)
		{
			other.m_QuadTree = nullptr;
			other.m_Lights = nullptr;
		}
		~Chunk()
		{
			delete m_Lights;
		}


		void Draw(Renderer& renderer) const;
		QTNode* const GetQuadTree()
		{
			return m_QuadTree;
		}
		const math::Vec2<float>& GetPos() const
		{
			return m_Pos;
		}
		const math::Vec2<float>& GetDim() const
		{
			return m_Dim;
		}
	private:
		QTNode* m_QuadTree;
		std::vector<Hitbox> m_Hitboxes;
		math::Vec2<float> m_Pos, m_Dim;
		std::vector<SpriteGroup> m_SpriteGroups;
		gfx::UniformBuffer<GL_STATIC_DRAW>* m_Lights;
		uint m_LightCount;
	};
}
