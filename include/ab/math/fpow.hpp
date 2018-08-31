#pragma once



namespace ab{


template<class T, class E, class OP>
inline T fpow(T a, E exp, const OP& op = std::multiplies<T>(), const T& neutral = 1){
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




}



