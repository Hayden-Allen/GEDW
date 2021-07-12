#pragma once
#include "pch.h"
#include "Chunk.h"

namespace engine
{
	class SpriteBank;

	class Map
	{
	public:
		Map(const char* fp, SpriteBank& bank, EngineInstance& engine);
		Map(const Map& other) = delete;
		Map(Map&& other) = delete;


		void Draw(Renderer& renderer) const
		{
			for (auto& chunk : m_Chunks)
				chunk.Draw(renderer);
		}
	private:
		struct TopLevelParams
		{
			SpriteBank& bank;
			EngineInstance& engine;
		};


		constexpr static char s_EndToken[] = "end";
		std::unordered_map<std::string, ChunkConstructor> m_Patterns;
		std::vector<Chunk> m_Chunks;
		bool m_Abort;


		// CHUNK
		void CreateChunk(std::ifstream& in, TopLevelParams& params);
		void AddTile(std::ifstream& in, SpriteBank& bank, ChunkConstructor& constructor);
		void AddRect(std::ifstream& in, SpriteBank& bank, ChunkConstructor& constructor);
		void AddPattern(std::ifstream& in, SpriteBank& bank, ChunkConstructor& constructor);
		void AddLight(std::ifstream& in, SpriteBank& bank, ChunkConstructor& constructor);
		// SETTING
		void UpdateSettings(std::ifstream& in, TopLevelParams& params);
		void SetClearColor(std::ifstream& in, EngineInstance& engine);
		void SetPixelSize(std::ifstream& in, EngineInstance& engine);
		// PATTERN
		void CreatePattern(std::ifstream& in, TopLevelParams& params);


		template<typename T, typename ... Args>
		bool Execute(const std::unordered_map<std::string, T>& map, std::string& word, std::ifstream& in, Args& ... args)
		{
			math::toLower(word);
			auto result = map.find(word);
			// function pointer with given name in given map doesn't exist
			if (result == map.end())
			{
				printf("Invalid keyword '%s', aborting...\n", word.c_str());
				m_Abort = true;
				return false;
			}
			// execute function mapped to by `word` with given `args`
			(this->*(result->second))(in, args...);
			return true;
		}
		bool CheckEnd(const std::string& word, const std::string& section)
		{
			if (!math::equalsIgnoreCase(word, s_EndToken))
			{
				printf("%s section must terminated by '%s', aborting...\n", section.c_str(), s_EndToken);
				m_Abort = true;
			}
			return !m_Abort;
		}
		bool IsEnd(const std::string& word)
		{
			return math::equalsIgnoreCase(word, s_EndToken);
		}


		// top level blocks
		typedef void(Map::* GenFunc)(std::ifstream&, TopLevelParams&);
		inline const static std::unordered_map<std::string, GenFunc> s_GenFunctions =
		{
			{ "chunk", &Map::CreateChunk },
			{ "setting", &Map::UpdateSettings },
			{ "pattern", &Map::CreatePattern }
		};
		// stuff that can go in CHUNK and PATTERN blocks
		typedef void(Map::* ChunkFunc)(std::ifstream&, SpriteBank&, ChunkConstructor&);
		inline const static std::unordered_map<std::string, ChunkFunc> s_ChunkFunctions =
		{
			{ "tile", &Map::AddTile },
			{ "rect", &Map::AddRect },
			{ "pattern", &Map::AddPattern },
			{ "light", &Map::AddLight }
		};
		// stuff that can go in a SETTING block
		typedef void(Map::* SettingFunc)(std::ifstream&, EngineInstance&);
		inline const static std::unordered_map<std::string, SettingFunc> s_SettingFunctions =
		{
			{ "clear", &Map::SetClearColor },
			{ "size", &Map::SetPixelSize }
		};
	};
}
