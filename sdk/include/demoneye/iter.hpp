
#ifndef __DEMONEYE__ITER_H
#define __DEMONEYE__ITER_H 1

#include "memory.hpp"



namespace demoneye
{
	namespace modules
	{
		namespace iter
		{
			template <typename _I>
			struct absolute_value
			{
				static_assert(std::is_integral_v<_I> || std::is_floating_point_v<_I>,
					"Cannot create absolute value of a non-integral, non-floating-point type.");

				using I_signable_t       = typename std::enable_if_t<std::is_integral_v<_I>, _I>;
				using I_floating_point_t = typename std::enable_if_t<std::is_floating_point_v<_I>, _I>;
				using unsigned_t         = std::make_unsigned_t<I_signable_t>;
				using signed_t           = std::make_signed_t<I_signable_t>;

				const _I         ivalue;
				const unsigned_t uvalue;

				absolute_value() = delete;

				absolute_value(signed_t v):
					uvalue((v < 0)? v - v - v : v), ivalue(0)
				{}

				absolute_value(I_floating_point_t v):
					ivalue((v < static_cast<_I>(0.0))? v - v - v : v), uvalue(static_cast<_I>(0.0))
				{}

				operator unsigned_t() const
				{
					return uvalue;
				}
				template <typename = std::enable_if_t<std::is_unsigned_v<I_signable_t>>>
				operator I_signable_t() const
				{
					return uvalue;
				}
				operator I_floating_point_t() const
				{
					return ivalue;
				}
			};

			constexpr size_t absolute_index(ssize_t rel_index, size_t subject_length)
			{
				const size_t abs = absolute_value<size_t>(rel_index);

				if (abs <= subject_length) {
					return (rel_index < 0)? (subject_length-abs) : size_t(rel_index);
				} else {
					return (rel_index < 0)? 0 : (subject_length-1);
				}
			}
			constexpr ssize_t relative_index(size_t abs_index, size_t subject_length)
			{
				abs_index -= ((abs_index-subject_length)+1) * (abs_index >= subject_length);

				// The halfway point of the iterable, if even, = floor(L/2) - 1, if odd, = floor(L/2)
				const size_t halfway = (subject_length / 2) - (!(subject_length%2));

				return (abs_index <= halfway)? ssize_t(abs_index) : -(subject_length-abs_index);
			}
		}
	}

	template <typename _ItTyp>
	class iIterable: public iTerminatable
	{
	protected:
		iIterable()
		{}
	public:
		static constexpr bool type_is_const    = std::is_const_v<_ItTyp>;
		static constexpr bool type_is_volatile = std::is_volatile_v<_ItTyp>;

		iIterable(const iIterable<_ItTyp>&) = delete;
		~iIterable()
		{}

		virtual const std::type_info& GetTypeInfo() const noexcept =0;

		virtual _ItTyp* GetOrigin() noexcept =0;

		virtual bool    HasLimit() const noexcept
		{
			return false;
		}
		virtual size_t  Limit() const noexcept
		{
			return 0;
		}
	};

	template <typename _ItTyp>
	class iterator
	{
	public:
		static constexpr bool is_const = std::is_const_v<_ItTyp>;

		typedef function<bool, iterator<_ItTyp>&> step_function_t;

		_ItTyp               *cursor;
		size_t                index;
	private:
		size_t                max, origin;
		iIterable<_ItTyp>    *iterable;
		step_function_t       stepfunc;
		lambda                closer;
		const std::type_info& typeinfo;
		bool                  last_result, use_max;
	public:

		iterator(const iterator<_ItTyp> &rhv) = delete;

		iterator()
		{
			index       = 0;
			cursor      = nullptr;
			iterable    = nullptr;
			max         = 0;
			origin      = 0;
			stepfunc    = nullptr;
			closer      = nullptr;
			last_result = false;
			use_max     = false;
		}
		iterator(_ItTyp *set, step_function_t step, lambda closer_func=nullptr)
		{
			index       = start_at_index;
			cursor      = set;
			iterable    = nullptr;
			max         = 0;
			origin      = 0;
			stepfunc    = step;
			closer      = closer_func;
			last_result = true;
			use_max     = false;
		}
		iterator(_ItTyp *set, size_t max_iterations, step_function_t step, lambda closer_func=nullptr)
		{
			index       = start_at_index;
			cursor      = set;
			iterable    = nullptr;
			max         = max_iterations;
			origin      = 0;
			stepfunc    = step;
			closer      = closer_func;
			last_result = true;
			use_max     = true;
		}
		iterator(const iIterable<_ItTyp> &set, step_function_t step, size_t start_at_index=0, lambda closer_func=nullptr)
		{
			index       = start_at_index;
			cursor      = set.GetOrigin();
			iterable    = set;
			max         = 0;
			origin      = start_at_index;
			stepfunc    = step;
			closer      = closer_func;
			last_result = true;
			use_max     = false;
		}
		iterator(const iIterable<_ItTyp> &set, size_t max_iterations, step_function_t step, size_t start_at_index=0, lambda closer_func=nullptr)
		{
			index       = start_at_index;
			cursor      = set.GetOrigin();
			iterable    = set;
			max         = max_iterations;
			origin      = start_at_index;
			stepfunc    = step;
			closer      = closer_func;
			last_result = true;
			use_max     = true;
		}
		iterator(const iterator<_ItTyp> &rhv, step_function_t new_step_function, size_t start_at_index=0, lambda closer_func=nullptr):
			iterator(rhv.iterable, new_step_function, start_at_index, closer_func)
		{}
		iterator(const iterator<_ItTyp> &rhv, size_t max_iterations, step_function_t new_step_function, size_t start_at_index=0, lambda closer_func=nullptr):
			iterator(rhv.iterable, max_iterations, new_step_function, start_at_index, closer_func)
		{}

		size_t Index() const noexcept
		{
			return index;
		}
		_ItTyp* Cursor() const noexcept
		{
			return cursor;
		}
		size_t Origin() const noexcept
		{
			return origin;
		}
		iIterable<_ItTyp>* Iterable() noexcept
		{
			return iterable;
		}
		const iIterable<_ItTyp>* Iterable() const noexcept
		{
			return iterable;
		}
		const step_function_t& StepFunction() const noexcept
		{
			return stepfunc;
		}
		const lambda& ClosingFunction() const noexcept
		{
			return closer;
		}
		const std::type_info& TypeInfo() const noexcept
		{
			return typeinfo;
		}
		bool Continuing() const noexcept
		{
			return last_result;
		}
		bool Stopped() const noexcept
		{
			return !last_result;
		}
		bool AtMaxIterations() const noexcept
		{
			return !last_result || (use_max && index == max_iterations);
		}
		bool HasIterationLimit() const noexcept
		{
			return use_max;
		}
		size_t GetMaxIterations() const noexcept
		{
			return max * use_max;
		}

		void SetCursor(_ItType *new_cursor)
		{
			if (new_cursor != nullptr)
				cursor = new_cursor;
		}
		void Index(size_t new_index)
		{
			index = new_index;
		}

		bool Step()
		{
			if (stepfunc != nullptr) {
				last_result = stepfunc(*this);
				if (!last_result)
					closer();
				return last_result;
			} else {
				last_result = false;
				return false;
			}
		}
		void ReturnToOrigin()
		{
			cursor = iterable.GetOrigin();
			index  = origin;
		}
	};
	template <typename _ItTyp>
	using iter = iterator<_ItTyp>;
}



#endif // __DEMONEYE__ITER_H
