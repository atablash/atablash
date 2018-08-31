#pragma once

// enable debug if any debug macro is defined
#if (defined _DEBUG || defined DEBUG) && !defined DEBUG_BUILD
#define DEBUG_BUILD
#endif

// DEBUG_BUILD (set or not set) overrides NDEBUG
#ifdef DEBUG_BUILD
	#undef NDEBUG
	//#pragma message "Compiling in DEBUG mode."
#else
	#ifndef NDEBUG
	#define NDEBUG
	#endif
#endif

//#include <cstdio>
//#include <cfloat>

#include <atablash/common.hpp>
//#include <atablash/log.hpp>


inline void breakpoint(){}
#ifdef NDEBUG
	#define DE if(0)
#else // DEBUG
	#define DE
#endif


#if defined NDEBUG && not defined ASSERTS
	#define ASS(x)
	#define FLOAT_OK(x)
	#define ASSERT_FLOAT(x)
#else
	#include<cassert>
	#define ASS(x) assert(x)
	#define FLOAT_OK(x) ((x) <= LDBL_MAX && (x) >= -LDBL_MAX)
	#define ASSERT_FLOAT(f) ASS(FLOAT_OK(f))
#endif

#define DV(x) (E<#x<": "<(x)<N)
#define XX E<"XX at line "<__LINE__<NL;


#if defined DEBUG_BUILD or defined COLOR_CONSOLE
	#ifdef _WIN32
	
		#ifndef NOMINMAX
		#define NOMINMAX
		#endif
		
		#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
		#endif
		
		#include<windows.h>
		inline void ec(int a){static HANDLE h=GetStdHandle(-12);SetConsoleTextAttribute(h,a?12:0xf);}	// color 12
		inline void oc(int a){static HANDLE h=GetStdHandle(-11);SetConsoleTextAttribute(h,a?10:0xf);}	// color 10
	#else
		// *nix includes for debug
	#endif
#else
	#define ec(x)
	#define oc(x)
#endif



#include <iostream>
struct _E{template<class T>_E&operator<(const T&_t){DE{ec(1);std::cerr<<_t;ec(0);}return*this;}};





#ifdef __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-variable"	// not working
#endif

static _E E;

#ifdef __GNUC__
	#pragma GCC diagnostic pop
#endif




// E unused
#ifdef __GNUC__
	template<class T>void ___fhsdhsdhrtsh(){(void)E;}
#endif


