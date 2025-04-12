#pragma once

#define PLATFORM_WIN32   1                  //Windows Platform
#define PLATFORM_LINUX   2                  //Linux Platform
#define PLATFORM_OSX     3

#define COMPILER_MSVC    1                  //Visual Studio Compiler
#define COMPILER_GNUC    2                  //GCC Compiler
#define COMPILER_INTEL   3                  //Intel Compiler
#define COMPILER_CLANG   4                  //Clang Compiler


/************************************************************************/
/**
 * Finds the current platform
 */
 /************************************************************************/
#if defined (__WIN32__) || defined (_WIN32)
# define PLATFORM PLATFORM_WIN32
#elif defined (__APPLE_CC__ )
# define PLATFORM PLATFORM_OSX
#elif defined (__ORBIS__)
# define PLATFORM PLATFORM_LINUX
#endif


/************************************************************************/
/**
 * Compiler type and version
 */
 /************************************************************************/

#if defined(__clang__)
#   define COMPILER COMPILER_CLANG
#   define COMP_VER __cland_version__
#elif defined (__GNUC__) //Check after Cland, as Clang defines this too
#   define COMPILER COMPILER_GNUC
#   define COMP_VER (((__GNUC__)*100)+(__GNUC_MINOR__*10)+__GNUC_PATCHLEVEL__)
#elif defined (__INTEL_COMPILER)
#   define COMPILER COMPILER_INTEL
#   define COMP_VER __INTEL_COMPILER
#elif defined (_MSC_VER)
#   define COMPILER COMPILER_MSVC
#   define COMP_VER _MSC_VER
#   undef __PRETTY_FUNCTION__
#   define __PRETTY_FUNCTION__ __FUNCSIG__
#else
//No know compiler found, send the error to the output (if any)
#   pragma error "No known compiler. "
#endif


#if COMPILER == COMPILER_MSVC
# if COMP_VER >= 1200
#   define FORCEINLINE __forceinline
# endif // COMP_VER >= 1200
#elif defined (__MINGW32__)
# if !defined (FORCEINLINE)
#   define FORCEINLINE __inline
# endif // !defined (FORCEINLINE)
#endif // COMPILER

#ifndef FORCEINLINE
# define FORCEINLINE __inline
#endif