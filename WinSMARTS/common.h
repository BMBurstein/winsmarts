#pragma once
#ifndef COMMON_H
#define COMMON_H

#if _M_X64

#if !_M_AMD64
# define _M_AMD64 _M_X64
#endif

#ifndef _WIN64
#  define _WIN64
#endif

#ifndef _AMD64
#  define _AMD64
#endif

#undef _M_IX86

#endif

#ifdef _AMD64
#  define STACK_ALIGN 16
#else
#  define STACK_ALIGN 4
#endif

#endif // COMMON_H