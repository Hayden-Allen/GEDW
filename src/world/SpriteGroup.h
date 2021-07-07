#pragma once
#include "pch.h"
#include "Tile.h"

namespace engine
{
	class Renderer;

	class SpriteGroup
	{
	public:
		SpriteGroup(const std::unordered_map<Sprite*, std::vector<Tile>>& data);
		SpriteGroup(const SpriteGroup& group) = delete;
		SpriteGroup(SpriteGroup&& group) = delete;
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
		gfx::RenderObject* m_RenderObject;
		// list of Sprites to send to our Renderer
		std::vector<Sprite*> m_Sprites;
	};
}
