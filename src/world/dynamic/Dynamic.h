#pragma once
#include "pch.h"
#include "DynamicList.h"

namespace engine
{
	class Sprite;
	class Renderer;

	class Dynamic
	{
	public:
		friend class DynamicList;


		Dynamic(DynamicList& list, const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state);
		Dynamic(const Dynamic& other) = delete;
		Dynamic(Dynamic&& other) = delete;


		void Update(float delta, DynamicList& list);
		void SetState(const std::string& state);
		math::Vec2<float> GetCurrentDims() const;
		const math::Vec2<float>& GetPos() const
		{
			return m_Pos;
		}
		const math::Vec2<float>& GetVel() const
		{
			return m_Vel;
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
		}
		Sprite* const GetCurrentSprite() const
		{
			return m_States.at(m_CurrentState);
		}
		const float* const GetVertices() const
		{
			return m_Vertices;
		}
	private:
		math::Vec2<float> m_Pos, m_Vel;
		float m_Speed, m_Vertices[s_FloatsPerDynamic];
		std::unordered_map<std::string, Sprite*> m_States;
		std::string m_CurrentState;
		DynamicList::Handle m_Handle;


		void UpdateVertices();
	};
}
