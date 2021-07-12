#pragma once
#include "pch.h"

namespace engine
{
	struct Light
	{
		// vec4 pos
		math::Vec2<float> pos = { 0.f, 0.f };
		float intensity = 0.f, padding1 = 0.f;
		// vec4 color
		float color[3] = { 0.f }, padding2 = 0.f;


		constexpr static uint s_MaxLightCount = 100;
	};
}
