#pragma once


// MSVC
#if defined(_MSC_VER)
#	define EXPORT __declspec(dllexport)
#	define IMPORT __declspec(dllimport)
// GCC
#elif defined(__GNUC__)
#	define EXPORT __attribute__((visibility("default")))
#	define IMPORT
// Other
#else
#	pragma error This compiler is not supported.
#endif


#ifdef TANK_DLL
#	define TANK_API EXPORT
#else
#	define TANK_API IMPORT
#endif
