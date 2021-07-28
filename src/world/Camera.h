#pragma once
#include "pch.h"
#include "dynamic/Dynamic.h"

namespace engine
{
	class Camera
	{
	public:
		Camera(const Dynamic* const target) :
			m_Target(target),
			m_Offset(0.f, 0.f)
		{}
		Camera(const Camera& other) = delete;
		Camera(Camera&& other) = delete;


		void Update()
		{
			m_Offset = -1.f * m_Target->GetPos();
		}
		const math::Vec2<float>& GetOffset() const
		{
			return m_Offset;
		}
	private:
		const Dynamic* const m_Target;
		math::Vec2<float> m_Offset;
	};
}
