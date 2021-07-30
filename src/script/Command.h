#pragma once
#include "pch.h"

namespace engine
{
	enum class ArgType
	{
		// error type
		NONE = 0,
		// int register, float register, vec register, int immediate, float immediate, label, string literal
		I = 1, F = 2, V = 4, MI = 8, MF = 16, L = 32, MS = 64,
		// combo types (for when multiple argument types are allowed for a given instruction)
		I_F = I | F,
		I_MI = I | MI,
		F_MF = F | MF,
		V_MF = V | MF,
		L_MI = L | MI,
		I_F_V = I | F | V,
		I_F_MI = I | F | MI,
		I_F_MF = I | F | MF,
		F_V_MF = F | V | MF,
		I_MI_MS = I | MI | MS,
		I_F_V_MF = I | F | V | MF,
		I_F_V_MI_MF = I | F | V | MI | MF
	};
	static bool operator&(ArgType a, ArgType b)
	{
		return CAST(uint, a) & CAST(uint, b);
	}
	static bool IsIntImmediate(ArgType a)
	{
		return a == ArgType::MI || a == ArgType::MS;
	}
	static bool IsImmediate(ArgType a)
	{
		return IsIntImmediate(a) || a == ArgType::MF;
	}



	struct CommandDescription
	{
		constexpr static uint s_RegCount = 3;


		uchar opcode;
		std::vector<ArgType> args;
	};



	struct Args
	{
		// pointers to registers
		int64_t* i[CommandDescription::s_RegCount] = { nullptr };
		double* f[CommandDescription::s_RegCount] = { nullptr };
		math::Vec2<float>* v[CommandDescription::s_RegCount] = { nullptr };
		uchar opcode;
		// immediate values
		int64_t imm1i = 0, imm2i = 0;
		double imm1f = 0.f;
	};
}
