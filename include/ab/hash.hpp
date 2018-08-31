#pragma once

#include<atablash/types.hpp>
#include<atablash/debug.hpp>
#include<atablash/math/fpow.hpp>

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
		return (*this)*=fpow(o, mod1-2, std::multiplies<H1>(), H1(1));
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






// dynamic hash
struct H1X{
	H1X(uint mod) : mod1(mod), h1(0) {}
	H1X(uint mod, uint x) : mod1(mod), h1(x%mod1) {}
	//H1X(const H1X& o) : mod1(o.mod1), h1(o.h1) {}
	H1X& operator=(const H1X& o){
		ASS(mod1==o.mod1);
		//mod1 = o.mod1;
		h1 = o.h1;
		return *this;
	}
	const uint mod1;
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
		return (*this)*=fpow(o,mod1-2, std::multiplies<H1X>(), H1X(mod1,1));
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
	
	H1X& operator++(){
		++h1;
		if(h1 == mod1) h1 = 0;
		return *this;
	}
	H1X& operator--(){
		if(h1 == 0) h1 = mod1;
		--h1;
		return *this;
	}

	operator ui32()const{
		return h1;
	}

	/* works only in c++11 (explicit keyword)
	template<class T>
	explicit operator T()const{
		return h1;
	}
	*/
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

