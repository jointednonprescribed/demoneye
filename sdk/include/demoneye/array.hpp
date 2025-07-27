
#ifndef __DEMONEYE__ARRAY_H
#define __DEMONEYE__ARRAY_H 1

#include "iter.hpp"



namespace demoneye
{
	template <typename _T, size_t _Dim=2>
	class array: public iIterable<_T>
	{
		static_assert(_Dim > 0, "Cannot create an array with a dimensionality of 0.");

		memref<_T>      values; // values are stored in a particular format so that
		//                         structural data and active data exist in the same
		//                         memory.
		memview<size_t> magnitudes;

	public:

		array():
			values()
		{}

		/* Specify individual sizes for all dimensions, if less arguments are
		.  given than there are dimensions in the array, subsequent dimensions
		.  will always take the previously specified length:
			array<_T, 4> example(
				3, 8 // Fewer specified size arguments than the array's number
				//      of dimensions
				// Size arguments should aways be given as size_t, or else data
				// will not be extracted from the stack properly
			);
		.  This example will instantiate with dimension sizes: 3, 8, 8, and 8,
		.  since only two dimension size arguments are given, yet the array is
		.  4-dimensional, and the last size argument given was 8. */
		template <typename ..._Dsz, typename = std::enable_if_t<
			(pack_traits<_Dsz...>::all_convertible<size_t>) &&
			(sizeof...(_Dsz)) > 0 &&
			(sizeof...(_Dsz)) <= _Dim>>
		array(_Dsz ...dimsizes):
			values(), magnitudes()
		{
			const size_t sizes[_Dim] = {static_cast<size_t>(std::forward<_Dsz>)...};

			size_t packsize = sizeof...(_Dsz), last = sizes[packsize-1];
			
			// find cross product of sizes for full memory size
			size_t full_magnitude = last;
	
			if (packsize < _Dim)
			for (size_t i = packsize; i < _Dim; i++)
				sizes[i] = last;
				full_magnitude *= last;

			// full_magnitude is the product of the last specified dimension size,
			// and any/all elements filled in with that last values for unspecified
			// dimension sizes, so the first specified size up to the second-to-last
			// are the only ones left to multiply.
			packsize--; // negate the last element since it's already been factored
			//             in
			// if there's more than one specified pack element, this loop will go
			// through them all to factor them in.
			for (size_t i = 0; i < packsize; i++)
				full_magnitude *= sizes[i];

			size_t full_memsize = (full_magnitude * sizeof(_T)) + (sizeof(size_t) * (_Dim + 1));

			memref<char> blk;
			blk.CreateNewArray(full_memsize);

			blk.ReinterpretType<_T>(&this->values);
		}
	};

	template <typename _T>
	class tuple: public array<_T, 1>
	{
	public:
		
		tuple():
			array<_T, 1>()
		{}
		tuple(size_t n, const _T *data):
			array<_T, 1>()
		{
			//SetRow(, n, data);
		}

		~tuple():
		{
			array<_T, 1>::~array<_T, 1>();
		}
	};
}



#endif // __DEMONEYE__ARRAY_H
