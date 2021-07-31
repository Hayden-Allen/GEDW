#include "pch.h"
#include "ScriptParser.h"
#include "Script.h"

namespace engine
{
	ScriptParser::ScriptParser(const char* fp, Script* const script) :
		m_LastStringWrite(Script::s_MemCount),
		m_Line(0),
		m_Filepath(fp),
		m_File(fp),
		m_Abort(false),
		m_Script(script)
	{
		if (!m_File.is_open())
		{
			printf("Error opening script file '%s'\n", fp);
			m_Abort = true;
		}
	}



	bool ScriptParser::Parse()
	{
		std::string line;
		// read each line of the input file
		while (!m_Abort && NextLine(&line))
		{
			// increment line count
			m_Line++;

			// empty line, ignore it
			if (Trim(line).empty())
				continue;

			ParseLine(line);
		}

		// resolve label references
		if (!m_Abort)
		{
			for (const auto& ref : m_References)
			{
				const auto& it = m_Labels.find(ref.second);
				// a non-existent label was referenced
				if (it == m_Labels.end())
				{
					Err(ref.first, "Unresolved reference '%s'", ref.second.c_str());
					m_Abort = true;
					break;
				}
				m_Script->m_Instructions[ref.first].imm1i = it->second;
			}
		}

		// if a "main" label is provided, use it as the entry point
		const auto& it = m_Labels.find(s_EntryPointToken);
		if (it != m_Labels.end())
			m_Script->m_EntryPoint = it->second;

		return !m_Abort;
	}



