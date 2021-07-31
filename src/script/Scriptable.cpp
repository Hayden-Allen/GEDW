#include "pch.h"
#include "Scriptable.h"
#include "Script.h"

namespace engine
{
	Scriptable::~Scriptable()
	{
		for (auto& script : m_Scripts)
			delete script.second;
	}



	const std::unordered_map<std::string, int64_t>& Scriptable::Run(ScriptRuntime& rt, std::vector<Scriptable*>& env)
	{
		m_Flags.clear();
		for (auto& script : m_Scripts)
			m_Flags[script.first] = script.second->Run(rt, this, env);
		return m_Flags;
	}
}
