#pragma once
#include "pch.h"
#include "dynamic/Dynamic.h"
#include "script/Scriptable.h"
#include "script/Script.h"

namespace engine
{
	class Camera : public Scriptable
	{
	public:
		Camera(const char* fp, const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, Dynamic* const target) :
			Scriptable(pos, vel, { 0.f, 0.f }, speed, { { "run", new Script(fp) } }, {}, ""),
			m_Target(target)
		{
			m_Environment.push_back(m_Target);
		}
		Camera(const Camera& other) = delete;
		Camera(Camera&& other) = delete;


		const std::unordered_map<std::string, int64_t>& RunScripts(ScriptRuntime& rt) override
		{
			return Run(rt, m_Environment);
		}
	private:
		Dynamic* const m_Target;
		std::vector<Scriptable*> m_Environment;
	};
}
