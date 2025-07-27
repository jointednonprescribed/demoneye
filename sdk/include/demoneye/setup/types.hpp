
#ifndef __DEMONEYE__setup_TYPES_H
#define __DEMONEYE__setup_TYPES_H

#include "traits.hpp"
#include "platform.hpp"



namespace demoneye
{
	/* Process ID type */
#if DE_PLATFORM == DE_WINDOWS
	typedef DWORD pid_t;
	constexpr pid_t NULL_PID = 0;
#elif DE_PLATFORM_UNIXLIKE == true
	typedef ::pid_t pid_t;
	constexpr pid_t NULL_PID = 0;
#endif

	/* Group ID type */
	typedef int32_t gid_t;
	constexpr gid_t NULL_GID    = -1;


	/* Terminatable/Closable interface */
	typedef class iTerminatable
	{
	protected:
		iTerminatable();
	public:
		iTerminatable(const iTerminatable&) = delete;
		~iTerminatable();

		virtual int Terminate() noexcept =0;
	} iClosable;

	template <typename _R, typename ..._A>
	class function
	{
	public:
		typedef _R (*address_t)(_A...);
		typedef std::function<_R(_A...)> lambdaexpr_t;

	protected:
		_R (*addr)(_A...);

	public:

		function(address_t funcaddr):
			addr(funcaddr)
		{}
		function():
			addr(NULL)
		{}


		function<_R, ..._A> static FromLambda(const std::function<_R(_A...)> &lambdaexpr)
		{
			return function<_R, ..._A>(static_cast<address_t>(lambdaexpr));
		}

		static constexpr size_t argstack_length      = sizeof...(_A);
		static constexpr bool   argstack_is_empty    = argstack_length == 0;

		static constexpr bool   returns_integral     = std::is_integral<_R>::value;
		static constexpr bool   returns_fp           = std::is_floating_point<_R>::value;
		static constexpr bool   returns_void         = std::is_void<_R>::value;
		static constexpr bool   returns_null_pointer = std::is_null_pointer<_R>::value;
		static constexpr bool   returns_pointer      = std::is_pointer<_R>::value;
		static constexpr bool   returns_function     = std::is_function<_R>::value;
		static constexpr bool   returns_array        = std::is_array<_R>::value;
		static constexpr bool   returns_enum         = std::is_enum<_R>::value;
		static constexpr bool   returns_union        = std::is_union<_R>::value;
		static constexpr bool   returns_class        = std::is_class<_R>::value;
		static constexpr bool   returns_fundamental  = std::is_fundamental<_R>::value;
		static constexpr bool   returns_arithmetic   = std::is_arithmetic<_R>::value;
		static constexpr bool   returns_scalar       = std::is_scalar<_R>::value;
		static constexpr bool   returns_object       = std::is_object<_R>::value;
		static constexpr bool   returns_compound     = std::is_compound<_R>::value;
		static constexpr bool   returns_const        = std::is_const<_R>::value;
		static constexpr bool   returns_volatile     = std::is_volatile<_R>::value;
		static constexpr bool   returns_pod          = std::is_pod<_R>::value;
		static constexpr bool   returns_signed       = std::is_signed<_R>::value;
		static constexpr bool   returns_unsigned     = std::is_unsigned<_R>::value;

		_R operator()(_A... a) const
		{
			if (addr != nullptr)
				return addr(a...);
			else {
				_R nullobj;
				char *pnullobj = reinterpret_cast<char*>(&nullobj);
				for (size_t i = 0; i < sizeof(_R); i++)
					pnullobj[i] = 0;
				logwarn(
					"Attempted to call a null function, attempting to synthesize a null return with dynamic type,\n"
					"do not use the result, as it is not necessarily type safe, but is zero-initialized."
				);
				return nullobj;
			}
		}

		operator void*() const
		{
			return static_cast<void*>(addr);
		}
		operator address_t() const
		{
			return addr;
		}
		operator lambdaexpr_t() const
		{
			return lambdaexpr_t(addr);
		}

		address_t Address() const
		{
			return addr
		}

		function& operator =(address_t rhv)
		{
			addr = rhv;
		}
		function& operator =(const function<_R, _A...> &rhv)
		{
			addr = rhv.addr;
		}

		bool operator ==(void *rhv) const
		{
			return static_cast<void*>(addr) == rhv;
		}
		bool operator ==(std::nullptr_t rhv) const
		{
			return addr == rhv;
		}
		bool operator ==(address_t rhv) const
		{
			return addr == rhv;
		}
		bool operator ==(const function<_R, _A...> &rhv) const
		{
			return addr == rhv.addr;
		}
		bool operator !=(void *rhv) const
		{
			return static_cast<void*>(addr) != rhv;
		}
		bool operator !=(std::nullptr_t rhv) const
		{
			return addr != rhv;
		}
		bool operator !=(address_t rhv) const
		{
			return addr != rhv;
		}
		bool operator !=(const function<_R, _A...> &rhv) const
		{
			return addr != rhv.addr;
		}
	};

	template <typename ..._A>
	using consumer  = function<void, _A...>;

	template <typename _R>
	using supplier  = function<_R>;

	template <typename ..._A>
	using validator = function<bool, _A...>;

	using predicate = function<bool>;

	using lambda    = function<void>;

	template <typename _T1, typename _T2>
	struct pair
	{
		_T1 former;
		_T2 latter;

		pair() = delete;
		pair(const pair<_T1, _T2> &rhv)
		{
			former = rhv.former;
			latter = rhv.latter;
		}
		pair(const pair<_T2, _T1> &rhv)
		{
			former = rhv.latter;
			latter = rhv.former;
		}
		pair(_T1 a1, _T2 a2)
		{
			former = a1;
			latter = a2;
		}
	};
}



#endif // __DEMONEYE__setup_TYPES_H
