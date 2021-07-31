#pragma once
#include "Dynamic.h"

namespace engine
{
	struct DynamicTemplate;

	class DynamicBank
	{
	public:
		DynamicBank() {}
		DynamicBank(const DynamicBank& other) = delete;
		DynamicBank(DynamicBank&& other) = delete;


		const DynamicTemplate* const Put(const std::string& name, const std::unordered_map<std::string, Script*>& scripts, const std::unordered_map<std::string, Sprite*>& states, const std::string& state, float speed);
		const DynamicTemplate* const Get(const std::string& name) const;
	private:
		std::unordered_map<std::string, DynamicTemplate> m_Templates;
	};
}
