#pragma once


//#include<type_traits>


#define _CAT(a,b) a##b
#define CAT(a,b) _CAT(a,b)
#define _10TH(a,b,c,d,e,f,g,h,i,X,...) X
#define VA_CNT(...) _10TH(__VA_ARGS__,9,8,7,6,5,4,3,2,1,0)
#define VA_SEL(a,...) CAT(a,VA_CNT(__VA_ARGS__))(__VA_ARGS__)

#define _VA_GTONE(...) _10TH(__VA_ARGS__,1,1,1,1,1,1,1,1,0,0)
#define VA_GTONE(a,...) CAT(a,_VA_GTONE(__VA_ARGS__))(__VA_ARGS__)


#define MIN_GCC(a,b,c) (__GNUC__ > (a) || \
              (__GNUC__ == (a) && (__GNUC_MINOR__ > (b) || \
                                 (__GNUC_MINOR__ == (b) && \
								 __GNUC_PATCHLEVEL__ >= (c)))))





// TODO: po co to bylo??
/*
#include<functional>
#if __cplusplus >= 201103L
namespace std
{
    template<class T>
    struct hash
    {
        typedef T argument_type;
        typedef typename std::underlying_type< argument_type >::type underlying_type;
        typedef typename std::hash< underlying_type >::result_type result_type;
        result_type operator()( const argument_type& arg ) const
        {
            std::hash< underlying_type > hasher;
            return hasher( static_cast< underlying_type >( arg ) );
        }
    };
}
#endif
*/


#define bac back()
#define fro front()
#define emp empty()
#define fi first
#define se second
#define siz size()
#define ins insert
#define era erase
#define mp make_pair
#define be begin()
#define en end()
#define rbe rbegin()
#define ren rend()
#define pub push_back
#define puf push_front
#define pob pop_back()
#define pof pop_front()
#define bs binary_search
#define lb lower_bound
#define ub upper_bound
#define pq priority_queue
#define si (int)
#define clr clear()


#undef FO
#undef OF

#define FOR(i,n) for(int i=0; i<(int)(n); ++i)
#define FO(i,a,b) for(int i=(a); i<(int)(b); ++i)
#define OF(i,a,b) for(int i=(int)(b)-1;i>=(int)(a);--i)
#define FE(it,c)  for(__typeof((c).begin()) it=(c).begin(); it!=(c).end(); ++it)
#define FER(it,c) for(__typeof((c).begin()) it=(c).begin(); it!=(c).end(); )
#define ALL(c) (c).begin(),(c).end()
#define ZERO(c) memset(c,0,sizeof(c))

#define AU(v,val) __typeof(val) v(val)

const char NL='\n';
const char SP=' ';


#include <atablash/types.hpp>



// Visual C++ intristics
#ifdef _MSC_VER
	#include <intrin.h>
	#define __builtin_popcount __popcnt

	#ifdef _WIN64
		#define __builtin_popcountll __popcnt64
	#else
		inline int __builtin_popcountll(__int64 a){ return __builtin_popcount((ui32)a) + __builtin_popcount(a >> 32); }
	#endif 
#endif


// Visual C++ 'and', 'or', 'not'
#ifdef _MSC_VER
	#define and &&
	#define or ||
	#define not !
#endif


namespace ab{

template<class T>inline T cbitl(T x,int i){return (x<<i) ^ (x>>(sizeof(T)*8-i));} // cycle bits left
inline int popcount(uint i){return __builtin_popcount(i);}
inline int popcount(ull i){return __builtin_popcountll(i);}

#ifdef __GNUC__
inline int parity(uint i){return __builtin_parity(i);}
inline int parity(ull i){return __builtin_parityll(i);}
inline int ilog2(uint i){return 31-__builtin_clz(i);} // int log; floor(log2(i))
inline int ilog2(ull i){return 63-__builtin_clzll(i);}
#endif


template<class T>inline bool isPowerOf2(const T&x){return !(x&(x-1));}



struct Static{
	Static() = default;
	Static(const Static&) = delete;
	Static(Static&&) = delete;
	Static(const Static&&) = delete;
	Static& operator=(const Static&) = delete;
	Static& operator=(Static&&) = delete;
	Static& operator=(const Static&&) = delete;
};



}



