#pragma once


namespace ab{

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


}

