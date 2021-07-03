#pragma once
#include "pch.h"

namespace engine
{
	template<std::ios_base::openmode MODE>
	class File
	{
	protected:
		std::ifstream m_File;


		File(const char* fp) :
			m_File(fp, MODE)
		{
			if (!m_File.is_open())
				printf("Error opening file '%s'\n", fp);
		}
		File(const File& other) = delete;
		File(File&& other) = delete;
		~File()
		{
			m_File.close();
		}


		uchar Next()
		{
			if (m_File.eof())
			{
				printf("End of file reached\n");
				return 0;
			}
			char buf;
			m_File.read(&buf, 1);
			return PUN(uchar, buf);
		}
	};
}
