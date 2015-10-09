#pragma once
















////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ACTUAL SOLUTION IS AT THE END OF THIS FILE!!!                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


















////////////////////////// PROTOTYPING TEMPLATE /////////////////////////////////
//
// Implementation: Adam Blaszkiewicz (atablash.pl), 2012-2014
/*
    This program is licensed under GNU General Public License version 3.
    See <http://www.gnu.org/licenses/>.
*/
//
//

// enable c++ iostream; only if no fastio
//#define IOSTREAM

//#define NO_UNLOCKED	// fread_unlocked, fwrite_unlocked



// PRIi64, PRIui64
#define __STDC_FORMAT_MACROS
#include<inttypes.h>

#include<atablash/common.hpp>

//#if MIN_GCC(4,1,3)
//#include<bits/stdc++.h>
//#else
#include<vector>
#include<map>
#include<queue>
#include<algorithm>
#include<string>
#include<stdint.h>
#include<cstring>
#include<iostream>
#include<set>
#include<climits>
#include<bitset>
#include<limits>
#include<sstream>
#include<cmath>
//#endif


#include<atablash/debug.hpp>
#include<atablash/stlext.hpp>




/*
#define fo3(i,a,b) for(int i=(int)(a); i<(int)(b); ++i)
#define of3(i,a,b) for(int i=(int)((b)-1); i>=(int)(a); --i)

#define fo2(i,n) fo3(i,0,n)
#define of2(i,n) of3(i,0,n)

#define fo(...) VA_SEL(fo,__VA_ARGS__)
#define of(...) VA_SEL(of,__VA_ARGS__)
*/

#define inl		inline __attribute__ ((always_inline))
#define ninl	__attribute__ ((noinline))




#include<atablash/types.hpp>


#ifdef DEBUG_BUILD
	#undef FASTIO
#endif





#ifdef _WIN32

// TODO
#ifndef NO_UNLOCKED
#define NO_UNLOCKED
#endif

//#define fread_unlocked _fread_nolock
//#define fwrite_unlocked _fwrite_nolock

#endif



using namespace std;
using namespace __gnu_cxx;

namespace ab{}
using namespace ab;





template<class T>inline void sortu(T& t){t.resize(std::unique(ALL(t))-t.be);}


inline uint _rand(){
	if(RAND_MAX < (1<<15)) return (rand()<<15) | rand();
	else return rand();
}
#define RND _rand()


#define ri _rint()
#define rui _ruint()
#define rll _rll()
#define rull _rull()
#define rchar _rchar()
#define rstr _rstr()


#ifndef FASTIO
inline void _fin(int){}
inline void _fout(int){}
#ifdef IOSTREAM
inline int _rint(){int r;cin>>r;return r;}
inline uint _ruint(){uint r;cin>>r;return r;}
inline ull _rull(){ull r;cin>>r;return r;}
inline ll _rll(){ll r;cin>>r;return r;}
inline char _rchar(){return cin.get();}
inline string _rstr(){string s;cin>>s;return s;}
template<typename T>inline void W(const T& i){cout<<i; cout.flush();}
inline void W(){W('\n');}
#else
inline int _rint(){int r;scanf("%d",&r);return r;}
inline uint _ruint(){uint r;scanf("%u",&r);return r;}
inline ull _rull(){ull r;scanf("%" SCNu64,&r);return r;}
inline ull _rll(){ll r;scanf("%" SCNd64,&r);return r;}
inline char _rchar(){return getchar();}
inline string _rstr(){
	string s;
	char c=rchar;
	while(c!=-1 && !isgraph(c))c=rchar;
	for(;;){
		if(!isgraph(c))break;
		s+=c;
		c=rchar;
	}
	return s;
}
inline void W(char a='\n'){printf("%c",a);}
inline void W(const string&s){printf("%s",s.c_str());}
inline void W(int a){printf("%d",a);}
inline void W(uint a){printf("%u",a);}
inline void W(const ui64& a){printf("%" PRIu64,a);}
inline void W(const i64& a){printf("%" PRIi64,a);}
inline void W(const char *str){fputs(str,stdout);}
inline void W(const double& d){printf("%.20f",d);}
inline void W(const long double& d){printf("%.50Lf",d);}
#endif
#else

#ifndef NO_UNLOCKED
#define FREAD fread_unlocked
#define FWRITE fwrite_unlocked
#else
#define FREAD fread
#define FWRITE fwrite
#endif

// todo: test on pointers
const int _IB=32768;const int _OB=32768;
char _iB[_IB];int _iP = _IB;
char _oB[_OB];int _oP = 0;
inline void _fin(int mc){
	int r=_IB-_iP;
	if(r>mc)return;
	FOR(i,r) _iB[i]=_iB[i+_iP];	// TODO: try memcpy
    int rd=FREAD(_iB+r,1,_iP,stdin);
	if(rd!=_iP) _iB[r+rd]=0;
	_iP = 0;
}
inline void _fout(int mc){
	if(_OB-_oP>mc)return;
	FWRITE(_oB,1,_oP,stdout);
	_oP = 0;
}
inline void _W(char c='\n'){_oB[_oP++]=c;} // unchecked!
inline char _rc(){return _iB[_iP++];}
inline char _rchar(){_fin(1);return _rc();}
inline string _rstr(){
	string s;
	for(;;){
		char c=rchar;
		if(!isprint(c)||isspace(c))break;
		s+=c;
	}
	return s;
}
template<class T>inline T _rur(){ // unchecked!
    T __r=0;char c=_rc();
    do c-='0',__r=__r*10+c,c=_rc();while(c>='0');
    return __r;}
