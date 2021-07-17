#pragma once
#include "pch.h"
#include "Tile.h"

namespace engine
{
	class Renderer;

	class SpriteGroup
	{
	public:
		constexpr static uint s_LayerCount = 2;


		SpriteGroup(const std::unordered_map<Sprite*, std::vector<Tile>>& data);
		SpriteGroup(const SpriteGroup& group) = delete;
		SpriteGroup(SpriteGroup&& group) noexcept :
			m_RenderObject(group.m_RenderObject),
			m_Sprites(std::move(group.m_Sprites))
		{
			group.m_RenderObject = nullptr;
		}
		~SpriteGroup()
		{
			delete m_RenderObject;
		}


		void Draw(Renderer& renderer) const;
		static uint GetMaxSpriteCount()
		{
			return gfx::getMaxTextureUnits();
		}
	private:
		// everything in this group will use a single VAO and IBO
		gfx::RenderObject<GL_STATIC_DRAW, GL_STATIC_DRAW>* m_RenderObject;
		// list of Sprites to send to our Renderer
		std::vector<Sprite*> m_Sprites;
	};
}
