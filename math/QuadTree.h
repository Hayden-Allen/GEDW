#pragma once
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include "Core.h"
#include "Range.h"
#include "Vec2.h"
#include "LinkedListNode.h"

namespace math
{
	/**
	 * QuadTreeNode.h
	 */

	template<uint THRESHOLD>
	class QuadTreeElement;

	template<uint THRESHOLD>
	class QuadTreeNode
	{
	private:
		typedef QuadTreeNode<THRESHOLD> Node;
		typedef QuadTreeElement<THRESHOLD> Element;
		typedef LinkedListNode<Element> ElementNode;
		friend class QuadTreeElement<THRESHOLD>;
	public:
		QuadTreeNode(const math::Vec2<float>& min, const math::Vec2<float>& max);
		QuadTreeNode(const Node& other) = delete;
		QuadTreeNode(Node&& other) = delete;
		~QuadTreeNode();


		void Add(Element* e);
		bool IsDivided() const
		{
			return m_Children[0];
		}
		ElementNode* const GetFirstElement()
		{
			return m_Data;
		}
		uint GetCount() const
		{
			return m_Count;
		}
		const math::Vec2<float>& GetPos() const
		{
			return m_Pos;
		}
		uint GetDim() const
		{
			return m_Dim;
		}
	private:
		// number of child Nodes each Node can contain, minimum side length of a Node (in simulated pixels)
		constexpr static uint s_Children = 4, s_MinDim = 2;
		// list of Elements contained by this Node
		ElementNode* m_Data;
		// list of child Nodes, parent Node
		Node* m_Children[s_Children], * m_Parent;
		// (x, y) of bottom left corner
		math::Vec2<float> m_Pos;
		// side length, number of contained Elements
		uint m_Dim, m_Count;


		QuadTreeNode(const math::Vec2<float>& pos, uint size, Node* const parent = nullptr);


		void DeleteElement(ElementNode* element);
		void DeleteData();
		void Divide();
		void GetUniqueElements(std::unordered_set<Element*>* const elements) const;
		void Merge();
	};


	/**
	 * QuadTreeElement.h
	 */
	template<uint THRESHOLD>
	class QuadTreeElement
	{
	protected:
		typedef QuadTreeNode<THRESHOLD> Node;
		typedef QuadTreeElement<THRESHOLD> Element;
		typedef LinkedListNode<Element> ElementNode;
		friend class QuadTreeNode<THRESHOLD>;


		struct CollisionInfo
		{
			Element* element;
			Vec2<float> normal, contact;
			float time;
		};
		struct NodeComparator
		{
			const bool operator()(const Node* const a, const Node* const b) const;
		};
	public:
		QuadTreeElement(const math::Vec2<float>& pos, const math::Vec2<float>& dim, const math::Vec2<float>& vel);
		QuadTreeElement(const Element& other) = delete;
		QuadTreeElement(Element&& other) = delete;
		virtual ~QuadTreeElement() {}


		virtual void ResolveCollisions(const CollisionInfo& info) = 0;
		void Move(const math::Vec2<float>& pos, const math::Vec2<float>& dim, const math::Vec2<float>& vel, Node* const root);
		void Update(float delta);
		const math::Vec2<float>& GetPos() const
		{
			return m_Pos;
		}
		const math::Vec2<float>& GetVel() const
		{
			return m_Vel;
		}
		const math::Vec2<float>& GetDim() const
		{
			return m_Dim;
		}
	protected:
		constexpr static uint s_MapReserveSize = THRESHOLD * THRESHOLD * THRESHOLD;
		// position, size, and velocity of the "host" object
		Vec2<float> m_Pos, m_Dim, m_Vel;
		// stores this Nodes that contain this Element and the ElementNode within each Node that they are actually stored in
		std::unordered_map<Node*, ElementNode*> m_Parents;
		// set of parent Nodes of all the Nodes in m_Parents
		std::set<Node*, NodeComparator> m_Grandparents;


