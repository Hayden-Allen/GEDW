#pragma once
#include "IndexedList.h"
#include "DrawGroupList.h"

namespace engine
{
	class Dynamic;
	class Renderer;
	struct ScriptRuntime;

	struct DynamicListHandle
	{
		uint list, group, texture;
	};

	class DynamicList : public IndexedList<Dynamic*, DynamicListHandle, Dynamic* const, Dynamic* const>
	{
	public:
		constexpr static uint s_MaxDynamics = 1024;
		friend class DrawGroup;


		DynamicList();
		DynamicList(const DynamicList& other) = delete;
		DynamicList(DynamicList&& other) = delete;
		~DynamicList();


		DynamicListHandle Add(Dynamic* const d) override;
		void Remove(Dynamic* const d) override;
		void Draw(Renderer& renderer);
		void Update(uint i);
		void Update(QTNode* const root, ScriptRuntime& rt);
	private:
		gfx::VertexArray<GL_DYNAMIC_DRAW>* m_VertexArray;
		DrawGroupList m_DrawGroups;
	};
}
