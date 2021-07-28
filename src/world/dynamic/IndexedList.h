#pragma once
#include "pch.h"

namespace engine
{
	template<typename T, typename RETURN = uint, typename ADD = const T&, typename REMOVE = const RETURN&>
	class IndexedList
	{
	public:
		typedef RETURN Handle;


		IndexedList(uint count, const T& placeholder) :
			m_Count(count),
			m_Next(0),
			m_List(new T[count]),
			m_Placeholder(placeholder)
		{
			for (uint i = 0; i < m_Count; i++)
				m_List[i] = placeholder;

			m_Openings.reserve(count);
		}
		IndexedList(const IndexedList& other) = delete;
		IndexedList(IndexedList&& other) = delete;
		virtual ~IndexedList()
		{
			delete[] m_List;
		}


		virtual RETURN Add(ADD t) = 0;
		virtual void Remove(REMOVE t) = 0;
		const T& operator[](uint i)
		{
			return m_List[i];
		}
		uint GetMaxSize() const
		{
			return m_Count;
		}
		uint GetSize() const
		{
			return m_Next - m_Openings.size();
		}
		uint GetLast() const
		{
			return m_Next;
		}
		bool IsEmpty() const
		{
			return m_Openings.size() == m_Next;
		}
		bool IsFull() const
		{
			return m_Next == m_Count && m_Openings.empty();
		}
		template<typename T>
		void ForEach(T fn) const
		{
			for (uint i = 0; i < GetLast(); i++)
				if (IsValid(i))
					fn(m_List[i]);
		}
		template<typename T>
		void ForEach(T fn)
		{
			for (uint i = 0; i < GetLast(); i++)
				if (IsValid(i))
					fn(m_List[i]);
		}
	protected:
		uint m_Count, m_Next;
		std::vector<uint> m_Openings;
		T* m_List, m_Placeholder;


		uint AddBase(ADD t)
		{
			if (IsFull())
			{
				printf("IndexedList is full\n");
				return m_Count;
			}

			const uint i = NextIndex();
			m_List[i] = t;
			m_Next += (i == m_Next);
			return i;
		}
		void RemoveBase(uint i)
		{
			if (i >= m_Count)
			{
				printf("Invalid index %u\n", i);
				return;
			}

			if (m_List[i] != m_Placeholder)
			{
				m_List[i] = m_Placeholder;
				m_Openings.push_back(i);
			}
		}
		uint NextIndex()
		{
			uint i = m_Next;
			if (!m_Openings.empty())
			{
				i = m_Openings.back();
				m_Openings.pop_back();
			}
			return i;
		}
		bool IsValid(uint i) const
		{
			return i < m_Count && m_List[i] != m_Placeholder;
		}
	};
}
