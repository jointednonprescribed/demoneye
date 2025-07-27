
#ifndef __DEMONEYE__setup_PLATFORM_H
#define __DEMONEYE__setup_PLATFORM_H 1

#include "config.hpp"



#define DE_UNKNOWN 0
/* Platform Specifiers */
#define DE_WINDOWS 1
#define DE_LINUX   3
#define DE_OSX     2
/* Compiler Specifiers */
#define DE_GCC     4
#define DE_MSVC    6
#define DE_CLANG   5
/* Architecture Specifiers */
#define DE_X86     7
#define DE_x86     DE_X86
#define DE_ARM     8


/* Platform Detection */
#ifdef _WIN32
#	define DE_PLATFORM          DE_WINDOWS
#	define DE_PLATFORM_UNIXLIKE false
#	ifdef _WIN64
#		define DE_PLATFORM32 false
#	else
#		define DE_PLATFORM32 true
#	endif
#elif defined(__linux__) || defined(__linux) || defined(__gnu_linux__)
#	define DE_PLATFORM          DE_LINUX
#	define DE_PLATFORM_UNIXLIKE true
#	include <unistd.h>
#	if __WORDSIZE == 64
#		define DE_PLATFORM32 false
#	else
#		define DE_PLATFORM32 true
#	endif
#elif defined(__MACH__) && defined(__APPLE__)
#	define DE_PLATFORM          DE_OSX
#	define DE_PLATFORM_UNIXLIKE true
#	include <unistd.h>
#	if __WORDSIZE == 64
#		define DE_PLATFORM32 false
#	else
#		define DE_PLATFORM32 true
#	endif
#else
#	error "Demoneye SDK only supports Windows10+, MacOSX, and Linux."
#endif

/* Compiler/Architecture Detection */
#ifdef _MSC_VER
#	define DE_COMPILER DE_MSVC
#	if defined(_M_ARM) || defined(_M_ARM64)
#		define DE_ARCH    DE_ARM
#	elif defined(_M_ARM64EC)
#		define DE_ARCH    DE_ARM
#		define DE_ARM64EC true
#	elif defined(_M_AMD64) || defined(_M_IX86)
#		define DE_ARCH    DE_X86
#	else
#		error "Demoneye SDK only supports ARM32/64 and x86/x64 architectures."
#	endif
#elif defined(__GNUC__)
#	define DE_COMPILER DE_GCC
#	if defined(__arm__) || defined(__aarch64__)
#		define DE_ARCH    DE_ARM
#	elif defined(__x86_64__) || defined(__amd64__) || defined(__i386__)
#		define DE_ARCH    DE_X86
#	else
#		error "Demoneye SDK only supports ARM32/64 and x86/x64 architectures."
#	endif
#elif defined(__clang__)
#	define DE_COMPILER DE_CLANG
#	if defined(__arm__) || defined(__aarch64__)
#		define DE_ARCH    DE_ARM
#	elif defined(__x86_64__) || defined(__amd64__) || defined(__i386__)
#		define DE_ARCH    DE_X86
#	else
#		error "Demoneye SDK only supports ARM32/64 and x86/x64 architectures."
#	endif
#endif


namespace demoneye
{
	constexpr int  UNKNOWN    = DE_UNKNOWN;
	constexpr int  WINDOWS    = DE_WINDOWS;
	constexpr int  LINUX      = DE_LINUX;
	constexpr int  OSX        = DE_OSX;
	constexpr int  GCC        = DE_GCC;
	constexpr int  MSVC       = DE_MSVC;
	constexpr int  CLANG      = DE_CLANG;
	constexpr int  X86        = DE_X86;
	constexpr int  x86        = DE_x86;
	constexpr int  ARM        = DE_ARM;

	constexpr int  PLATFORM   = DE_PLATFORM;
	constexpr int  ARCH       = DE_ARCH;
	constexpr int  COMPILER   = DE_COMPILER;
	constexpr bool PLATFORM32 = DE_PLATFORM32;

	const char *const PLATFORM_NAME =
	#if DE_PLATFORM == DE_WINDOWS
		"Windows";
	#elif DE_PLATFORM == DE_LINUX
		"Linux";
	#elif DE_PLATFORM == DE_OSX
		"MacOSX";
	#endif
	const char *const ARCH_NAME =
	#if DE_ARCH == DE_X86
	#	if DE_PLATFORM32 == false
		"x86";
	#	else
		"AMD64";
	#	endif
	#elif DE_ARCH == DE_ARM
	#	if DE_PLATFORM32 == false
		"ARM";
	#	elif defined(DE_ARM64EC) && DE_ARM64EC == true
		"ARM64EC";
	#	else
		"ARM64";
	#	endif
	#endif
	const char *const COMPILER_NAME =
	#if DE_COMPILER == DE_MSVC
		"Microsoft Visual C++";
	#elif DE_COMPILER == DE_GCC
		"GNU G++";
	#elif DE_COMPILER == DE_CLANG
		"CLang++";
	#endif

	class EngineContext;
	EngineContext& GetEngineEnv();
	const EngineContext& GetEngineEnvConst();
}


/* Required Included Headers and Dependencies */
#ifndef __DEMONEYE_setup_INCLUDEs_H
#	include "includes.hpp"
#endif



#endif // __DEMONEYE__setup_PLATFORM_H
