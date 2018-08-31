#pragma once

#include<atablash/debug.hpp>

// first greater or equal
template<class FUN, class ARG, class VAL, class=typename std::enable_if<std::is_integral<ARG>::value>::type>
inline ARG funlb(const ARG&_a, const ARG&_b, const VAL&val, const FUN&f)
{
	ARG a = _a;
	ARG b = _b;
	A(a<b);
	while(a+1!=b){
		ARG s = (a+b)/2;
		auto nval = f(s-1);
		if(nval >= val)b=s;
		else a=s;
	}
	return a;
}
