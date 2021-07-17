#pragma once
#include "IndexedList.h"

namespace engine
{
	class DrawGroup;

	class DrawGroupList : public IndexedList<DrawGroup*, uint, DrawGroup* const, uint>
	{
	public:
		DrawGroupList(uint count) :
			IndexedList<DrawGroup*, uint, DrawGroup* const, uint>(count, nullptr)
		{}
		DrawGroupList(const DrawGroupList& other) = delete;
		DrawGroupList(DrawGroupList&& other) = delete;
		~DrawGroupList();

		uint Add(DrawGroup* const dg) override
		{
			return AddBase(dg);
		}
		void Remove(uint i) override
		{
			RemoveBase(i);
		}
	};
}
