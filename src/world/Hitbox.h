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
			const auto& op = info.element->GetPos(), od = info.element->GetDim();
			const float distances[4] =
			{
				// top
				math::abs(m_Pos.y - (op.y + od.y)),
				// left
				math::abs((m_Pos.x + m_Dim.x) - op.x),
				// bottom
				math::abs((m_Pos.y + m_Dim.y) - op.y),
				// right
				math::abs(m_Pos.x - (op.x + od.x))
			};

			// determine which direction a collision is most likely to occur from
			uint min = 0;
			for (uint i = 1; i < 4; i++)
				min = (distances[i] < distances[min] ? i : min);

			// if this overlaps info.element on the x-axis
			if (m_Pos.x + m_Dim.x >= op.x && m_Pos.x <= op.x + od.x)
			{
				// collision between bottom of this and top of info.element
				if (min == 0 && m_Pos.y < op.y + od.y)
				{
					m_Pos.y = op.y + od.y;
					info.element->SetVel({ info.element->GetVel().x, 0.f });
					m_Vel.y = 0.f;
				}
				// collision between top of this and bottom of info.element
				if (min == 2 && m_Pos.y + m_Dim.y > op.y)
				{
					m_Pos.y = op.y - m_Dim.y;
					info.element->SetVel({ info.element->GetVel().x, 0.f });
					m_Vel.y = 0.f;
				}
			}
			// if this overlaps info.element on the y-axis
			if (m_Pos.y + m_Dim.y >= op.y && m_Pos.y <= op.y + od.y)
			{
				// collision between right of this and left of info.element
				if (min == 1 && m_Pos.x + m_Dim.x > op.x)
				{
					m_Pos.x = op.x - m_Dim.x;
					info.element->SetVel({ 0.f, info.element->GetVel().y });
					m_Vel.x = 0.f;
				}
				// collision between left of this and right of info.element
				if (min == 3 && m_Pos.x < op.x + od.x)
				{
					m_Pos.x = op.x + od.x;
					info.element->SetVel({ 0.f, info.element->GetVel().y });
					m_Vel.x = 0.f;
				}
			}
		}
	private:
		constexpr static math::RangeOverlapsParams s_OverlapsParams = { .left = { true, true }, .right = { true, true } };



		bool IsContainedBy(const Node* const node) const override
		{
			return math::rectIntersect(node->GetPos(), math::Vec2<float>(1.f * node->GetDim(), 1.f * node->GetDim()), m_Pos, m_Dim, s_OverlapsParams);
		}
		bool Intersects(const QuadTreeElement* const other, float delta, math::Vec2<float>* const normal, math::Vec2<float>* const contact, float* const time) const override
		{
			return math::rectIntersect(m_Pos, m_Dim, other->GetPos(), other->GetDim(), s_OverlapsParams);
		}
	};
}
