#pragma once
#include "pch.h"
#include "Command.h"

namespace engine
{
	class Script;

	class ScriptParser
	{
	public:
		ScriptParser(const char* fp, Script* const script);
		ScriptParser(const ScriptParser& other) = delete;
		ScriptParser(ScriptParser&& other) = delete;


		bool Parse();
	private:
		constexpr static char s_LabelToken = ':', s_CommentToken = '#', s_StringToken = '"', s_RegToken = '$', s_EntryPointToken[] = "main", s_Whitespace[] = "\t ";


		uint m_Line, m_LastStringWrite;
		// label definitions
		std::unordered_map<std::string, uint> m_Labels;
		// label uses
		std::unordered_map<uint, std::string> m_References;
		std::string m_Filepath;
		std::ifstream m_File;
		bool m_Abort;
		Script* m_Script;


		void ParseLine(std::string& line);
		Args Create(const std::string& command, const std::string& arglist);
		ArgType GetArgType(const std::string& arg);
		std::pair<int64_t, bool> ResolveInt(const std::string& arg);
		// print a formatted error with filepath and line number automatically prepended
		template<typename ... Args>
		void Err(uint line, const char* fmt, const Args& ... args)
		{
			char buf[1024];
			sprintf(buf, "[%s:%u] %s\n", m_Filepath.c_str(), line, fmt);
			printf(buf, args...);
			m_Abort = true;
		}
		/**
		 * Various string utilities
		 */
		std::string Trim(const std::string& s)
		{
			const size_t start = s.find_first_not_of(s_Whitespace);
			const size_t end = s.find_last_not_of(s_Whitespace);
			if (start == std::string::npos || end == std::string::npos)
				return "";
			return s.substr(start, end - start + 1);
		}
		std::vector<std::string> Split(const std::string& s, char delim)
		{
			std::vector<std::string> result;
			std::string cur;
			uint last = 0;
			bool string = false;

			for (uint i = 0; i < s.size(); i++)
			{
				if (s[i] == s_StringToken)
					string = !string;
				if (!string && s[i] == delim)
				{
					result.emplace_back(Trim(s.substr(last, i - last)));
					last = i + 1;
				}
			}
			result.emplace_back(Trim(s.substr(last)));

			return result;
		}
		bool NextLine(std::string* const line)
		{
			if (m_File.eof())
				return false;

			getline(m_File, *line);
			return true;
		}
		size_t NextSpace(const std::string& s)
		{
			size_t space = s.find_first_of(s_Whitespace);
			if (space == std::string::npos)
				space = s.size();
			return space;
		}
	};
}
