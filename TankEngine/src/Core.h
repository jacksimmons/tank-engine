#pragma once


// MSVC
#if defined(_MSC_VER)
#	define TANK_EXPORT __declspec(dllexport)
#	define TANK_IMPORT __declspec(dllimport)
// GCC
#elif defined(__GNUC__)
#	define TANK_EXPORT __attribute__((visibility("default")))
#	define TANK_IMPORT
// Other
#else
#	pragma error This compiler is not supported.
#endif


#ifdef TANK_DLL
#	define TANK_API TANK_EXPORT
#else
#	define TANK_API TANK_IMPORT
#endif