		virtual bool IsContainedBy(const Node* const node) const = 0;
		virtual bool Intersects(const Element* const other, float delta, Vec2<float>* const normal, Vec2<float>* const contact, float* const time) const = 0;
		void AddTo(Node* const node, ElementNode* const container);
		void RemoveFrom(Node* const node);
		void Delete();
		void RemoveFromParents();
		void TryRemoveGrandparent(Node* const gp);
		void ClearGrandparents()
		{
			m_Grandparents.clear();
		}
		void Merge(std::set<Node*>& grandparents)
		{
			for (auto& gp : grandparents)
				gp->Merge();
		}
		void CopyHostValues(const Vec2<float>& pos, const Vec2<float>& dim, const Vec2<float>& vel)
		{
			m_Pos = pos;
			m_Dim = dim;
			m_Vel = vel;
		}
	};


	/**
	 * QuadTreeNode.cpp
	 */
	template<uint THRESHOLD>
	QuadTreeNode<THRESHOLD>::QuadTreeNode(const math::Vec2<float>& min, const math::Vec2<float>& max) :
		m_Data(nullptr),
		m_Children{ nullptr },
		m_Parent(nullptr),
		m_Pos(min),
		m_Dim(0),
		m_Count(0)
	{
		// determine the amount of space this quad tree has to span
		const Vec2<float> diff = max - min;
		// our implementation (using a minimum dimension) must be able to be divided cleanly until m_Dim = 2. The only way to ensure this is to make the side length of the root node a power of 2.
		m_Dim = nextPower(2, math::max(diff.x, diff.y));
	}
	template<uint THRESHOLD>
	QuadTreeNode<THRESHOLD>::~QuadTreeNode()
	{
		// delete storage for all the Elements this Node contains
		DeleteData();

		// recursively delete all child Nodes
		for (uint i = 0; i < s_Children; i++)
			delete m_Children[i];
	}
	template<uint THRESHOLD>
	void QuadTreeNode<THRESHOLD>::Add(Element* e)
	{
		if (!e->IsContainedBy(this))
		{
			// if this Node is the root and even it doesn't contain the given Element, the Element is outside of the area controlled by this QuadTree
			if (!m_Parent)
			{
				const auto& pos = e->m_Pos, dim = e->m_Dim;
				printf("QuadTree [(%f, %f), (%f, %f)] cannot contain element [(%f, %f), (%f, %f)]\n", m_Pos.x, m_Pos.y, m_Pos.x + m_Dim, m_Pos.y + m_Dim, pos.x, pos.y, pos.x + dim.x, pos.y + dim.y)
			}
			return;
		}

		// if we already have the threshold number of Elements, preemptively divide
		if (m_Count == THRESHOLD)
			Divide();

		// divided Nodes can't store Elements, so push it down to each child Node
		if (IsDivided())
			for (uint i = 0; i < s_Children; i++)
				m_Children[i]->Add(e);
		// non-divided Nodes must store Elements
		else
		{
			// this will become the head of our list of Elements
			ElementNode* node = new ElementNode();
			node->data = e;
			// no nodes to the left
			node->prev = nullptr;

			// if we already have some Elements
			if (m_Data)
			{
				// new node has stuff on the right
				node->next = m_Data;
				// old head now has something on the left
				m_Data->prev = node;
			}
			// make this the new head
			m_Data = node;
			m_Count++;

			// handles adding to parent/grandparent containers
			e->AddTo(this, node);
		}
	}
	template<uint THRESHOLD>
	QuadTreeNode<THRESHOLD>::QuadTreeNode(const math::Vec2<float>& pos, uint size, Node* const parent) :
		m_Data(nullptr),
		m_Children{ nullptr },
		m_Parent(parent),
		m_Pos(pos),
		m_Dim(dim),
		m_Count(0)
	{}
	template<uint THRESHOLD>
	void QuadTreeNode<THRESHOLD>::DeleteElement(ElementNode* element)
	{
		// link left and right elements around this node before deleting
		if (element->prev)
			element->prev->next = element->next;
		if (element->next)
			element->next->prev = element->prev;
		// if this node is the head of our list, update the head
		if (m_Data == element)
			m_Data = element->next;

		// finally actually delete it
		delete element;
		m_Count--;
	}
	template<uint THRESHOLD>
	void QuadTreeNode<THRESHOLD>::DeleteData()
	{
		// delete all contained ElementNodes (not the Elements themselves, just the storage for them within this Node)
		ElementNode* cur = m_Data;
		while (cur)
		{
			ElementNode* next = cur->next;
			delete cur;
			cur = next;
		}
		// important to reset count/pointers
		m_Data = nullptr;
		m_Count = 0;
	}
	template<uint THRESHOLD>
	void QuadTreeNode<THRESHOLD>::Divide()
	{
		// a Node can't be smaller than 2x2 because that means we're inserting intersecting objects
		if (m_Dim <= s_MinDim)
		{
			printf("Cannot divide QuadTreeNode further, intersecting objects must have been inserted\n");
			return;
		}

		// remove this Node from each of its contained Elements' caches
		ElementNode* cur = m_Data;
		while (cur)
		{
			cur->data->RemoveFrom(this);
			cur = cur->next;
		}

		// create child Nodes
		constexpr math::Vec2<float> offsets[s_Children] = { {0.f, 0.f}, {.5f, 0.f}, {.5f, .5f}, {0.f, .5f} };
		for (uint i = 0; i < s_Children; i++)
		{
			Node* child = new Node(m_Pos + offsets[i] * CAST(float, m_Dim), m_Dim / 2, this);
			// attempt to add all of this Node's Elements to each of the new children
			cur = m_Data;
			while (cur)
			{
				cur->data->AddTo(child);
				cur = cur->next;
			}
			m_Children[i] = child;
		}

		// now that this Node is divided, it can only contain children, so delete the ElementNodes
		DeleteData();
	}
	template<uint THRESHOLD>
	void QuadTreeNode<THRESHOLD>::GetUniqueElements(std::unordered_set<Element*>* const elements) const
	{
		if (!IsDivided())
		{
			ElementNode* cur = m_Data;
			while (cur)
			{
				elements->insert(cur->data);
				cur = cur->next;
			}
		}
		else
			for (uint i = 0; i < s_Children; i++)
				m_Children[i]->GetUniqueElements(elements);
	}
	template<uint THRESHOLD>
	void QuadTreeNode<THRESHOLD>::Merge()
	{
		if (!IsDivided())
		{
			printf("Cannot merge a QuadTreeNode that is not divided\n");
			return;
		}

		// get the unique elements that all of this Node's children contain
		std::unordered_set<Element*> elements;
		GetUniqueElements(&elements);

		// if the total number of Elements within this Node's space is within the threshold range, we can just store them all in this Node directly
		if (elements.size() <= THRESHOLD)
		{
			// remove each Element from all of this Node's children
			for (Element* cur : elements)
			{
				cur->m_Grandparents.erase(this);
				auto& parents = element->m_Parents;
				for (uint i = 0; i < s_Children; i++)
					parents.erase(m_Children[i]);
			}

			// delete children
			for (uint i = 0; i < s_Children; i++)
			{
				delete m_Children[i];
				m_Children[i] = nullptr;
			}

			// add each Element to this Node
			for (Element* cur : elements)
				Add(cur);
		}
	}



