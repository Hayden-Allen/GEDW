#include "pch.h"
#include "Map.h"
#include "graphics/SpriteBank.h"
#include "graphics/Sprite.h"
#include "Light.h"
#include "dynamic/Dynamic.h"

namespace engine
{
	Map::Map(const char* fp, SpriteBank& bank, EngineInstance& engine) :
		m_Abort(false)
	{
		std::ifstream in(fp, std::ios::binary);
		if (!in.is_open())
		{
			printf("Error opening map file '%s', aborting...\n", fp);
			return;
		}

		TopLevelParams params = { bank, engine };
		std::string word;
		// while there's still stuff to read and we haven't hit an error
		while (in >> word && Execute(s_GenFunctions, word, in, params) && !m_Abort);

		in.close();
	}



	void Map::Draw(Renderer& renderer, const Dynamic* const player)
	{
		const math::Vec2<float>& pos = player->GetPos(), dim = player->GetDims();

		// search for a Chunk that contains the player
		bool found = false;
		for (uint i = 0; i < m_Chunks.size(); i++)
		{
			const Chunk& cur = m_Chunks[i];
			if (math::rectIntersect(cur.GetPos(), cur.GetDim(), pos, dim))
			{
				// multiple Chunks contain the player, which isn't allowed
				if (found)
				{
					printf("Player cannot be within multiple Chunks\n");
					return;
				}
				m_CurrentChunk = i;
				found = true;
			}
		}

		// no Chunks contain the player, which also isn't allowed
		if (!found)
			printf("Player must be in a Chunk\n");

		// only process/draw the Chunk that the player is actually in this frame
		m_Chunks[m_CurrentChunk].Draw(renderer);
	}



	void Map::CreateChunk(std::ifstream& in, TopLevelParams& params)
	{
		ChunkConstructor constructor;
		in >> constructor.pos.x >> constructor.pos.y;

		std::string word;
		// run child functions (TILE, RECT, PATTERN) until we read "END"
		while (in >> word && !IsEnd(word) && Execute(s_ChunkFunctions, word, in, params.bank, constructor));
		if (!CheckEnd(word, "CHUNK"))
			return;

		m_Chunks.emplace_back(constructor);
	}
	void Map::AddTile(std::ifstream& in, SpriteBank& bank, ChunkConstructor& constructor)
	{
		// add a single tile to the given constructor
		Tile tile;

		std::string path;
		uint frames, time;
		in >> path >> frames >> time >> tile.pos.x >> tile.pos.y >> tile.layer >> tile.rigid;
		// create Sprite if necessary
		tile.sprite = bank.Put(path, frames, time);
		// offset position by the constructor's position
		tile.pos += constructor.pos;

		if (!CheckLayer(tile.layer))
			return;

		constructor.tiles.push_back(tile);
	}
	void Map::AddRect(std::ifstream& in, SpriteBank& bank, ChunkConstructor& constructor)
	{
		// add a rectangle of tiles to the given constructor
		std::string path;
		float x, y;
		uint frames, time, w, h, layer;
		bool rigid;
		in >> path >> frames >> time >> x >> y >> w >> h >> layer >> rigid;

		if (!CheckLayer(layer))
			return;

		Sprite* const sprite = bank.Put(path, frames, time);
		float width = 1.f * sprite->GetWidth(), height = 1.f * sprite->GetHeight();

		// loop over whole area defined by (w, h)
		for(uint i = 0; i < h; i++)
			for(uint j = 0; j < w; j++)
				//constructor.tiles.emplace_back(
				//	sprite,
				//	// base coord of the rect + offset for current tile's position + constructor's position
				//	math::Vec2<float>(x + j * width, y + i * height) + constructor.pos,
				//	layer,
				//	rigid
				//);
				constructor.tiles.emplace_back(sprite, math::Vec2<float>(x + j * width, y + i * height) + constructor.pos, layer, rigid);
	}
	void Map::AddPattern(std::ifstream& in, SpriteBank& bank, ChunkConstructor& constructor)
	{
		// add an existing Chunk pattern to the given constructor
		std::string name;
		// offset to apply to everything in the pattern (in addition to the constructor's offset)
		float x, y;
		in >> name >> x >> y;

		auto result = m_Patterns.find(name);
		if (result == m_Patterns.end())
		{
			printf("Unknown pattern '%s', aborting...\n", name.c_str());
			m_Abort = true;
			return;
		}

		// copy all data from the pattern into constructor at given offset
		constructor.Merge(result->second, { x, y });
	}
	void Map::AddLight(std::ifstream& in, SpriteBank& bank, ChunkConstructor& constructor)
	{
		Light light;

		in >> light.pos.x >> light.pos.y >> light.intensity >> light.color[0] >> light.color[1] >> light.color[2];
		light.pos += constructor.pos;

		constructor.lights.push_back(light);
	}
	void Map::UpdateSettings(std::ifstream& in, TopLevelParams& params)
	{
		std::string word;
		while (in >> word && !IsEnd(word) && Execute(s_SettingFunctions, word, in, params.engine));
		CheckEnd(word, "SETTING");
	}
	void Map::SetClearColor(std::ifstream& in, EngineInstance& engine)
	{
		float r, g, b;
		in >> r >> g >> b;
		engine.SetClearColor(r, g, b);
	}
	void Map::SetPixelSize(std::ifstream& in, EngineInstance& engine)
	{
		float size;
		in >> size;
		engine.SetPixelSize(size);
	}
	void Map::CreatePattern(std::ifstream& in, TopLevelParams& params)
	{
		// Notice that we don't take an (x, y) pair for this block, even though it's technically a chunk. All positions within a pattern should be relative to the position at which that pattern is copied to (see AddPattern)
		ChunkConstructor pattern;
		std::string word, name;
		in >> name;

		while (in >> word && !IsEnd(word) && Execute(s_ChunkFunctions, word, in, params.bank, pattern));
		if (!CheckEnd(word, "PATTERN"))
			return;

		// I'm allowing redefinition of existing pattern names here. You can add a check to prevent this if you want
		m_Patterns.insert({ name, pattern });
	}
}
