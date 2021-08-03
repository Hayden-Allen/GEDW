#pragma once
#include "pch.h"
#include "DynamicList.h"
#include "world/Hitbox.h"
#include "script/Scriptable.h"

namespace engine
{
	class Sprite;
	class Renderer;

	struct DynamicTemplate
	{
		std::unordered_map<std::string, Script*> scripts;
		std::unordered_map<std::string, Sprite*> states;
		std::string state;
		float speed;
	};

	class Dynamic : public Scriptable
	{
	public:
		friend class DynamicList;


		Dynamic(QTNode* const root, DynamicList& dl, const DynamicTemplate& temp, bool add);
		Dynamic(const std::unordered_map<std::string, Script*>& scripts, QTNode* const root, DynamicList& list, const math::Vec2<float>& pos, const math::Vec2<float>& vel, float speed, const std::unordered_map<std::string, Sprite*>& states, const std::string& state);
		Dynamic(const Dynamic& other) = delete;
		Dynamic(Dynamic&& other) = delete;
		~Dynamic()
		{
			delete m_Hitbox;
		}


		const std::unordered_map<std::string, int64_t>& RunScripts(ScriptRuntime& rt) override;
		void MoveHitbox(QTNode* const root);
		void ResolveCollisions(float delta, DynamicList& list);
		void Update(float delta);
		Sprite* const GetCurrentSprite() const
		{
			return GetCurrentState<Sprite>();
		}
		const float* const GetVertices() const
		{
			return m_Vertices;
		}
		void AddTo(QTNode* const root, DynamicList& dl)
		{
			if (m_Added)
			{
				printf("Cannot add the same Dynamic to a DynamicList twice\n");
				return;
			}
			m_Handle = dl.Add(this);
			m_Hitbox = new Hitbox(m_Pos, m_Dim, m_Vel, root);
			m_Added = true;
		}
	private:
		Hitbox* m_Hitbox;
		float m_Vertices[s_FloatsPerDynamic];
		DynamicList::Handle m_Handle;
		bool m_Added;


		void UpdateVertices();
		void Init(QTNode* const root);
	};
}
