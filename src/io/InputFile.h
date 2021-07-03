#pragma once
#include "File.h"

namespace engine
{
	class InputFile : public File<std::ios::in | std::ios::binary>
	{
	public:
		InputFile(const char* fp) : File(fp) {}
		InputFile(const InputFile& other) = delete;
		InputFile(InputFile&& other) = delete;


		uchar Byte()
		{
			return Next();
		}
		ushort Short()
		{
			return (CAST(ushort, Next()) << 8) | Next();
		}
		uint Int()
		{
			return (CAST(uint, Short()) << 16) | Short();
		}
		ulong Long()
		{
			return (CAST(ulong, Int()) << 32) | Int();
		}
		void Read(uchar* buffer, uint count)
		{
			m_File.read(PUN(char*, buffer), count);
		}
	};
}
