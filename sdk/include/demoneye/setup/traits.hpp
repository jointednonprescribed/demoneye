
#ifndef __DEMONEYE__setup_TRAITS_H
#define __DEMONEYE__setup_TRAITS_H 1

#include "platform.hpp"



namespace demoneye
{
	template <typename ..._Pack>
	class pack_traits
	{
		template <typename _ToClass>
		static constexpr bool all_convertible =
			(std::is_convertible_v<_Pack, _ToClass> && ...);

		template <typename _ToClass>
		static constexpr bool all_same =
			(std::is_same_v<_Pack, _ToClass> && ...);
	};
}



#endif // __DEMONEYE__setup_TRAITS_H
