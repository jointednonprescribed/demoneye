
#ifndef __DEMONEYE_setup_INCLUDES_H
#define __DEMONEYE_setup_INCLUDES_H 1

#ifndef __DEMONEYE_setup_PLATFORM_H
#	include "platform.hpp"
#endif



/* C Standard Library Dependencies */
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstring>
#include <ctime>
#include <csetjmp>
#include <cassert>
#include <cerrno>
// POSSIBLE HEADERS TO INCLUDE LATER ON:
//#include <cctype>
//#include <cuchar>
//#include <cwchar>
//#include <cwctype>
//#include <clocale>
/* C++ Standard Library Dependencies */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <bit>
#include <numeric>
#include <chrono>
#include <memory>
#include <string>
#include <array>
#include <tuple>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <format>
#include <functional>
// POSSIBLE HEADERS TO INCLUDE LATER ON:
//#include <system_error>
//#include <ratio>
//#include <locale>
//#include <text_encoding>
//#include <regex>


/* System Dependencies */
#if DE_PLATFORM == DE_WINDOWS
#	include <windows.h>
#	include <winnt.h>
#	include <processthreadsapi.h>
#	include <heapapi.h>
#elif DE_PLATFORM_UNIXLIKE == true
#	include <unistd.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#	include <signal.h>
#endif
#if DE_PLATFORM == DE_OSX
#elif DE_PLATFORM == DE_LINUX
#endif

/* API Dependencies */

// SDL 3.2
#include <SDL3/SDL.h>

// GLFW 3.3 & GLAD
// If not a number or not defined, default OpenGL version is 4.5
#if !defined(DE_OPENGL_VERSION_MAJOR) || !(DE_OPENGL_VERSION_MAJOR > 0 || DE_OPENGL_VERSION_MAJOR <= 0)
#   define DE_OPENGL_VERSION_MAJOR 4
#endif
#if !defined(DE_OPENGL_VERSION_MINOR) || !(DE_OPENGL_VERSION_MINOR > 0 || DE_OPENGL_VERSION_MINOR <= 0)
#   define DE_OPENGL_VERSION_MINOR 5
#endif
#define  DE_OPENGL_VERSION DE_OPENGL_VERSION_MAJOR##DE_OPENGL_VERSION_MINOR
#include <glad/glad.h>
#include <GLFW/glfw3.h>



#endif // __DEMONEYE_setup_INCLUDES_H
