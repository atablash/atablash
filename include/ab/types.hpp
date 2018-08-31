#pragma once


#include<stdint.h>

#include<utility>

#include<vector>
#include<memory>




// abbrevs

typedef unsigned int uint;

typedef int8_t i8;
typedef uint8_t ui8;
typedef int16_t i16;
typedef uint16_t ui16;
typedef int32_t i32;
typedef uint32_t ui32;
typedef int64_t i64;
typedef uint64_t ui64;

// old:
typedef i64 ll;
typedef ui64 ull;

typedef std::pair<i32,i32> pii;
typedef std::pair<double,double> pdd;

typedef std::vector<i32> vi;
#define VE std::vector

#if defined __GNUC__ && __cplusplus < 201103L
#define umap __gnu_cxx::unordered_map
#define umultimap __gnu_cxx::unordered_multimap
#define uset __gnu_cxx::unordered_set
#define umultiset __gnu_cxx::unordered_multiset
#else
#define umap std::unordered_map
#define umultimap std::unordered_multimap
#define uset std::unordered_set
#define umultiset std::unordered_multiset
#endif


typedef unsigned short ushort;
typedef unsigned char uchar;


#if __cplusplus >= 201103L
template<class T> using sptr = std::shared_ptr<T>;
template<class T> using uptr = std::unique_ptr<T>;
#else
#define sptr std::shared_ptr
#define uptr std::unique_ptr
#endif



namespace ab{

struct Void {};


// convert to float with enough precision
template<typename I>struct FloatFrom{typedef Void R;};
template<>struct FloatFrom<i32>{typedef double R;};
template<>struct FloatFrom<ui32>{typedef double R;};
template<>struct FloatFrom<i64>{typedef long double R;};
template<>struct FloatFrom<ui64>{typedef long double R;};
template<>struct FloatFrom<i16>{typedef float R;};
template<>struct FloatFrom<ui16>{typedef float R;};
template<>struct FloatFrom<i8>{typedef float R;};
template<>struct FloatFrom<ui8>{typedef float R;};
template<>struct FloatFrom<float>{typedef float R;};
template<>struct FloatFrom<double>{typedef double R;};
template<>struct FloatFrom<long double>{typedef long double R;};

// get type capable of storing multiplication result
template<typename I>struct Double{typedef Void R;};
template<>struct Double<i8>{typedef i16 R;};
template<>struct Double<ui8>{typedef ui16 R;};
template<>struct Double<i16>{typedef i32 R;};
template<>struct Double<ui16>{typedef ui32 R;};
template<>struct Double<i32>{typedef i64 R;};
template<>struct Double<ui32>{typedef ui64 R;};

template<>struct Double<float>{typedef float R;};
template<>struct Double<double>{typedef double R;};
template<>struct Double<long double>{typedef long double R;};



#if defined __GNUC__ && ( defined __x86_64__ || defined __ppc64__ || defined __MINGW64__ )
typedef __int128_t i128;
typedef __uint128_t ui128;
template<>struct Double<i64>{typedef i128 R;};
template<>struct Double<ui64>{typedef ui128 R;};
#endif


}
