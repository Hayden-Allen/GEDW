#include "pch.h"
#include "Script.h"
#include "ScriptParser.h"
#include "world/Map.h"
#include "graphics/Renderer.h"

namespace engine
{
	Script::Script(const char* fp) :
		m_Compiled(false),
		m_Abort(false),
		m_Sleeping(false),
		m_ProgramCounter(0),
		m_EntryPoint(0),
		m_StackPointer(0),
		m_Stack{ 0 },
		m_Memory{ 0 },
		m_SleepEnd(0.f),
		m_Filepath(fp)
	{
		if (s_CommandNames.empty())
		{
			for (uint i = 0; i < s_Instructions.size(); i++)
			{
				const auto& cur = s_Instructions[i];
				s_CommandNames.emplace(i, cur.name);
				s_CommandDescriptions.emplace(cur.name, CommandDescription(i, cur.desc));
				s_Operations[i] = cur.op;
			}
		}

		m_Compiled = ScriptParser(fp, this).Parse();
	}



	Script::integer Script::Run(const ScriptRuntime& rt, Scriptable* const host, std::vector<Scriptable*>& env)
	{
		if (!m_Compiled)
		{
			printf("Cannot run a Script that failed to compile\n");
			return 0;
		}

		// this script can't be run yet
		if (rt.renderer->GetTime() < m_SleepEnd)
			return 0;

		// if this script was sleeping, have it resume from where it left off, otherwise start over
		if (!m_Sleeping)
			m_ProgramCounter = m_EntryPoint;

		// reset default values
		m_Sleeping = false;
		m_Abort = false;
		m_Registers.i[Registers::s_RegHost] = s_HostIndex;
		m_Registers.i[Registers::s_RegObjCount] = env.size();
		m_Registers.i[Registers::s_RegFlags] = 0;
		m_SpawnQueue.clear();

		while (!m_Abort && m_ProgramCounter < m_Instructions.size())
		{
			const auto& cur = m_Instructions[m_ProgramCounter];
			(this->*(s_Operations[cur.opcode]))(cur, rt.renderer->GetTime(), rt.renderer->GetFrameDelta(), rt.world, host, env);
			m_ProgramCounter++;
		}

		for (Dynamic* spawned : m_SpawnQueue)
			spawned->AddTo(rt.world->m_Map->GetCurrentQuadTree(), *rt.world->m_DynamicList);

		return m_Registers.i[Registers::s_RegFlags];
	}



	Script::integer* const Script::GetIntRegister(uint index)
	{
		if (index <= 18)
			return &m_Registers.i[index];
		if (index >= 24 && index <= 26)
			return &m_Registers.i[index - 5];
		if (index >= 49)
			return &m_Registers.i[index - 27];

		return nullptr;
	}
	Script::fp* const Script::GetFloatRegister(uint index)
	{
		if (index >= 19 && index <= 21)
			return &m_Registers.f[index - 19];
		if (index >= 27 && index <= 29)
			return &m_Registers.f[index - 24];
		if (index >= 32 && index <= 39)
			return &m_Registers.f[index - 26];

		return nullptr;
	}
	Script::vec* const Script::GetVecRegister(uint index)
	{
		if (index >= 22 && index <= 23)
			return &m_Registers.v[index - 22];
		if (index >= 30 && index <= 31)
			return &m_Registers.v[index - 28];
		if (index >= 40 && index <= 48)
			return &m_Registers.v[index - 36];

		return nullptr;
	}
	bool Script::RangeCheck(integer i, integer min, integer max)
	{
		if (i < min || i >= max)
		{
			m_Abort = true;
			printf("Invalid index %lld (must be in [%lld, %lld))\n", i, min, max);
		}
		return !m_Abort;
	}
}
