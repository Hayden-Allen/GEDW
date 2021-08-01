#pragma once
#include "Registers.h"
#include "Command.h"
#include "world/World.h"
#include "Scriptable.h"

namespace engine
{
	class ScriptParser;
	class Renderer;


	struct ScriptRuntime
	{
		const Renderer* const renderer;
		World* const world;
	};


	class Script
	{
	private:
		typedef int64_t integer;
		typedef double fp;
		typedef math::Vec2<float> vec;
	public:
		friend class ScriptParser;


		Script(const char* fp);
		Script(const Script& other) = delete;
		Script(Script&& other) = delete;


		integer Run(const ScriptRuntime& rt, Scriptable* const host, std::vector<Scriptable*>& env);
	private:
		// 8KB stack, 4KB RAM, 128 supported operations
		constexpr static uint s_StackCount = 8192 / (sizeof(ulong) / sizeof(uchar)), s_MemCount = 4096, s_OpCount = 128;
		// special value representing the "host" of this Script invocation
		constexpr static int s_HostIndex = -1;
		// special register indices
		const static inline std::unordered_map<std::string, int64_t> s_SpecialRegisters =
		{
			{ "$obj", 49 },
			{ "$hst", 50 },
			{ "$oc", 51 },
			{ "$flags", 52 }
		};
		// register index offsets
		const static inline std::unordered_map<char, int64_t> s_RegisterOffsets =
		{
			{ 't', 0 },
			{ 's', 8 },
			{ 'a', 16 },
			{ 'r', 24 },
			{ 'f', 32 },
			{ 'v', 40 }
		};


		bool m_Compiled, m_Abort, m_Sleeping;
		uchar m_Memory[s_MemCount];
		uint m_ProgramCounter, m_EntryPoint, m_StackPointer;
		ulong m_Stack[s_StackCount];
		float m_SleepEnd;
		Registers m_Registers;
		std::vector<Args> m_Instructions;
		std::vector<Dynamic*> m_SpawnQueue;
		std::string m_Filepath;


		integer* const GetIntRegister(uint index);
		fp* const GetFloatRegister(uint index);
		vec* const GetVecRegister(uint index);
		bool RangeCheck(integer i, integer min, integer max);
		template<typename T>
		void StackPush(const T& t)
		{
			if (m_StackPointer >= s_StackCount)
			{
				printf("Stack overflow\n");
				m_Abort = true;
				return;
			}

			m_Stack[m_StackPointer++] = PUN(ulong, t);
		}
		template<typename T>
		T StackPop()
		{
			if (m_StackPointer == 0)
			{
				printf("Stack underflow\n");
				m_Abort = true;
				return CAST(T, 0);
			}

			return PUN(T, m_Stack[--m_StackPointer]);
		}


#define I(name, code) \
	void name(const Args& args, float current, float delta, World* const world, Scriptable* const host, std::vector<Scriptable*>& env) { code }
#define ROI(r, i) ((r) ? (*r) : (i))

