#include "pch.h"
#include "Chunk.h"
#include "graphics/Renderer.h"

namespace engine
{
	Chunk::Chunk(const ChunkConstructor& constructor) :
		m_Pos(constructor.pos),
		m_Lights(nullptr),
		m_LightCount(0)
	{
		// we need to take the given list of Tiles and separate them out by Sprite
		std::vector<std::unordered_map<Sprite*, std::vector<Tile>>> groups;
		// cache indices into `groups` for fast lookup times
		std::unordered_map<Sprite*, uint> indices;

		// for each given Tile
		for (auto& tile : constructor.tiles)
		{
			// get the index of the group that contains our current Tile's sprite
			Sprite* sprite = tile.sprite;
			uint index = 0;
			auto cached = indices.find(sprite);

			// this is a new Sprite, it isn't part of an existing group yet
			if (cached == indices.end())
			{
				bool found = false;
				uint i;

				// check existing groups to see if they have room for another Sprite
				for (i = 0; i < groups.size(); i++)
				{
					if (groups.at(i).size() < SpriteGroup::GetMaxSpriteCount())
					{
						found = true;
						break;
					}
				}

				// no existing groups have any room, so we need to create a new group
				if (!found)
				{
					std::unordered_map<Sprite*, std::vector<Tile>> temp;
					// insert our current Sprite with an empty vector
					temp.insert({ sprite, std::vector<Tile>() });
					// add new group to our list
					groups.push_back(temp);
					// we added to the end, so the index is size-1
					i = CAST(uint, groups.size() - 1);
				}
				// an existing group does have room, add our current Sprite to that group
				else
					groups.at(i).insert({ sprite, std::vector<Tile>() });

				// cache the index for later lookups
				index = i;
				indices.insert({ sprite, index });
			}
			// we do have an index cached, so just use that
			else
				index = cached->second;

			// add current Tile to the appropriate group
			groups.at(index).at(sprite).push_back(tile);
		}


		// now that our data is split up correctly, we can actually create SpriteGroups from it
		for (auto& group : groups)
			m_SpriteGroups.emplace_back(group);


		auto ptr = constructor.lights.data();
		m_LightCount = math::min(Light::s_MaxLightCount, constructor.lights.size());
		// We're giving our buffer a pointer to our array of lights as a float pointer. This works because our Light struct only contains floats.
		// The number of floats = (number of lights) * (number of bytes per light) / (number of bytes per float)
		m_Lights = new gfx::UniformBuffer<GL_STATIC_DRAW>(m_LightCount * sizeof(Light) / sizeof(float), PUN(float*, ptr), 0);
	}



	void Chunk::Draw(Renderer& renderer) const
	{
		renderer.SetLights(m_LightCount, *m_Lights);
		for (auto& group : m_SpriteGroups)
			group.Draw(renderer);
	}
}
