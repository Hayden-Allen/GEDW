#include "pch.h"
#include "Dynamic.h"
#include "graphics/Renderer.h"
#include "graphics/Sprite.h"

namespace engine
{
	Dynamic::Dynamic(const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state) :
		m_Pos(pos),
		m_Vel(vel),
		m_Speed(speed),
		m_Vertices{ 0 },
		m_States(states),
		m_CurrentState(state),
		m_RenderObject(nullptr)
	{
		SetState(state);
		m_RenderObject = new gfx::RenderObject<GL_DYNAMIC_DRAW, GL_STATIC_DRAW>(s_FloatsPerDynamic, m_Vertices, { 2, 2, 1 }, s_IndicesPerQuad, s_VerticesPerQuad, s_IndexOffsets);
	}



	void Dynamic::Update(float delta)
	{
		m_Pos += m_Vel * delta;
		UpdateVertices();
	}
	void Dynamic::Draw(Renderer& renderer)
	{
		renderer.Draw(*m_RenderObject, GetCurrentSprite());
	}
	void Dynamic::SetState(const std::string& state)
	{
		const auto& it = m_States.find(state);
		if (it != m_States.end())
			m_CurrentState = state;
		else
			printf("Invalid state '%s'\n", state.c_str());
	}



	void Dynamic::UpdateVertices()
	{
		const Sprite* const sprite = GetCurrentSprite();
		const float sw = .5 * sprite->GetWidth(), sh = .5f * sprite->GetHeight();
		const float dims[] = { -sw, -sh, sw, -sh, sw, sh, -sw, sh };

		for (uint i = 0; i < s_VerticesPerQuad; i++)
		{
			const uint off = i * s_FloatsPerVertex;
			// x, y
			m_Vertices[off + 0] = m_Pos.x + dims[i * 2 + 0];
			m_Vertices[off + 1] = m_Pos.y + dims[i * 2 + 1];
			// s, t
			m_Vertices[off + 2] = s_CornerPoints[i * 2 + 0];
			m_Vertices[off + 3] = s_CornerPoints[i * 2 + 1];
			// i
			m_Vertices[off + 4] = 0.f;
		}

		m_RenderObject->GetVertexArray().GetBuffer().Update(math::arrlen(m_Vertices), m_Vertices, 0);
	}
}
