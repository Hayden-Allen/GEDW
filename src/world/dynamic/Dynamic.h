#pragma once
#include "pch.h"

namespace engine
{
	class Sprite;
	class Renderer;

	class Dynamic
	{
	public:
		Dynamic(const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state);
		Dynamic(const Dynamic& other) = delete;
		Dynamic(Dynamic&& other) = delete;
		~Dynamic()
		{
			delete m_RenderObject;
		}


		void Update(float delta);
		void Draw(Renderer& renderer);
		void SetState(const std::string& state);
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
		Sprite* const GetCurrentSprite()
		{
			return m_States[m_CurrentState];
		}
	private:
		math::Vec2<float> m_Pos, m_Vel;
		float m_Speed, m_Vertices[s_FloatsPerQuad];
		std::unordered_map<std::string, Sprite*> m_States;
		std::string m_CurrentState;
		gfx::RenderObject<GL_DYNAMIC_DRAW, GL_STATIC_DRAW>* m_RenderObject;


		void UpdateVertices();
	};
}
