#pragma once

namespace math
{
	template<typename T>
	struct LinkedListNode
	{
		T* data = nullptr;
		LinkedListNode<T>* prev = nullptr, * next = nullptr;
	};
}
