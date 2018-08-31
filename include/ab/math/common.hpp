#pragma once


namespace ab
{

// type-less!
#define PI 3.141592653589793238462643383279502884197169399375105820974944L



template<class T> const T& max2(const T& a, const T& b){return a<b?b:a;}
template<class T> const T& min2(const T& a, const T& b){return b<a?b:a;}



template<class T, class F>
inline T lerp(const T& a, const T& b, const F& alpha){
	return (1.0f-alpha)*a + alpha*b;
}




}

