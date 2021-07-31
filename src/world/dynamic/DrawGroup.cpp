#include "pch.h"
#include "DrawGroup.h"
#include "graphics/Sprite.h"
#include "graphics/Renderer.h"
#include "DynamicList.h"
#include "Dynamic.h"
#include "script/Script.h"

namespace engine
{
	DrawGroup::DrawGroup() :
		IndexedList<uint, uint, uint, uint>(gfx::getMaxTextureUnits(), DynamicList::s_MaxDynamics),
		m_Index(0),
		m_Buffer(new gfx::IndexBuffer<GL_DYNAMIC_DRAW>(gfx::getMaxTextureUnits() * s_IndicesPerQuad))
	{
		m_SpriteBuffer.reserve(gfx::getMaxTextureUnits());
		for (uint i = 0; i < m_Count; i++)
			m_SpriteBuffer.push_back(nullptr);
	}



	uint DrawGroup::Add(uint dynamic)
	{
		// index = AddBase()
		// create index data for given dynamic
		// upload that index data into our IB
		// return index

		const uint index = AddBase(dynamic);

		constexpr uint count = s_IndicesPerQuad;
		uint indices[count];
		for (uint i = 0; i < count; i++)
			indices[i] = dynamic * s_VerticesPerQuad + s_IndexOffsets[i];
		
		m_Buffer->Update(count, indices, index * count);

		return index;
	}
	void DrawGroup::Remove(uint i)
	{
		// RemoveBase();
		// remove relevant indices from our IB

		RemoveBase(i);

		constexpr uint count = s_IndicesPerQuad;
		uint indices[count] = { 0 };
		m_Buffer->Update(count, indices, i * count);
	}
	void DrawGroup::RunScripts(DynamicList& list, ScriptRuntime& rt) const
	{
		ForEach([&list, &rt](uint i) {list.m_List[i]->RunScripts(rt); });
	}
	void DrawGroup::Move(DynamicList& list, float delta) const
	{
		ForEach([&list, delta](uint i) { list.m_List[i]->Update(delta, list); });
	}
	void DrawGroup::MoveHitboxes(DynamicList& list, QTNode* const root) const
	{
		ForEach([&list, root](uint i) { list.m_List[i]->MoveHitbox(root); });
	}
	void DrawGroup::ResolveCollisions(DynamicList& list, float delta) const
	{
		ForEach([&list, delta](uint i) { list.m_List[i]->ResolveCollisions(delta); });
	}
	void DrawGroup::Draw(DynamicList& list, Renderer& renderer) const
	{
		// for each element in our list
		// if the element exists (!= m_Placeholder), update it and add it's current Sprite to our vector
		// draw everything using list.VA, this.IB, and this.m_SpriteBuffer
		for (uint i = 0; i < GetLast(); i++)
		{
			Sprite* ptr = nullptr;
			if (IsValid(i))
				ptr = list.m_List[m_List[i]]->GetCurrentSprite();
			m_SpriteBuffer[i] = ptr;
		}
		renderer.DrawDynamics(*list.m_VertexArray, *m_Buffer, m_SpriteBuffer);
	}
}