	/**
	 * QuadTreeElement.h
	 */
	template<uint THRESHOLD>
	const bool QuadTreeElement<THRESHOLD>::NodeComparator::operator()(const Node* const a, const Node* const b) const
	{
		// gives smaller Nodes a higher priority, meaning that they will be merged first
		return a && b && a->m_Dim < b->m_Dim;
	}
	template<uint THRESHOLD>
	QuadTreeElement<THRESHOLD>::QuadTreeElement(const math::Vec2<float>& pos, const math::Vec2<float>& dim, const math::Vec2<float>& vel) :
		m_Pos(pos),
		m_Dim(dim),
		m_Vel(vel)
	{}
	template<uint THRESHOLD>
	void QuadTreeElement<THRESHOLD>::Move(const math::Vec2<float>& pos, const math::Vec2<float>& dim, const math::Vec2<float>& vel, Node* const root)
	{
		// if position of dimensions have changed, we might need to move around in the tree
		if (m_Pos != pos || m_Dim != dim)
		{
			CopyHostValues(pos, dim, vel);

			// copy current grandparents
			std::set<Node*, NodeComparator> originalGrandparents = m_Grandparents;
			// remove ourselves from the tree
			RemoveFromParents();
			m_Grandparents.clear();
			// add back to the tree with our new position/dimensions
			root->Add(this);

			// remove all new grandparents from our list of old grandparents
			for (auto gp : m_Grandparents)
				originalGrandparents.erase(gp);

			// attempt to merge old grandparents
			Merge(originalGrandparents);
		}
		// we haven't moved or changed size, so our position in the tree doesn't need to change
		else
			CopyHostValues(pos, dim, vel);
	}
}
