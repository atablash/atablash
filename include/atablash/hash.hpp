#pragma once

#include<atablash/types.hpp>
#include<atablash/debug.hpp>
#include<atablash/math.hpp>
#include<atablash/lambda.hpp>

namespace ab{

template<int mod1 = (1u<<31)-1, int mod2 = (1u<<31)-19>
struct H2{
	H2() : h1(0), h2(0) {}
	H2(int x) : h1(x%mod1), h2(x%mod2) {}
	ui32 h1,h2;
	H2& operator+=(const H2& o){
		h1 = (h1+o.h1)%mod1;
		h2 = (h2+o.h2)%mod2;
		return *this;
	}
	H2 operator+(const H2& o)const{
		return H2(*this)+=o;
	}
	H2& operator-=(const H2& o){
		h1 = (mod1+h1-o.h1)%mod1;
		h2 = (mod2+h2-o.h2)%mod2;
		return *this;
	}
	H2 operator-(const H2& o)const{
		return H2(*this)-=o;
	}
	H2& operator*=(const H2& o){
		h1 = (1ULL*h1*o.h1)%mod1;
		h2 = (1ULL*h2*o.h2)%mod2;
		return *this;
	}
	H2 operator*(const H2& o)const{
		return H2(*this)*=o;
	}

	bool operator==(const H2& o)const{
		return h1==o.h1 and h2==o.h2;
	}

	bool operator!=(const H2& o)const{
		return !operator==(o);
	}
};

template<int mod1 = (1u<<31)-1>
struct H1{
	H1() : h1(0) {}
	H1(int x) : h1(x%mod1) {}
	ui32 h1;
	H1& operator+=(const H1& o){
		h1 = (h1+o.h1);
		if(h1>=mod1)h1-=mod1;
		return *this;
	}
	H1 operator+(const H1& o)const{
		return H1(*this)+=o;
	}
	H1& operator-=(const H1& o){
		h1 = (mod1+h1-o.h1);
		if(h1>=mod1)h1-=mod1;
		return *this;
	}
	H1 operator-(const H1& o)const{
		return H1(*this)-=o;
	}
	H1& operator*=(const H1& o){
		h1 = (1ULL*h1*o.h1)%mod1;
		return *this;
	}
	H1 operator*(const H1& o)const{
		return H1(*this)*=o;
	}
	H1& operator/=(const H1& o){
		//ASS(millerRabin(mod1));
		return (*this)*=fpow(o, mod1-2, _1 * _2, H1(1));
	}
	H1 operator/(const H1& o)const{
		return H1(*this)/=o;
	}

	/*
	bool operator==(const H1& o)const{
		return h1==o.h1;
	}

	bool operator!=(const H1& o)const{
		return !operator==(o);
	}*/

	operator ui32()const{
		return h1;
	}
};





// hash long long,
// fpow multiplication
template<ull mod = (1ULL<<63)-25> // big prime less than (2^64)/2
struct HLL{
	HLL() : h(0) {}
	HLL(ull x) : h(x%mod) {}
	ull h;
	HLL& operator+=(const HLL& o){
		h = (h+o.h);
		if(h>=mod)h-=mod;
		return *this;
	}
	HLL operator+(const HLL& o)const{
		return HLL(*this)+=o;
	}
	HLL& operator-=(const HLL& o){
		h = (mod+h-o.h);
		if(h>=mod)h-=mod;
		return *this;
	}
	HLL operator-(const HLL& o)const{
		return HLL(*this)-=o;
	}
	// multiplication by fast exponentation
	HLL operator*(const HLL& o)const{
		return fpow(*this, (ull)o, _1 + _2, HLL(0));
	}
	HLL& operator*=(const HLL& o){
		*this = *this * o;
		return *this;
	}
	HLL& operator/=(const HLL& o){
		//ASS(millerRabin(mod));
		return (*this)*=fpow(o, mod-2, _1 * _2, HLL(1));
	}
	HLL operator/(const HLL& o)const{
		return HLL(*this)/=o;
	}
	
