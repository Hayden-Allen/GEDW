#include "pch.h"
#include "SpriteGroup.h"
#include "graphics/Renderer.h"
#include "graphics/Sprite.h"

namespace engine
{
	SpriteGroup::SpriteGroup(const std::unordered_map<Sprite*, std::vector<Tile>>& data) :
		m_RenderObject(nullptr)
	{
		const uint max = GetMaxSpriteCount();
		// given data has more unique Sprites than current system can support
		if (data.size() > max)
		{
			printf("Cannot create a SpriteGroup with %llu sprites (must be at most %u)\n", data.size(), max);
			return;
		}

		// total number of tiles we need to generate vertex data for
		uint tileCount = 0;
		for (auto& entry : data)
			tileCount += CAST(uint, entry.second.size());

		// total number of floats we need
		const uint floatCount = s_FloatsPerVertex * s_VerticesPerQuad * tileCount;
		float* vertices = new float[floatCount];
		// index at which to start writing vertices
		uint offset = 0;
		// current sprite index (maps to i_TexIndex in our shader)
		float index = 0.f;

		for (auto& entry : data)
		{
			// for each Sprite, add it to our list
			Sprite* s = entry.first;
			m_Sprites.emplace_back(s);
			// store dimensions for convenience
			const float sw = 1.f * s->GetWidth(), sh = 1.f * s->GetHeight();

			// for each Tile that uses the current Sprite
			for (auto& tile : entry.second)
			{
				// generate all vertices for current Tile
				for (uint i = 0; i < s_VerticesPerQuad; i++)
				{
					// offset for current tile + offset for current vertex
					const uint vertexOffset = offset + i * s_FloatsPerVertex;
					// x, y => bottom left corner + current corner offset * sprite dimensions
					vertices[vertexOffset + 0] = tile.pos.x + s_CornerPoints[i * 2 + 0] * sw;
					vertices[vertexOffset + 1] = tile.pos.y + s_CornerPoints[i * 2 + 1] * sh;
					// s, t => current corner offset (each Sprite takes up a whole texture)
					vertices[vertexOffset + 2] = s_CornerPoints[i * 2 + 0];
					vertices[vertexOffset + 3] = s_CornerPoints[i * 2 + 1];
					// i
					vertices[vertexOffset + 4] = index;
					// depth
					vertices[vertexOffset + 5] = 1.f - 1.f * tile.layer / (s_LayerCount - 1);
				}
				// increment offset by the number of floats we just wrote
				offset += s_FloatsPerVertex * s_VerticesPerQuad;
			}
			// enumerate unordered Sprites by incrementing index after all Tiles for each Sprite are processed
			index++;
		}


		// upload our data to the GPU then delete the local copy
		m_RenderObject = new gfx::RenderObject<GL_STATIC_DRAW, GL_STATIC_DRAW>(floatCount, vertices, { 2, 2, 1, 1 }, s_IndicesPerQuad, s_VerticesPerQuad, s_IndexOffsets);
		delete[] vertices;
	}



	void SpriteGroup::Draw(Renderer& renderer) const
	{
		renderer.Draw(*m_RenderObject, m_Sprites);
	}
}
