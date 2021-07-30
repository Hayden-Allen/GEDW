#pragma once
#include "Registers.h"
#include "Command.h"
#include "world/World.h"

namespace engine
{
	class ScriptParser;
	class Scriptable;
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
			{ 'v', 32 },
			{ 'f', 40 }
		};


		bool m_Compiled, m_Abort, m_Sleeping;
		uchar m_Memory[s_MemCount];
		uint m_ProgramCounter, m_EntryPoint, m_StackPointer;
		ulong m_Stack[s_StackCount];
		float m_SleepEnd;
		Registers m_Registers;
		std::vector<Args> m_Instructions;
		std::vector<Dynamic*> m_SpawnQueue;


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
	void name(const Arg& arg, float current, float delta, World* const world, Scriptable* const host, std::vector<Scriptable*>& env) { code }

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
		};
	};
}
