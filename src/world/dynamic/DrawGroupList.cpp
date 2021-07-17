#include "pch.h"
#include "DrawGroupList.h"
#include "DrawGroup.h"

namespace engine
{
	DrawGroupList::~DrawGroupList()
	{
		for (uint i = 0; i < m_Count; i++)
			delete m_List[i];
	}
}
