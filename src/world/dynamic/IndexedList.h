#pragma once
#include "pch.h"

namespace engine
{
	template<typename T>
	class IndexedList
	{
	public:
		IndexedList(uint count) :
			m_Count(count),
			m_Next(0),
			m_List(new T[count])
		{
			m_Openings.reserve(count);
		}
		IndexedList(const IndexedList& other) = delete;
		IndexedList(IndexedList&& other) = delete;
		virtual ~IndexedList()
		{
			delete[] m_List;
		}


		uint Add(const T& t)
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
		void Remove(uint i)
		{
			if (i >= m_Count)
			{
				printf("Invalid index %u\n", i);
				return;
			}

			m_Openings.push_back(i);
		}
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
	protected:
		uint m_Count, m_Next;
		std::vector<uint> m_Openings;
		T* m_List;


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
	};
}
