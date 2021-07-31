#include "pch.h"
#include "DynamicBank.h"
#include "Dynamic.h"

namespace engine
{
	const DynamicTemplate* const DynamicBank::Put(const std::string& name, const std::unordered_map<std::string, Script*>& scripts, const std::unordered_map<std::string, Sprite*>& states, const std::string& state, float speed)
	{
		const auto& it = m_Templates.find(name);
		if (it != m_Templates.end())
			printf("Overriding DynamicTemplate '%s'\n", name.c_str());
		m_Templates[name] = { scripts, states, state, speed };
		return &m_Templates[name];
	}
	const DynamicTemplate* const DynamicBank::Get(const std::string& name) const
	{
		const auto& it = m_Templates.find(name);
		if (it != m_Templates.end())
		{
			printf("DynamicTemplate '%s' doesn't exist\n", name.c_str());
			return nullptr;
		}
		return &it->second;
	}
}
