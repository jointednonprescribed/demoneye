
#ifndef __DEMONEYE__MATH_H
#define __DEMONEYE__MATH_H

#include "iter.hpp"



namespace demoneye
{
	class iMathematical
	{
	public:
		iMathematical() = delete;
		iMathematical(const iMathematical&) = delete;
		~iMathematical() = delete;
	};

	/* A generic morphism type */
	class _gmorphism
	{
	public:
		_gmorphism() = delete;
		_gmorphism(const _gmorphism&) = delete;
		~_gmorphism() = delete;
	};

	/* A type trait checker for mathematical objects. */
	template <typename _T>
	struct is_mathematical
	{
		static constexpr bool value =
			std::is_integral<_T>::value ||
			std::is_floating_point<_T>::value ||
			std::is_base_of<iMathematical, _T>::value ||
			std::is_base_of<_gmorphism, _T>::value;

		is_mathematical() = delete;
		is_mathematical(const _gmorphism&) = delete;
		~is_mathematical() = delete;
	};
	template <typename _T>
	constexpr bool is_mathematical_v = is_mathematical<_T>::value;

	/* Mathematical Function (Morphism) type */
	template <typename _T>
	class morphism
	{
		static_assert(is_mathematical<_T>::value,
			"Cannot create a morphism (function) of a non-mathematical object, use a lambda instead.");
	};

	/* Mathematical Vector type */
	template <size_t _Dimc, typename _T=double>
	class vector: public iIterable<_T>
	{
		static_assert(is_mathematical<_T>::value,
			"Cannot create a vector of a non-mathematical object, use a tuple instead.");
		static_assert(_Dimc > 0, "Cannot create a vector with 0 dimensions.");

		memref<_T> values;
		bool       anglesum; // This variable flags that the values are interpreted
		//                      in the format: vector-length, angle1, angle2, etc.,
		//                      instead of dimensional coordinates of the vector's
		//                      destination.

	public:

		vector():
			values(), anglesum(false)
		{}

		vector<_T> Copy(const vector<_T> &rhv);
		vector(const vector &rhv)
		{
			Copy(rhv);
		}

		~vector()
		{}

		vector<_T> Copy(const vector<_T> &rhv)
		{
			if (!rhv.values.IsNull())
				values.Copy(rhv.values);
			anglesum = rhv.anglesum;
		}
		vector<_T> Borrow(const vector<_T> &rhv)
		{
			values    = rhv.values;
			anglesume = rhv.anglesum;
		}

		vector<_T>& operator =(const vector<_T> &rhv)
		{
			Copy(rhv);
		}
	};

	template <typename _T=double>
	using vec2 = vector<2, _T>;
	template <typename _T=double>
	using vec3 = vector<3, _T>;
	template <typename _T=double>
	using vec4 = vector<4, _T>;

	template <size_t _Sz, typename _T=double>
	class matrix
	{
		static_assert(is_mathematical<_T>::value,
			"Cannot create a vector of a non-mathematical object, use a table instead.");
		static_assert(_Dimc > 0, "Cannot create a matrix with size 0.");

		memref<_T> values;

	public:

		static constexpr size_t RootSize()
		{
			return _Sz;
		}
		static constexpr size_t Size()
		{
			return _Sz * _Sz;
		}

		matrix():
			values()
		{
			try {
				values.CreateNewBlock(Size());
			} catch (exception e) {
				return;
			} catch (std::exception e) {
				logerr(e);
				return;
			}
		}
		matrix(const matrix<_Sz, _T> &rhv):
			values()
		{
			if (!rhv.values.IsNull())
				data.Copy(rhv.values);
		}

		~matrix()
		{}
	};
}



#endif // __DEMONEYE__MATH_H