		// math
		I(add,
			*args.i[2] = *args.i[0] + ROI(args.i[1], args.imm1i);
		);
		I(addf,
			*args.f[2] = *args.f[0] + ROI(args.f[1], args.imm1f);
			);
		I(addv,
			*args.v[2] = *args.v[0] + ROI(args.v[1], ROI(args.f[1], args.imm1f));
			);
		I(sub,
			*args.i[2] = *args.i[0] - ROI(args.i[1], args.imm1i);
		);
		I(subf,
			*args.f[2] = *args.f[0] - ROI(args.f[1], args.imm1f);
		);
		I(subv,
			*args.v[2] = *args.v[0] - ROI(args.v[1], ROI(args.f[1], args.imm1f));
		);
		I(mul,
			*args.i[2] = *args.i[0] * ROI(args.i[1], args.imm1i);
		);
		I(mulf,
			*args.f[2] = *args.f[0] * ROI(args.f[1], args.imm1f);
		);
		I(mulv,
			*args.v[2] = *args.v[0] * ROI(args.v[1], ROI(args.f[1], args.imm1f));
		);
		I(div,
			*args.i[2] = *args.i[0] / ROI(args.i[1], args.imm1i);
		);
		I(divf,
			*args.f[2] = *args.f[0] / ROI(args.f[1], args.imm1f);
		);
		I(divv,
			*args.v[2] = *args.v[0] / ROI(args.v[1], ROI(args.f[1], args.imm1f));
		);
		// math.bit
		I(band,
			*args.i[2] = *args.i[0] & ROI(args.i[1], args.imm1i);
		);
		I(bxor,
			*args.i[2] = *args.i[0] ^ ROI(args.i[1], args.imm1i);
		);
		I(bor,
			*args.i[2] = *args.i[0] | ROI(args.i[1], args.imm1i);
		);
		I(bnot,
			*args.i[1] = ~ROI(args.i[0], args.imm1i);
		);
		I(sl,
			*args.i[2] = (*args.i[0]) << ROI(args.i[1], args.imm1i);
		);
		I(sr,
			*args.i[2] = (*args.i[0]) >> ROI(args.i[1], args.imm1i);
		);
		// math.trig
		I(sine,
			*args.f[1] = math::sin(*args.f[0]);
		);
		I(cosine,
			*args.f[1] = math::cos(*args.f[0]);
		);
		I(tangent,
			*args.f[1] = math::tan(*args.f[0]);
		);
		I(arcsine,
			*args.f[1] = math::asin(*args.f[0]);
		);
		I(arccosine,
			*args.f[1] = math::acos(*args.f[0]);
		);
		I(arctangent,
			*args.f[2] = math::atan(*args.f[0], *args.f[1]);
		);
		// math.fn
		I(min,
			*args.i[2] = math::min(*args.i[0], ROI(args.i[1], args.imm1i));
		);
		I(max,
			*args.i[2] = math::max(*args.i[0], ROI(args.i[1], args.imm1i));
		);
		I(minf,
			*args.f[2] = math::min(*args.f[0], ROI(args.f[1], args.imm1f));
		);
		I(maxf,
			*args.f[2] = math::max(*args.f[0], ROI(args.f[1], args.imm1f));
		);
		I(minv,
			*args.f[1] = math::min(args.v[0]->x, args.v[0]->y);
			);
		I(maxv,
			*args.f[1] = math::max(args.v[0]->x, args.v[0]->y);
			);
		I(power,
			*args.f[2] = math::pow(*args.f[0], ROI(args.f[1], args.imm1f));
			);
		I(squareroot,
			*args.f[1] = math::sqrt(ROI(args.f[0], args.imm1f));
			);
		I(absolute,
			*args.i[1] = math::abs(ROI(args.i[0], args.imm1i));
			);
		I(absolutef,
			*args.f[1] = math::abs(ROI(args.f[0], args.imm1f));
		);
		I(absolutev,
			*args.v[1] = args.v[0]->Abs();
		);
		I(random,
			*args.i[2] = math::rand(*args.i[0], ROI(args.i[1], args.imm1i));
			);
		I(randomf,
			*args.f[2] = math::rand(*args.f[0], ROI(args.f[1], args.imm1f));
		);
		I(sign,
			*args.i[1] = math::sign(ROI(args.i[0], args.imm1i));
		);
		I(signf,
			*args.f[1] = math::sign(ROI(args.f[0], args.imm1f));
		);
		I(signv,
			*args.v[1] = args.v[0]->Unit();
		);
		// math.vec
		I(dot,
			*args.f[2] = args.v[0]->Dot(*args.v[1]);
			);
		I(mag,
			*args.f[1] = args.v[0]->Magnitude();
			);
		I(ang,
			*args.f[1] = math::deg(args.v[0]->Angle());
			);
		I(angv,
			*args.f[2] = math::deg(args.v[0]->AngleBetween(*args.v[1]));
			);
		I(norm,
			*args.v[1] = args.v[0]->Normalized();
			);
		// mem
		I(psh,
			if (args.i[0])
				StackPush(*args.i[0]);
			else if (args.f[0])
				StackPush(*args.f[0]);
			else
				StackPush(*args.v[0]);
		);
		I(pop,
			if (args.i[0])
				*args.i[0] = StackPop<integer>();
			else if (args.f[0])
				*args.f[0] = StackPop<fp>();
			else
				*args.v[0] = StackPop<vec>();
		);
		I(mov,
			*args.i[1] = CAST(integer, ROI(args.f[0], ROI(args.i[0], args.imm1i)));
		);
		I(movl,
			*args.i[1] &= Registers::s_RegMaskHi;
			*args.i[1] |= ROI(args.i[0], args.imm1i);
		);
		I(movh,
			*args.i[1] &= Registers::s_RegMaskLo;
			*args.i[1] |= (ROI(args.i[0], args.imm1i) << 32);
		);
		I(movf,
			*args.f[1] = CAST(fp, ROI(args.f[0], ROI(args.i[0], args.imm1f)));
		);
		I(movv,
			*args.v[1] = ROI(args.v[0], ROI(args.i[0], ROI(args.f[0], args.imm1f)));
		);
		I(movx,
			if (args.v[0])
				args.v[1]->x = args.v[0]->x;
			else
				args.v[1]->x = CAST(float, ROI(args.i[0], ROI(args.f[0], args.imm1f)));
		);
		I(movy,
			if (args.v[0])
				args.v[1]->y = args.v[0]->y;
			else
				args.v[1]->y = CAST(float, ROI(args.i[0], ROI(args.f[0], args.imm1f)));
		);
		I(stm,
			const integer index = ROI(args.i[1], args.imm1i);
			if (!RangeCheck(index, 0, s_MemCount))
				return;

			// write to memory in chunks of 8 bytes by casting to a ulong pointer
			if (args.i[0])
				*((ulong*)(&m_Memory[index])) = *(ulong*)args.i[0];
			else if (args.f[0])
				*((ulong*)(&m_Memory[index])) = *(ulong*)args.f[0];
			else if (args.v[0])
				*((ulong*)(&m_Memory[index])) = *(ulong*)args.v[0];
		);
		I(ldm,
			const integer index = ROI(args.i[1], args.imm1i);
			if (!RangeCheck(index, 0, s_MemCount))
				return;

			if (args.i[1])
				*args.i[1] = PUN(integer, m_Memory[index]);
			else if (args.f[1])
				*args.f[1] = PUN(fp, m_Memory[index]);
			else if (args.v[1])
				*args.v[1] = PUN(vec, m_Memory[index]);
			);
		// ctrl
		I(beq,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;

			if (ROI(args.i[0], ROI(args.f[0], *args.v[0])) == ROI(args.i[1], ROI(args.f[1], *args.v[1])))
				m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(beqz,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;

			if (ROI(args.i[0], ROI(args.f[0], *args.v[0])) == 0)
				m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(bne,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;

			if (ROI(args.i[0], ROI(args.f[0], *args.v[0])) != ROI(args.i[1], ROI(args.f[1], *args.v[1])))
				m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(blt,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;

			if (ROI(args.i[0], *args.f[0]) < ROI(args.i[1], *args.f[1]))
				m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(bgt,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;

			if (ROI(args.i[0], *args.f[0]) > ROI(args.i[1], *args.f[1]))
				m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(ble,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;

			if (ROI(args.i[0], *args.f[0]) <= ROI(args.i[1], *args.f[1]))
				m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(bge,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;

			if (ROI(args.i[0], *args.f[0]) >= ROI(args.i[1], *args.f[1]))
				m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(j,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;
			m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(call,
			if (!RangeCheck(args.imm1i, 0, m_Instructions.size()))
				return;
			StackPush(m_ProgramCounter);
			m_ProgramCounter = CAST(uint, args.imm1i) - 1;
		);
		I(ret,
			m_ProgramCounter = StackPop<uint>();
		);
		I(end,
			m_Abort = true;
		);
		I(slp,
			m_SleepEnd = current + ROI(args.i[0], args.imm1i);
			m_Sleeping = true;
			m_Abort = true;
		);
		I(blk,
			math::sleep(CAST(uint, ROI(args.i[0], args.imm1i)));
		);
		// debug
		I(dbg,
			printf("[%s]: %lld\n", m_Filepath.c_str(), ROI(args.i[0], args.imm1i));
		);
		I(dbgf,
			printf("[%s]: %f\n", m_Filepath.c_str(), ROI(args.f[0], args.imm1f));
		);
		I(dbgv,
			printf("[%s]: <%f, %f>\n", m_Filepath.c_str(), args.v[0]->x, args.v[0]->y);
		);
		I(dbgs,
			printf("[%s]: %s\n", m_Filepath.c_str(), (char*)(m_Memory + ROI(args.i[0], args.imm1i)));
		);
		// engine
		I(gettime,
			*args.f[0] = current;
		);
		// engine.input
		I(imp,
			*args.v[0] = world->m_Engine->GetCursorPos();
		);
		I(ims,
			*args.v[0] = world->m_Engine->GetScroll();
		);
		I(imb,
			*args.i[1] = CAST(integer, world->m_Engine->IsMousePressed(CAST(uint, ROI(args.i[0], args.imm1i))));
		);
		I(ikp,
			*args.i[1] = CAST(integer, world->m_Engine->IsKeyPressed(CAST(uint, ROI(args.i[0], args.imm1i))));
		);
		I(ikd,
			const integer a = CAST(integer, world->m_Engine->IsKeyPressed(CAST(uint, ROI(args.i[0], args.imm1i))));
			const integer b = CAST(integer, world->m_Engine->IsKeyPressed(CAST(uint, ROI(args.i[1], args.imm2i))));
			*args.i[2] = a - b;
		);
		// engine.obj
#define CS (m_Registers.i[Registers::s_RegObj] == s_HostIndex ? host : env[m_Registers.i[Registers::s_RegObj]])
		I(ogp,
			*args.v[0] = CS->GetPos();
		);
		I(osp,
			CS->SetPos(*args.v[0]);
		);
		I(ogv,
			*args.v[0] = CS->GetVel();
		);
		I(osv,
			CS->SetVel(*args.v[0]);
		);
		I(ogd,
			*args.v[0] = CS->GetDims();
		);
		I(ogs,
			*args.f[0] = CS->GetSpeed();
		);
		I(oss,
			CS->SetState((char*)(m_Memory + ROI(args.i[0], args.imm1i)));
		);
		I(spn,
			Dynamic* d = world->CreateDynamic((char*)(m_Memory + ROI(args.i[0], args.imm1i)), false);
			env.push_back((Scriptable*)d);
			m_Registers.i[Registers::s_RegObjCount]++;
			m_SpawnQueue.push_back(d);
			*args.i[1] = env.size() - 1;
		);
#undef CS
#undef ROI
#undef I


		typedef void(Script::* Operation)(const Args&, float, float, World* const, Scriptable* const, std::vector<Scriptable*>&);
		// these 3 will be auto-populated from s_Instructions
		static inline Operation s_Operations[s_OpCount];
		static inline std::unordered_map<uint, std::string> s_CommandNames;
		static inline std::unordered_map<std::string, CommandDescription> s_CommandDescriptions;
		struct Instruction
		{
			std::string name;
			std::vector<ArgType> desc;
			Operation op;
		};
		const static inline std::vector<Instruction> s_Instructions =
		{
			// math
			{ "add",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::add },
			{ "addf",	{ ArgType::F, ArgType::F_MF, ArgType::F }, &Script::addf },
			{ "addv",	{ ArgType::V, ArgType::F_V_MF, ArgType::V }, &Script::addv },
			{ "sub",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::sub },
			{ "subf",	{ ArgType::F, ArgType::F_MF, ArgType::F }, &Script::subf },
			{ "subv",	{ ArgType::V, ArgType::F_V_MF, ArgType::V }, &Script::subv },
			{ "mul",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::mul },
			{ "mulf",	{ ArgType::F, ArgType::F_MF, ArgType::F }, &Script::mulf },
			{ "mulv",	{ ArgType::V, ArgType::F_V_MF, ArgType::V }, &Script::mulv },
			{ "div",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::div },
			{ "divf",	{ ArgType::F, ArgType::F_MF, ArgType::F }, &Script::divf },
			{ "divv",	{ ArgType::V, ArgType::F_V_MF, ArgType::V }, &Script::divv },
			// math.bit
			{ "and",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::band },
			{ "xor",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::bxor },
			{ "or",		{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::bor },
			{ "not",	{ ArgType::I_MI, ArgType::I }, &Script::bnot },
			{ "sl",		{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::sl },
			{ "sr",		{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::sr },
			// math.trig
			{ "sin",		{ ArgType::F, ArgType::F }, &Script::sine },
			{ "cos",		{ ArgType::F, ArgType::F }, &Script::cosine },
			{ "tan",		{ ArgType::F, ArgType::F }, &Script::tangent },
			{ "asin",		{ ArgType::F, ArgType::F }, &Script::arcsine },
			{ "acos",		{ ArgType::F, ArgType::F }, &Script::arccosine },
			{ "atan",		{ ArgType::F, ArgType::F }, &Script::arctangent },
			// math.fn
			{ "min",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::min },
			{ "max",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::max },
			{ "minf",	{ ArgType::F, ArgType::F_MF, ArgType::F }, &Script::minf },
			{ "maxf",	{ ArgType::F, ArgType::F_MF, ArgType::F }, &Script::maxf },
			{ "minv",	{ ArgType::V, ArgType::F }, &Script::minv },
			{ "maxv",	{ ArgType::V, ArgType::F }, &Script::maxv },
			{ "pow",	{ ArgType::F, ArgType::F_MF, ArgType::F }, &Script::power },
			{ "sqrt",	{ ArgType::F_MF, ArgType::F }, &Script::squareroot },
			{ "abs",	{ ArgType::I_MI, ArgType::I }, &Script::absolute },
			{ "absf",	{ ArgType::F_MF, ArgType::F }, &Script::absolutef },
			{ "absv",	{ ArgType::V, ArgType::V }, &Script::absolutev },
			{ "rand",	{ ArgType::I, ArgType::I_MI, ArgType::I }, &Script::random },
			{ "randf",	{ ArgType::F, ArgType::F_MF, ArgType::F }, &Script::randomf },
			{ "sign",	{ ArgType::I_MI, ArgType::I }, &Script::sign },
			{ "signf",	{ ArgType::F_MF, ArgType::F }, &Script::signf },
			{ "signv",	{ ArgType::V, ArgType::V }, &Script::signv },
			// math.vec
			{ "dot",	{ ArgType::V, ArgType::V, ArgType::F }, &Script::dot },
			{ "mag",	{ ArgType::V, ArgType::F }, &Script::mag },
			{ "ang",	{ ArgType::V, ArgType::F }, &Script::ang },
			{ "angv",	{ ArgType::V, ArgType::V, ArgType::F }, &Script::angv },
			{ "norm",	{ ArgType::V, ArgType::V }, &Script::norm },
			// mem
			{ "psh",	{ ArgType::I_F_V }, &Script::psh },
			{ "pop",	{ ArgType::I_F_V }, &Script::pop },
			{ "mov",	{ ArgType::I_F_MI, ArgType::I }, &Script::mov },
			{ "movl",	{ ArgType::I_MI, ArgType::I }, &Script::movl },
			{ "movh",	{ ArgType::I_MI, ArgType::I }, &Script::movh },
			{ "movf",	{ ArgType::I_F_MF, ArgType::F }, &Script::movf },
			{ "movv",	{ ArgType::I_F_V_MF, ArgType::V }, &Script::movv },
			{ "movx",	{ ArgType::I_F_V_MF, ArgType::V }, &Script::movx },
			{ "movy",	{ ArgType::I_F_V_MF, ArgType::V }, &Script::movy },
			{ "stm",	{ ArgType::I_F_V, ArgType::I_MI }, &Script::stm },
			{ "ldm",	{ ArgType::I_MI, ArgType::I_F_V }, &Script::ldm },
			// ctrl
			{ "beq",	{ ArgType::I_F_V, ArgType::I_F_V, ArgType::L_MI }, &Script::beq },
			{ "beqz",	{ ArgType::I_F_V, ArgType::L_MI }, &Script::beqz },
			{ "bne",	{ ArgType::I_F_V, ArgType::I_F_V, ArgType::L_MI }, &Script::bne },
			{ "blt",	{ ArgType::I_F, ArgType::I_F, ArgType::L_MI }, &Script::blt },
			{ "bgt",	{ ArgType::I_F, ArgType::I_F, ArgType::L_MI }, &Script::bgt },
			{ "ble",	{ ArgType::I_F, ArgType::I_F, ArgType::L_MI }, &Script::ble },
			{ "bge",	{ ArgType::I_F, ArgType::I_F, ArgType::L_MI }, &Script::bge },
			{ "j",		{ ArgType::L_MI }, &Script::j },
			{ "call",	{ ArgType::L_MI }, &Script::call },
			{ "ret",	{ }, &Script::ret },
			{ "end",	{ }, &Script::end },
			{ "slp",	{ ArgType::I_MI }, &Script::slp },
			{ "blk",	{ ArgType::I_MI }, &Script::blk },
			// debug
			{ "dbg",	{ ArgType::I_MI }, &Script::dbg },
			{ "dbgf",	{ ArgType::F_MF }, &Script::dbgf },
			{ "dbgv",	{ ArgType::V }, &Script::dbgv },
			{ "dbgs",	{ ArgType::I_MI_MS }, &Script::dbgs },
			// engine
			{ "time",	{ ArgType::F }, &Script::gettime },
			// engine.input
			{ "imp",	{ ArgType::V }, &Script::imp },
			{ "ims",	{ ArgType::V }, &Script::ims },
			{ "imb",	{ ArgType::I_MI, ArgType::I }, &Script::imb },
			{ "ikp",	{ ArgType::I_MI, ArgType::I }, &Script::ikp },
			{ "ikd",	{ ArgType::I_MI, ArgType::I_MI, ArgType::I }, &Script::ikd },
			// engine.obj
			{ "ogp",	{ ArgType::V }, &Script::ogp },
			{ "osp",	{ ArgType::V }, &Script::osp },
			{ "ogv",	{ ArgType::V }, &Script::ogv },
			{ "osv",	{ ArgType::V }, &Script::osv },
			{ "ogd",	{ ArgType::V }, &Script::ogd },
			{ "ogs",	{ ArgType::F }, &Script::ogs },
			{ "oss",	{ ArgType::I_MI_MS }, &Script::oss },
			{ "spn",	{ ArgType::I_MI_MS, ArgType::I }, &Script::spn }
		};
	};
}
