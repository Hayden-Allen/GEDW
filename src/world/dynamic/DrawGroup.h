#pragma once
#include "IndexedList.h"
#include "DrawGroupList.h"

namespace engine
{
	class Sprite;
	class Renderer;
	class DynamicList;

	class DrawGroup : public IndexedList<uint, uint, uint, uint>
	{
	public:
		friend class DynamicList;


		DrawGroup();
		DrawGroup(const DrawGroup& other) = delete;
		DrawGroup(DrawGroup&& other) = delete;
		~DrawGroup()
		{
			delete m_Buffer;
		}


		uint Add(uint dynamic) override;
		void Remove(uint i) override;
		void Draw(DynamicList& list, Renderer& renderer) const;
	private:
		DrawGroupList::Handle m_Index;
		gfx::IndexBuffer<GL_DYNAMIC_DRAW>* m_Buffer;
		mutable std::vector<Sprite*> m_SpriteBuffer;
	};
}
