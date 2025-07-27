
#ifndef __DEMONEYE__setup_CONFIG_H
#define __DEMONEYE__setup_CONFIG_H 1



#if !defined(__cplusplus) || __cplusplus <= 201199L
#	error "Demoneye SDK requires C++14 or later."
#endif


#if DE_MEMREF_CACHE_ADDR > 1
#   define DE_MEMREF_CACHE_ADDR true
#elif DE_MEMREF_CACHE_ADDR < 0
#   define DE_MEMREF_CACHE_ADDR false
#else
#   define DE_MEMREF_CACHE_ADDR false 
#endif



#endif // __DEMONEYE__setup_CONFIG_H
