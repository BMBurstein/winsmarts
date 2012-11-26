#pragma message("Compiling precompiled headers.\n")

#if _M_X64

#if !_M_AMD64
# define _M_AMD64 _M_X64
#endif

#ifndef _WIN64
#	define _WIN64
#endif

#ifndef _AMD64
#	define _AMD64
#endif

#undef _M_IX86

#endif

#ifdef _WIN32
#	include <Windows.h>
#endif

#include <iostream>
#include <stdint.h>
