
#ifndef __DEMONEYE__SETUP_H
#define __DEMONEYE__SETUP_H 1



namespace demoneye
{
	constexpr unsigned long VERSION = 20251200UL;
}

/* Header Inclusion Tree:
.
.  setup/
.    config.hpp
.     |
.    platform.hpp -- includes.hpp
.     |               |
.     +---------------+
.     |
.    traits.hpp
.     |
.    types.hpp
.     |
.    errno.hpp
.*/
#include "setup/errno.hpp"



#endif // __DEMONEYE__SETUP_H