	HLL& operator--(){
		if(h==0)h=mod-1;
		else --h;
	}
	HLL& operator++(){
		if(h==mod-1)h=0;
		else ++h;
	}

	/*
	bool operator==(const H1& o)const{
		return h1==o.h1;
	}

	bool operator!=(const H1& o)const{
		return !operator==(o);
	}*/

	operator ull()const{
		return h;
	}
};






// 2 ints + 1 long long
template<int mod1 = (1u<<31)-1, int mod2 = (1u<<31)-19>
struct SuperHash2{
	SuperHash2() : h1(0), h2(0), h(0) {}
	SuperHash2(int x) : h1(x%mod1), h2(x%mod2), h(x) {}
	ui32 h1,h2;
	ull h;
	SuperHash2& operator+=(const SuperHash2& o){
		h1 += o.h1;
		h2 += o.h2;
		if(h1>=mod1) h1-=mod1;
		if(h2>=mod2) h2-=mod2;
		h += o.h;
		return *this;
	}
	SuperHash2 operator+(const SuperHash2& o)const{
		return SuperHash2(*this)+=o;
	}
	SuperHash2& operator-=(const SuperHash2& o){
		h1 += mod1-o.h1;
		h2 += mod2-o.h2;
		if(h1>=mod1) h1-=mod1;
		if(h2>=mod2) h2-=mod2;
		h -= o.h;
		return *this;
	}
	SuperHash2 operator-(const SuperHash2& o)const{
		return SuperHash2(*this)-=o;
	}
	SuperHash2& operator*=(const SuperHash2& o){
		h1 = (1ULL*h1*o.h1)%mod1;
		h2 = (1ULL*h2*o.h2)%mod2;
		h *= o.h;
		return *this;
	}
	SuperHash2 operator*(const SuperHash2& o)const{
		return SuperHash2(*this)*=o;
	}

	bool operator==(const SuperHash2& o)const{
		return h1==o.h1 and h2==o.h2 and h==o.h;
	}

	bool operator!=(const SuperHash2& o)const{
		return !operator==(o);
	}
};








// 3 ints + 1 long long
template<int mod1 = (1u<<31)-1, int mod2 = (1u<<31)-19, int mod3 = (1u<<31)-69>
struct SuperHash3{
	SuperHash3() : h1(0), h2(0), h3(0), h(0) {}
	SuperHash3(int x) : h1(x%mod1), h2(x%mod2), h3(x%mod3), h(x) {}
	ui32 h1,h2,h3;
	ull h;
	SuperHash3& operator+=(const SuperHash3& o){
		h1 += o.h1;
		h2 += o.h2;
		h3 += o.h3;
		if(h1>=mod1) h1-=mod1;
		if(h2>=mod2) h2-=mod2;
		if(h3>=mod3) h3-=mod3;
		h += o.h;
		return *this;
	}
	SuperHash3 operator+(const SuperHash3& o)const{
		return SuperHash3(*this)+=o;
	}
	SuperHash3& operator-=(const SuperHash3& o){
		h1 += mod1-o.h1;
		h2 += mod2-o.h2;
		h3 += mod3-o.h3;
		if(h1>=mod1) h1-=mod1;
		if(h2>=mod2) h2-=mod2;
		if(h3>=mod3) h3-=mod3;
		h -= o.h;
		return *this;
	}
	SuperHash3 operator-(const SuperHash3& o)const{
		return SuperHash3(*this)-=o;
	}
	SuperHash3& operator*=(const SuperHash3& o){
		h1 = (1ULL*h1*o.h1)%mod1;
		h2 = (1ULL*h2*o.h2)%mod2;
		h3 = (1ULL*h3*o.h3)%mod3;
		h *= o.h;
		return *this;
	}
	SuperHash3 operator*(const SuperHash3& o)const{
		return SuperHash3(*this)*=o;
	}

