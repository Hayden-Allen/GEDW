#include "pch.h"
#include "DynamicList.h"
#include "Dynamic.h"
#include "graphics/Renderer.h"
#include "DrawGroup.h"

namespace engine
{
	DynamicList::DynamicList() :
		IndexedList<Dynamic*, DynamicListHandle, Dynamic* const, Dynamic* const>(s_MaxDynamics, nullptr),
		m_VertexArray(new gfx::VertexArray<GL_DYNAMIC_DRAW>(s_MaxDynamics* s_FloatsPerDynamic, { 2, 2, 1, 1 })),
		m_DrawGroups(s_MaxDynamics / gfx::getMaxTextureUnits())
	{}
	DynamicList::~DynamicList()
	{
		for (uint i = 0; i < m_Count; i++)
			delete m_List[i];
		delete m_VertexArray;
	}

	

	DynamicListHandle DynamicList::Add(Dynamic* const d)
	{
		// set m_List[NextIndex()] = d;
		// find a Group for d and add it to that Group
		// return index and set d->index = to it
		const uint index = AddBase(d);

		uint drawGroupIndex = 0, textureIndex = 0;
		bool found = false;
		for (uint i = 0; !found && i < m_DrawGroups.GetLast(); i++)
		{
			DrawGroup* group = m_DrawGroups[i];
			if (group && !group->IsFull())
			{
				drawGroupIndex = i;
				textureIndex = group->Add(index);
				found = true;
			}
		}

		if (!found)
		{
			DrawGroup* group = new DrawGroup();
			textureIndex = group->Add(index);
			group->m_Index = m_DrawGroups.Add(group);
			drawGroupIndex = group->m_Index;
		}

		return { index, drawGroupIndex, textureIndex };
	};
	void DynamicList::Remove(Dynamic* const d)
	{
		// m_List[d->index] = nullptr
		// add d->index to m_Openings
		// clear relevant data in VA
		auto& indices = d->m_Handle;
		RemoveBase(indices.list);

		DrawGroup* group = m_DrawGroups[indices.group];
		group->Remove(indices.texture);

		if (group->IsEmpty())
			m_DrawGroups.Remove(group->m_Index);
	}
	void DynamicList::Update(QTNode* const root, float delta)
	{
		m_DrawGroups.ForEach([this, root](DrawGroup* g) { g->MoveHitboxes(*this, root); });
		m_DrawGroups.ForEach([this, delta](DrawGroup* g) { g->ResolveCollisions(*this, delta); });
		m_DrawGroups.ForEach([this, delta](DrawGroup* g) { g->Move(*this, delta); });
	}
	void DynamicList::Draw(Renderer& renderer)
	{
		// loop through all of our Groups and draw each one
		m_DrawGroups.ForEach([this, &renderer](DrawGroup* g) { g->Draw(*this, renderer); });
	}
	void DynamicList::Update(uint i)
	{
		// get new vertex data from m_List[i]
		// upload it to the right place in our VA
		if (!IsValid(i))
		{
			printf("Invalid DynamicList update index %u\n", i);
			return;
		}

		constexpr uint count = s_FloatsPerDynamic;
		m_VertexArray->GetBuffer().Update(count, m_List[i]->GetVertices(), i * count);
	}
}
