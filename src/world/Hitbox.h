#pragma once
#include "pch.h"

namespace engine
{
	class Hitbox : public math::QuadTreeElement<s_QuadTreeThreshold>
	{
	public:
		Hitbox(const math::Vec2<float>& pos, const math::Vec2<float>& dim, const math::Vec2<float>& vel, Node* const root) :
			Element(pos, dim, vel)
		{
			if (root)
				root->Add(this);
		}
		Hitbox(const Hitbox& other) = delete;
		Hitbox(Hitbox&& other) noexcept :
			Element(std::move(other))
		{}


		void ResolveCollision(const CollisionInfo& info) override
		{
			// bad hacky math. Removes enough velocity such that the collision won't happen in the first place
			m_Vel += info.normal * m_Vel.Abs() * (1 - info.time);
		}
	private:
		constexpr static math::RangeOverlapsParams s_OverlapsParams = { .left = { true, true }, .right = { true, true } };



		bool IsContainedBy(const Node* const node) const override
		{
			return math::rectIntersect(node->GetPos(), math::Vec2<float>(1.f * node->GetDim(), 1.f * node->GetDim()), m_Pos, m_Dim, s_OverlapsParams);
		}
		bool Intersects(const QuadTreeElement* const other, float delta, math::Vec2<float>* const normal, math::Vec2<float>* const contact, float* const time) const override
		{
			// cast a ray from the middle of this hitbox at the other hitbox
			return math::Ray(m_Pos + m_Dim / 2.f, m_Vel * delta).IntersectsRect(other->GetPos() - m_Dim / 2.f, other->GetDim() + m_Dim, normal, contact, time);
		}
	};
}
