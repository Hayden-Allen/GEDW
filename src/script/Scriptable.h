#pragma once
#include "pch.h"

namespace engine
{
	class Script;
	class World;
	struct ScriptRuntime;

	class Scriptable
	{
	public:
		template<typename T = void>
		Scriptable(const math::Vec2<float>& pos, const math::Vec2<float>& vel, const math::Vec2<float>& dim, float speed, const std::unordered_map<std::string, Script*>& scripts, const std::unordered_map<std::string, T*>& states, const std::string& state) :
			m_Pos(pos),
			m_Vel(vel),
			m_Dim(dim),
			m_Speed(speed),
			m_Scripts(scripts),
			m_States(math::castMap<std::string, void*>(states)),
			m_CurrentState(state)
		{}
		Scriptable(const Scriptable& other) = delete;
		Scriptable(Scriptable&& other) = delete;
		~Scriptable();


		virtual const std::unordered_map<std::string, int64_t>& RunScripts(ScriptRuntime& rt) = 0;
		bool Has(const std::string& name) const
		{
			const auto& it = m_Scripts.find(name);
			return it != m_Scripts.end();
		}
		const math::Vec2<float>& GetPos() const
		{
			return m_Pos;
		}
		const math::Vec2<float>& GetVel() const
		{
			return m_Vel;
		}
		const math::Vec2<float>& GetDims() const
		{
			return m_Dim;
		}
		float GetSpeed() const
		{
			return m_Speed;
		}
		void SetPos(const math::Vec2<float>& pos)
		{
			m_Pos = pos;
		}
		void SetVel(const math::Vec2<float>& vel)
		{
			m_Vel = vel;
			m_Vel.Clamp(0, m_Speed);
			if (m_Vel.IsZero())
				m_Vel = { 0.f, 0.f };
		}
		void SetState(const std::string& state)
		{
			const auto& it = m_States.find(state);
			if (it != m_States.end())
				m_CurrentState = state;
			else
				printf("Invalid state '%s'\n", state.c_str());
		}
	protected:
		math::Vec2<float> m_Pos, m_Dim, m_Vel;
		float m_Speed;
		std::unordered_map<std::string, Script*> m_Scripts;
		std::unordered_map<std::string, int64_t> m_Flags;
		std::unordered_map<std::string, void*> m_States;
		std::string m_CurrentState;


		const std::unordered_map<std::string, int64_t>& Run(ScriptRuntime& rt, std::vector<Scriptable*>& env);
		template<typename T>
		T* const GetCurrentState() const
		{
			return CAST(T* const, m_States.at(m_CurrentState));
		}
	};
}
