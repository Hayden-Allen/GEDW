#pragma once
#include "pch.h"

namespace engine
{
	struct Registers
	{
		constexpr static uint s_IntRegCount = 26, s_FloatRegCount = 14, s_VecRegCount = 12;
		// upper and lower 32 bits of a 64-bit register
		constexpr static ulong s_RegMaskHi = 0xffffffff00000000, s_RegMaskLo = 0xffffffff;
		// special register indices
		constexpr static uint s_RegObj = 22, s_RegHost = 23, s_RegObjCount = 24, s_RegFlags = 25;

		int64_t i[s_IntRegCount] = { 0 };
		double f[s_FloatRegCount] = { 0.f };
		math::Vec2<float> v[s_VecRegCount];
	};
}
