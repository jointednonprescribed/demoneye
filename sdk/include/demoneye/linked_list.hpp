
#ifndef __DEMONEYE__LINKED_LIST_H
#define __DEMONEYE__LINKED_LIST_H 1

#include "iter.hpp"



namespace demoneye
{
	template <typename _T>
	class arraylist
	{
		memref<_T> values;
		size_t     len;

	public:

		arraylist():
			values(), len(0)
		{}

		arraylist(size_t n)
			values(n), len(0)
		{}
		arraylist(size_t n, const _T *arr):
			values(n)
		{
			_T* addr = values;

			for (size_t i = 0; i < n; i++)
				addr[i] = arr[i];
			addr = nullptr;

			len = n;
		}
		template <size_t _N>
		arraylist(const _T arr[_N]):
			arraylist(_N, arr)
		{}

		~arraylist()
		{}

		template <typename _U>
		typename std::enable_if_t<
			!std::is_same_v<_U, _T> &&
			std::is_convertible_v<_U, _T>,
		arraylist<_T>&>
		operator =(const arraylist<_U> &rhv)
		{}
		arraylist<_T>& operator =(const arraylist<_T> &rhv)
		{}
	};
}



#endif // __DEMONEYE__LINKED_LIST_H