template<class T>inline T _rsi(){ // unchecked!
	char c=_rc();
	while(c<'-')c=_rc();
	if(c=='-')return -_rur<T>();
	else{--_iP;return _rur<T>();}}
template<class T>inline T _ru(){  // unchecked!
	while(_rc()<'0');--_iP;
	return _rur<T>();
}

inline int _rint(){_fin(15);return _rsi<int>();}
inline uint _ruint(){_fin(15);return _ru<uint>();}
inline ll _rll(){_fin(25);return _rsi<ll>();}
inline ull _rull(){_fin(25);return _ru<ull>();}

const int _rs = 20; char _r[_rs];
template<class T>inline void _w(T i){	// unchecked!
	if(numeric_limits<T>::is_signed and i<0) _W('-'), i=-i;
	else if(i==0){_W('0'); return;}
	int pos = _rs;
	do _r[--pos]='0'+i%10, i/=10; while(i);
	do _W(_r[pos++]); while(pos<_rs);
}
inline void W(const char c){_fout(2);_W(c);}
inline void W(const int32_t& i){_fout(15);_w(i);}
inline void W(const uint32_t& i){_fout(15);_w(i);}
inline void W(const int64_t& i){_fout(25);_w(i);}
inline void W(const uint64_t& i){_fout(25);_w(i);}
inline void W(const char*str, int l){
	for(int b=0; b<l; b+=_OB){
		int num = min(_OB,l-b);
		_fout(num);
		memcpy(&_oB[_oP], str+b, num);
		_oP+=num;
	}
}
inline void W(const char*str){W(str,strlen(str));}
inline void W(const string&s){W(s.c_str(),s.siz);}
inline void W(double d){_fout(50);_oP+=sprintf(&_oB[_oP],"%.20f",d);}	// todo
#endif
template<class A,class B>inline void W(const A&a,const B&b){W(a);W(b);}
template<class A,class B,class C>
inline void W(const A&a,const B&b,const C&c){W(a);W(b);W(c);}
template<class A,class B,class C,class DD>
inline void W(const A&a,const B&b,const C&c,const DD&dd){W(a);W(b);W(c);W(dd);}
template<class T>inline void sortall(T&t){sort(t.be,t.en);}
template<class T>inline void uniqall(T&t){t.resize(unique(t.be,t.en)-t.be);}


#define rgraph _rgraph()
inline char _rgraph(){char c;do c=rchar;while(!isgraph(c));return c;}

#define subset(a,b) (a&b==a)
#define kbegin(k) ((1<<k)-1)
#define kend(k,n) ((kbegin(k)<<n-k)+1)
inline uint knext(uint a){uint b=(a|(a-1))+1;return b|((a^b)>>(__builtin_ctz(a)+2));}
#define fors(x,k,n) for(uint x=kbegin(k);x<kend(k,n);x=knext(x))

struct _O{template<class T>_O&operator<(const T&_t){
		oc(1);W(_t);oc(0);
		return *this;
}}O;

struct _I{
	_I&operator>(int32_t&_t){_t=ri;return*this;}
	_I&operator>(uint32_t&_t){_t=rui;return*this;}
	_I&operator>(int64_t&_t){_t=rll;return*this;}
	_I&operator>(uint64_t&_t){_t=rull;return*this;}
	_I&operator>(char&_t){_t=rchar;return*this;}
	_I&operator>(string&_t){_t=rstr;return*this;}
}I;




inline void tc();
#ifdef GEN
int argc;  char **argv;
int main(int _argc, char *_argv[]){
	argc = _argc;  argv = _argv;
	if(argc<2){
		O < "Usage: " < argv[0] < " NR_TESTU" < N;
		return 1;
	}
	srand(atoi(argv[1]));
#else
int main(){
#endif

	//Log log;
	//log.autoFlush = true;
	//Log::global() = &log;

#ifdef FASTIO
    setvbuf(stdin,0,_IONBF,0);setvbuf(stdout,0,_IONBF,0);
#elif defined IOSTREAM
	ios_base::sync_with_stdio(0);cout.precision(10);cout.setf(ios::fixed,ios::floatfield);
	#if not defined DEBUG_BUILD and not defined COLOR_CONSOLE
	cin.tie(0);
	#endif
#endif

#ifdef DEBUG_BUILD
#ifdef _WIN32
	ec(0);
	oc(0);
#else
// TODO: *nix
#endif
#endif
	
#ifdef READ_NUM_TEST_CASES
	int nz=rui;FOR(i,nz)
#endif
	tc();
#ifdef FASTIO
    FWRITE(_oB,1,_oP,stdout);
#endif
	return 0;
}
// END OF TEMPLATE





































////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ACTUAL SOLUTION BEGINS HERE:                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