	void ScriptParser::ParseLine(std::string& line)
	{
		// remove any whitespace from the front of this line then get the first whitespace index
		size_t space = NextSpace(line), letter = line.find_first_not_of(s_Whitespace);
		if (space < letter)
		{
			line = line.substr(letter);
			space = NextSpace(line);
		}
		// separate line into the instruction and the arguments
		std::string command = line.substr(0, space), args = line.substr(space);
		command = Trim(command);
		args = Trim(args);

		// this line is commented out, ignore it
		if (command[0] == s_CommentToken)
			return;

		// this line is a label definition
		if (command.back() == s_LabelToken)
		{
			// labels must be on their own line
			if (!args.empty())
			{
				Err(m_Line, "Labels must be on their own line");
				return;
			}
			// get the label name
			const std::string label = command.substr(0, command.size() - 1);
			// labels must be unique
			const auto& it = m_Labels.find(label);
			if (it != m_Labels.end())
			{
				Err(m_Line, "Label '%s' is defined twice", label.c_str());
				return;
			}
			
			m_Labels.emplace(label, CAST(uint, m_Script->m_Instructions.size()));
			return;
		}

		// otherwise, this line is an instruction
		m_Script->m_Instructions.emplace_back(Create(command, args));
	}
	Args ScriptParser::Create(const std::string& command, const std::string& arglist)
	{
		Args args;

		// get info about the given instruction
		const auto& description = Script::s_CommandDescriptions.find(command);
		if (description == Script::s_CommandDescriptions.end())
		{
			Err(m_Line, "Invalid command '%'s", command.c_str());
			return args;
		}
		args.opcode = description->second.opcode;

		// make sure given arguments are valid
		const auto& expected = description->second.args;
		std::vector<std::string> list = Split(arglist, ',');
		list.erase(std::remove_if(list.begin(), list.end(), [this](const std::string& s) { return Trim(s).size() == 0; }), list.end());
		if (list.size() != expected.size())
		{
			Err(m_Line, "Instruction '%s' expects %zu arguments but %zu were given", command.c_str(), expected.size(), list.size());
			return args;
		}
		for (uint i = 0; i < list.size(); i++)
		{
			const ArgType cur = GetArgType(list[i]);
			if (!(expected[i] & cur))
			{
				Err(m_Line, "Invalid argument %u for instruction '%s'", i, command.c_str());
				return args;
			}
		}

		// parse each argument in the list
		uint micount = 0;
		for (uint i = 0; i < list.size(); i++)
		{
			ArgType cur = GetArgType(list[i]);
			// this argument is a label reference, it must be resolved at the end of parsing
			if (cur == ArgType::L)
				continue;

			// convert this argument into an int
			auto result = ResolveInt(list[i]);

			// this argument is an immediate value
			if (IsImmediate(cur))
			{
				if (!result.second)
				{
					(micount == 0 ? args.imm1i : args.imm2i) = result.first;
					micount++;
				}
				// result.second being set indicates that the returned "integer" is actually a float
				else
					args.imm1f = PUN(Script::fp, result.first);
			}
			// this argument is a register
			else
			{
				// get a register pointer from the returned register index
				if (cur == ArgType::I)
					args.i[i] = m_Script->GetIntRegister(CAST(uint, result.first));
				else if (cur == ArgType::F)
					args.f[i] = m_Script->GetFloatRegister(CAST(uint, result.first));
				else
					args.v[i] = m_Script->GetVecRegister(CAST(uint, result.first));
			}
		}

		return args;
	}
	ArgType ScriptParser::GetArgType(const std::string& arg)
	{
		// string literal
		if (arg[0] == s_StringToken)
			return ArgType::MS;

		// label reference
		if (std::isalpha(arg[0]))
		{
			m_References.emplace(CAST(uint, m_Script->m_Instructions.size()), arg);
			return ArgType::L;
		}

		// special register (they all happen to be int registers)
		const auto& it = Script::s_SpecialRegisters.find(arg);
		if (it != Script::s_SpecialRegisters.end())
			return ArgType::I;

		// regular register
		if (arg[0] == s_RegToken)
		{
			// all regular register names are of the form "$<c><n>"
			if (arg.size() != 3)
				goto GetArgType_end;

			const char c = arg[1], n = arg[2];
			if ((c == 't' || c == 's') && (n >= '0' && n <= '7'))
				return ArgType::I;
			if ((c == 'a' || c == 'r') && (n >= '0' && n <= '2'))
				return ArgType::I;
			if (c == 'f' && (n >= '0' && n <= '7'))
				return ArgType::F;
			if ((c == 'a' || c == 'r') && (n >= '3' && n <= '5'))
				return ArgType::F;
			if (c == 'v' && (n >= '0' && n <= '7'))
				return ArgType::V;
			if ((c == 'a' || c == 'r') && (n >= '6' && n <= '7'))
				return ArgType::V;
		}
		// float immediate (must contain a '.' to denote this)
		if (arg.find('.') != std::string::npos)
			return ArgType::MF;
		// int immediate
		if (isdigit(arg[0]) || arg[0] == '-')
			return ArgType::MI;

	GetArgType_end:
		Err(m_Line, "Invalid argument '%s'", arg.c_str());
		return ArgType::NONE;
	}
	std::pair<int64_t, bool> ScriptParser::ResolveInt(const std::string& arg)
	{
		// string literal
		if (arg[0] == s_StringToken)
		{
			// make sure the last character is also a "
			if (arg.back() != s_StringToken)
			{
				Err(m_Line, "String literals must be enclosed in '\"'");
				return { 0, false };
			}
			// write string into RAM
			const uint length = CAST(uint, arg.size() - 1);
			m_LastStringWrite = m_LastStringWrite - length;
			for (uint i = 0; i < length; i++)
				m_Script->m_Memory[m_LastStringWrite + i] = (i == length - 1 ? 0 : arg[i + 1]);
			// return pointer to the string
			return { CAST(int64_t, m_LastStringWrite), false };
		}

		// get special register index from map
		const auto& it = Script::s_SpecialRegisters.find(arg);
		if (it != Script::s_SpecialRegisters.end())
			return { it->second, false };

		// $<c><n>
		if (arg[0] == s_RegToken)
		{
			// group base index + number
			char group = arg[1], n = arg[2] - '0';
			return { Script::s_RegisterOffsets.at(group) + n, false };
		}

		// floats are still returned as "ints" but with the flag set
		if (arg.find('.') != std::string::npos)
		{
			Script::fp d = std::stod(arg);
			return { PUN(int64_t, d), true };
		}

		// support bin, dec, and hex
		int base = 10;
		if (arg.find("0x") == 0)
			base = 16;
		if (arg.find("0b") == 0)
			base = 2;
		return { std::stoll((base != 10 ? arg.substr(2) : arg), nullptr, base), false };
	}
}