	bool operator==(const SuperHash3& o)const{
		return h1==o.h1 and h2==o.h2 and h3==o.h3 and h==o.h;
	}

	bool operator!=(const SuperHash3& o)const{
		return !operator==(o);
	}
};










// dynamic hash
struct H1X{
	H1X(int mod) : mod1(mod), h1(0) {}
	H1X(int mod, int x) : mod1(mod), h1(x%mod1) {}
	//H1X(const H1X& o) : mod1(o.mod1), h1(o.h1) {}
	H1X& operator=(const H1X& o){
		ASS(mod1==o.mod1);
		//mod1 = o.mod1;
		h1 = o.h1;
		return *this;
	}
	const int mod1;
	ui32 h1;
	H1X& operator+=(const H1X& o){
		ASS(mod1==o.mod1);
		h1 = (h1+o.h1)%mod1;
		return *this;
	}
	H1X operator+(const H1X& o)const{
		return H1X(*this)+=o;
	}
	H1X& operator-=(const H1X& o){
		ASS(mod1==o.mod1);
		h1 = (mod1+h1-o.h1)%mod1;
		return *this;
	}
	H1X operator-(const H1X& o)const{
		return H1X(*this)-=o;
	}
	H1X& operator*=(const H1X& o){
		ASS(mod1==o.mod1);
		h1 = (1ULL*h1*o.h1)%mod1;
		return *this;
	}
	H1X operator*(const H1X& o)const{
		return H1X(*this)*=o;
	}
	H1X& operator/=(const H1X& o){
		ASS(mod1==o.mod1);
		//ASS(millerRabin(mod1));
		return (*this)*=fpow(o,mod1-2, (_1 * _2), H1X(mod1,1));
	}
	H1X operator/(const H1X& o)const{
		return H1X(*this)/=o;
	}

	/*
	bool operator==(const H1X& o)const{
		return h1==o.h1;
	}

	bool operator!=(const H1X& o)const{
		return !operator==(o);
	}*/

	operator ui32()const{
		return h1;
	}

	template<class T>
	explicit operator T()const{
		return h1;
	}
};


template<class INT> H1X operator/(const H1X& h, const INT& i){ ASS(h.mod1);
	return h / H1X(h.mod1,i);
}
template<class INT> H1X operator/(const INT& i, const H1X& h){ ASS(h.mod1);
	return H1X(h.mod1,i) / h;
}

template<class INT> H1X operator*(const H1X& h, const INT& i){ ASS(h.mod1);
	return h * H1X(h.mod1,i);
}
template<class INT> H1X operator*(const INT& i, const H1X& h){ ASS(h.mod1);
	return H1X(h.mod1,i) * h;
}

template<class INT> H1X operator+(const H1X& h, const INT& i){ ASS(h.mod1);
	return h + H1X(h.mod1,i);
}
template<class INT> H1X operator+(const INT& i, const H1X& h){ ASS(h.mod1);
	return H1X(h.mod1,i) + h;
}

template<class INT> H1X operator-(const H1X& h, const INT& i){ ASS(h.mod1);
	return h - H1X(h.mod1,i);
}
template<class INT> H1X operator-(const INT& i, const H1X& h){ ASS(h.mod1);
	return H1X(h.mod1,i) - h;
}



}



#if __cplusplus >= 201103L or defined _MSC_VER
#include <unordered_map>
namespace std{
#else
#include <ext/hash_map>
namespace __gnu_cxx{
#endif

template<int h1, int h2> struct hash<ab::H2<h1,h2> >{
	size_t operator()(const ab::H2<h1,h2>& h)const{
		return h.h1^h.h2;
	}
};
template<int h1> struct hash<ab::H1<h1> >{
	size_t operator()(const ab::H1<h1>& h)const{
		return h.h1;
	}
};

}

