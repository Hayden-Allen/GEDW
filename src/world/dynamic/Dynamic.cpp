#include "pch.h"
#include "Dynamic.h"
#include "graphics/Renderer.h"
#include "graphics/Sprite.h"

namespace engine
{
	Dynamic::Dynamic(QTNode* const root, DynamicList& list, const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state) :
		m_Pos(pos),
		m_Vel(vel),
		m_Speed(speed),
		m_Vertices{ 0 },
		m_States(states),
		m_CurrentState(state),
		m_Handle(list.Add(this)),
		m_Hitbox(nullptr)
	{
		SetState(state);
		UpdateVertices();

		const auto& dim = GetCurrentDims();
		m_Hitbox = new Hitbox(m_Pos - dim / 2.f, dim, m_Vel, root);
	}



	void Dynamic::MoveHitbox(QTNode* const root)
	{
		// update hitbox with current values
		const auto& dim = GetCurrentDims();
		m_Hitbox->Move(m_Pos - dim / 2.f, dim, m_Vel, root);
	}
	void Dynamic::ResolveCollisions(float delta)
	{
		m_Hitbox->Update(delta);

		// copy "resolved" values from the hitbox
		SetVel(m_Hitbox->GetVel());
		SetPos(m_Hitbox->GetPos() + m_Hitbox->GetDim() / 2.f);
	}
	void Dynamic::Update(float delta, DynamicList& list)
	{
		m_Pos += m_Vel * delta;
		UpdateVertices();
		list.Update(m_Handle.list);
	}
	void Dynamic::SetState(const std::string& state)
	{
		const auto& it = m_States.find(state);
		if (it != m_States.end())
			m_CurrentState = state;
		else
			printf("Invalid state '%s'\n", state.c_str());
	}
	math::Vec2<float> Dynamic::GetCurrentDims() const
	{
		return GetCurrentSprite()->GetDims();
	}



	void Dynamic::UpdateVertices()
	{
		const Sprite* const sprite = GetCurrentSprite();
		const float sw = .5f * sprite->GetWidth(), sh = .5f * sprite->GetHeight();
		const float dims[] = { -sw, -sh, sw, -sh, sw, sh, -sw, sh };

		for (uint i = 0; i < s_VerticesPerQuad; i++)
		{
			const uint off = i * s_FloatsPerDynamicVertex;
			// x, y
			m_Vertices[off + 0] = m_Pos.x + dims[i * 2 + 0];
			m_Vertices[off + 1] = m_Pos.y + dims[i * 2 + 1];
			// s, t
			m_Vertices[off + 2] = s_CornerPoints[i * 2 + 0];
			m_Vertices[off + 3] = s_CornerPoints[i * 2 + 1];
			// i
			m_Vertices[off + 4] = CAST(float, m_Handle.texture);
			// center y
			m_Vertices[off + 5] = m_Pos.y;
		}
	}
}
