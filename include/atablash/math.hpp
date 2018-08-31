#pragma once

#include<cstdlib>


#include<atablash/lambda.hpp>

namespace ab
{


// type-less!
#define PI 3.141592653589793238462643383279502884197169399375105820974944


template<typename T, typename IDX=int>
struct binomial
{
	binomial() : n(0), k(0), val(1) {}
	binomial(const IDX& nn, const IDX& kk) : n(nn), val(1){
		ASS(0<=kk);
		if(2*kk<nn){
			k = 0;
			while(k<kk)moveR();
		}
		else{
			k=nn;
			while(k>kk)moveL();
		}
	}
	IDX n,k;
	T val;
	
	operator T(){
		return val;
	}

	bool outside(){
		bool ret = n<0 || k<0 || k>n;
		if(ret)val=0;
		return ret;
	}

	void moveL(){
		--k;
		if(outside()) return;

		if(k==n) val=1;
		else
		{
			val*=(k+1);
			val/=(n-k);
		}
	}
	void moveR(){
		++k;
		if(outside()) return;

		if(k==0) val=1;
		else
		{
			val*=(n-k+1);
			val/=(k);
		}
	}
	void moveD(){
		++n;
		if(outside()) return;

		if(n==k) val=1;
		else
		{
			val*=n;
			val/=(n-k);
		}
	}
	void moveU(){
		--n;
		if(outside()) return;

		val*=(n-k+1);
		val/=(n+1);
	}
};



// GCD
#define gcd __gcd
/*
template<typename T>
T gcd(T a, T b)
{
	while(b!=0)
	{
		T t = a;
		a = a%b;
		b = t;
	}
	return a;
}
*/


// a*aa + b*bb == gcd(a,b)
template<typename T>
T gcdExt(T a, T b, T& aaa, T& bbb)
{
	T a0 = a;
	T b0 = b;

	T aa = 0;
	T bb = 1;
	aaa = 1;
	bbb = 0;
	// niezmiennik: a0*aa  + b0*bb  == b
	// niezmiennik: a0*aaa + b0*bbb == a
	while(b != 0)
	{
		T q = a/b;
		a = a%b;
		// a -= b*(a/b)
		aaa -= aa*q;
		bbb -= bb*q;
		swap(a,b);
		swap(aa,aaa);
		swap(bb,bbb);
	}

	ASS(a0*aaa + b0*bbb == a);

	return a;
}



template<class T, class E, class OP>
inline T fpow(T a, E exp, const OP& op, const T& neutral = 1){
	T r = neutral;
	while(exp){
		if(exp&1){
			r = op(r,a);
			--exp;
		}
		else{
			exp >>= 1;
			a = op(a,a);
		}
	}
	return r;
}

template<class T, class E>
inline T fpow(T a, E exp){
	return fpow(a, exp, _1 * _2);
}



template<class T> const T& max2(const T& a, const T& b){return a<b?b:a;}
template<class T> const T& min2(const T& a, const T& b){return b<a?b:a;}




template<class T, class F>
inline T lerp(const T& a, const T& b, const F& alpha){
	return (1.0f-alpha)*a + alpha*b;
}





}

