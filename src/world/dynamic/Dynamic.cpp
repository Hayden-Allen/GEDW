#include "pch.h"
#include "Dynamic.h"
#include "graphics/Renderer.h"
#include "graphics/Sprite.h"

namespace engine
{
	Dynamic::Dynamic(QTNode* const root, DynamicList& dl, const DynamicTemplate& temp, bool add) :
		Scriptable({ 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f }, temp.speed, temp.scripts, temp.states, temp.state),
		m_Vertices{ 0.f },
		m_Handle((add ? dl.Add(this) : DynamicList::Handle(0, 0, 0))),
		m_Hitbox(nullptr),
		m_Added(add)
	{
		Init(root);
	}
	Dynamic::Dynamic(const std::unordered_map<std::string, Script*>& scripts, QTNode* const root, DynamicList& list, const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state) :
		Scriptable(pos, vel, { 0.f, 0.f }, speed, scripts, states, state),
		m_Vertices{ 0.f },
		m_Handle(list.Add(this)),
		m_Hitbox(nullptr),
		m_Added(true)
	{
		Init(root);
	}



	const std::unordered_map<std::string, int64_t>& Dynamic::RunScripts(ScriptRuntime& rt)
	{
		std::vector<Scriptable*> env;
		const auto& ret = Run(rt, env);
		m_Dim = GetCurrentSprite()->GetDims();
		return ret;
	}
	void Dynamic::MoveHitbox(QTNode* const root)
	{
		// update hitbox with current values
		m_Hitbox->Move(m_Pos - m_Dim / 2.f, m_Dim, m_Vel, root);
	}
	void Dynamic::ResolveCollisions(float delta, DynamicList& list)
	{
		m_Hitbox->Update(delta);

		// copy "resolved" values from the hitbox
		SetVel(m_Hitbox->GetVel());
		SetPos(m_Hitbox->GetPos() + m_Hitbox->GetDim() / 2.f);
		UpdateVertices();
		list.Update(m_Handle.list);
	}
	void Dynamic::Update(float delta)
	{
		m_Pos += m_Vel * delta;
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
	void Dynamic::Init(QTNode* const root)
	{
		SetState(m_CurrentState);
		m_Dim = GetCurrentSprite()->GetDims();
		UpdateVertices();

		if(m_Added)
			m_Hitbox = new Hitbox(m_Pos - m_Dim / 2.f, m_Dim, m_Vel, root);
	}
}
