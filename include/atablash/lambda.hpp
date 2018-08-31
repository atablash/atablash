#pragma once

#include<atablash/functional.hpp>
#include<atablash/templates.hpp>

namespace ab{

struct Lambda1{
	template<class A1>
	const A1& operator()(const A1& a1)const{return a1;}
	template<class A1>
	      A1& operator()(      A1& a1)const{return a1;}
	
	template<class A1,class A2>
	const A1& operator()(const A1& a1, const A2&)const{return a1;}
	template<class A1,class A2>
	      A1& operator()(      A1& a1, const A2&)const{return a1;}
	
	template<class A1,class A2,class A3>
	const A1& operator()(const A1& a1, const A2&, const A3&)const{return a1;}
	template<class A1,class A2,class A3>
	      A1& operator()(      A1& a1, const A2&, const A3&)const{return a1;}
};


struct Lambda2{
	template<class A1,class A2>
	const A2& operator()(const A1& a1, const A2& a2)const{return a2;}
	template<class A1,class A2>
	      A2& operator()(const A1& a1,       A2& a2)const{return a2;}
	
	template<class A1,class A2,class A3>
	const A2& operator()(const A1& a1, const A2& a2, const A3& a3)const{return a2;}
	template<class A1,class A2,class A3>
	      A2& operator()(const A1& a1,       A2& a2, const A3& a3)const{return a2;}
};


struct Lambda3{
	template<class A1,class A2,class A3>
	const A3& operator()(const A1& a1, const A2& a2, const A3& a3)const{return a3;}
	template<class A1,class A2,class A3>
	      A3& operator()(const A1& a1, const A2& a2,       A3& a3)const{return a3;}
};


#ifdef __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-variable"	// not working
#endif


static Lambda1 _1;
static Lambda2 _2;
static Lambda3 _3;
//#define _1 Lambda1()
//#define _2 Lambda2()
//#define _3 Lambda3()



#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif




#ifdef __GNUC__
	// _1 _2 _3 unused
	// pragma diagnostic ignored not working?
	template<class T>void ___fhsdhsdhrtshx(){(void)_1;(void)_2;(void)_3;}
#endif







template<class L, class A1, class A2, class A3> struct RET_TYPE3{typedef L& R;};

template<class L, class A1> struct RET_TYPE1{typedef typename RET_TYPE3<L,A1,A1,A1>::R R;};
template<class L, class A1, class A2> struct RET_TYPE2{typedef typename RET_TYPE3<L,A1,A2,A2>::R R;};






// is lambda before evaluation
template<class L> struct IS_LAMBDA{static const bool r = false;};





// is lambda after evaluation
template<class L, class A1, class A2, class A3> struct IS_LAMBDA3{
static const bool r = !SAME<L&, typename RET_TYPE3<L,A1,A2,A3>::R&>::r;};

template<class L, class A1, class A2> struct IS_LAMBDA2{
static const bool r = !SAME<typename RR<L>::R, typename RR<typename RET_TYPE2<L,A1,A2>::R>::R>::r;};

template<class L, class A1> struct IS_LAMBDA1{
static const bool r = !SAME<typename RR<L>::R, typename RR<typename RET_TYPE1<L,A1>::R>::R>::r;};




template<class T, class A1>
typename ENABLE_IF  < IS_LAMBDA1<T,A1>::r,      typename RET_TYPE1<T,const A1>::R>::R Eval(const T& t, const A1& a1){return t(a1);}
template<class T, class A1>
typename ENABLE_IF  < IS_LAMBDA1<T,A1>::r,      typename RET_TYPE1<T,      A1>::R>::R Eval(const T& t,       A1& a1){return t(a1);}

template<class T, class A1>
typename DISABLE_IF < IS_LAMBDA1<T,A1>::r,      typename RET_TYPE1<T,A1>::R&>::R Eval(      T& t, const A1& a1){return t;}




template<class T, class A1, class A2>
typename ENABLE_IF < IS_LAMBDA2<T,A1,A2>::r,      typename RET_TYPE2<T,const A1,const A2>::R>::R Eval(const T& t, const A1& a1, const A2& a2){return t(a1,a2);}
template<class T, class A1, class A2>
typename ENABLE_IF < IS_LAMBDA2<T,A1,A2>::r,      typename RET_TYPE2<T,const A1,      A2>::R>::R Eval(const T& t, const A1& a1,       A2& a2){return t(a1,a2);}
template<class T, class A1, class A2>
typename ENABLE_IF < IS_LAMBDA2<T,A1,A2>::r,      typename RET_TYPE2<T,      A1,const A2>::R>::R Eval(const T& t,       A1& a1, const A2& a2){return t(a1,a2);}
template<class T, class A1, class A2>
typename ENABLE_IF < IS_LAMBDA2<T,A1,A2>::r,      typename RET_TYPE2<T,      A1,      A2>::R>::R Eval(const T& t,       A1& a1,       A2& a2){return t(a1,a2);}

template<class T, class A1, class A2>
typename DISABLE_IF< IS_LAMBDA2<T,A1,A2>::r,      typename RET_TYPE2<T,A1,A2>::R&>::R Eval(      T& t, const A1& a1, const A2& a2){return t;}




template<class T, class A1, class A2, class A3>
typename ENABLE_IF  < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,const A1,const A2,const A3>::R>::R Eval(const T& t, const A1& a1, const A2& a2, const A3& a3){return t(a1,a2,a3);}
template<class T, class A1, class A2, class A3>
typename ENABLE_IF  < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,const A1,const A2,      A3>::R>::R Eval(const T& t, const A1& a1, const A2& a2,       A3& a3){return t(a1,a2,a3);}
template<class T, class A1, class A2, class A3>
typename ENABLE_IF  < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,const A1,      A2,const A3>::R>::R Eval(const T& t, const A1& a1,       A2& a2, const A3& a3){return t(a1,a2,a3);}
template<class T, class A1, class A2, class A3>
typename ENABLE_IF  < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,const A1,      A2,      A3>::R>::R Eval(const T& t, const A1& a1,       A2& a2,       A3& a3){return t(a1,a2,a3);}
template<class T, class A1, class A2, class A3>
typename ENABLE_IF  < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,      A1,const A2,const A3>::R>::R Eval(const T& t,       A1& a1, const A2& a2, const A3& a3){return t(a1,a2,a3);}
template<class T, class A1, class A2, class A3>
typename ENABLE_IF  < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,      A1,const A2,      A3>::R>::R Eval(const T& t,       A1& a1, const A2& a2,       A3& a3){return t(a1,a2,a3);}
template<class T, class A1, class A2, class A3>
typename ENABLE_IF  < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,      A1,      A2,const A3>::R>::R Eval(const T& t,       A1& a1,       A2& a2, const A3& a3){return t(a1,a2,a3);}
template<class T, class A1, class A2, class A3>
typename ENABLE_IF  < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,      A1,      A2,      A3>::R>::R Eval(const T& t,       A1& a1,       A2& a2,       A3& a3){return t(a1,a2,a3);}

template<class T, class A1, class A2, class A3>
typename DISABLE_IF < IS_LAMBDA3<T,A1,A2,A3>::r,      typename RET_TYPE3<T,A1,A2,A3>::R&>::R Eval(      T& t, const A1& a1, const A2& a2, const A3& a3){return t;}







template<class L1, class L2>
class LambdaPlus{
public:
	LambdaPlus(const L1& _l1, const L2& _l2) : l1(_l1), l2(_l2){}
	
	template<class A1>
	typename RET_TYPE1<LambdaPlus,A1>::R operator()(const A1& a1)const{
		return Eval(l1,a1) + Eval(l2,a1);
	}
	
	template<class A1, class A2>
	typename RET_TYPE2<LambdaPlus,A1,A2>::R operator()(const A1& a1, const A2& a2)const{
		return Eval(l1,a1,a2) + Eval(l2,a1,a2);
	}
	
	template<class A1, class A2, class A3>
	typename RET_TYPE3<LambdaPlus,A1,A2,A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{
		return Eval(l1,a1,a2,a3) + Eval(l2,a1,a2,a3);
	}
	
private:
	const L1& l1;
	const L2& l2;
};

template<class L1, class L2>
typename ENABLE_IF<HAS_OPERATOR_CALL<L1>::r or HAS_OPERATOR_CALL<L2>::r, LambdaPlus<L1,L2> >::R operator+(const L1& l1, const L2& l2){
	return LambdaPlus<L1,L2>(l1,l2);
}



template<class L1, class L2>
class LambdaMinus{
public:
	LambdaMinus(const L1& _l1, const L2& _l2) : l1(_l1), l2(_l2){}
	
	template<class A1>
	typename RET_TYPE1<LambdaMinus,A1>::R operator()(const A1& a1)const{
		return Eval(l1,a1) - Eval(l2,a1);
	}
	
	template<class A1, class A2>
	typename RET_TYPE2<LambdaMinus,A1,A2>::R operator()(const A1& a1, const A2& a2)const{
		return Eval(l1,a1,a2) - Eval(l2,a1,a2);
	}
	
	template<class A1, class A2, class A3>
	typename RET_TYPE3<LambdaMinus,A1,A2,A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{
		return Eval(l1,a1,a2,a3) - Eval(l2,a1,a2,a3);
	}
	
private:
	const L1& l1;
	const L2& l2;
};

template<class L1, class L2>
typename ENABLE_IF<HAS_OPERATOR_CALL<L1>::r or HAS_OPERATOR_CALL<L2>::r, LambdaMinus<L1,L2> >::R operator-(const L1& l1, const L2& l2){
	return LambdaMinus<L1,L2>(l1,l2);
}






template<class L1>
class LambdaUnaryMinus{
public:
	LambdaUnaryMinus(const L1& _l1) : l1(_l1){}
	
	template<class A1>
	typename RET_TYPE1<LambdaUnaryMinus,A1>::R operator()(const A1& a1)const{
		return -Eval(l1,a1);
	}
	
	template<class A1, class A2>
	typename RET_TYPE2<LambdaUnaryMinus,A1,A2>::R operator()(const A1& a1, const A2& a2)const{
		return -Eval(l1,a1,a2);
	}
	
	template<class A1, class A2, class A3>
	typename RET_TYPE3<LambdaUnaryMinus,A1,A2,A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{
		return -Eval(l1,a1,a2,a3);
	}
	
private:
	const L1& l1;
};

template<class L1>
typename ENABLE_IF<HAS_OPERATOR_CALL<L1>::r, LambdaUnaryMinus<L1> >::R operator-(const L1& l1){
	return LambdaUnaryMinus<L1>(l1);
}






template<class L1, class L2>
class LambdaMultiplies{
public:
	LambdaMultiplies(const L1& _l1, const L2& _l2) : l1(_l1), l2(_l2){}
	
	template<class A1>
	typename RET_TYPE1<LambdaMultiplies,A1>::R operator()(const A1& a1)const{
		return Eval(l1,a1) * Eval(l2,a1);
	}
	
	template<class A1, class A2>
	typename RET_TYPE2<LambdaMultiplies,A1,A2>::R operator()(const A1& a1, const A2& a2)const{
		return Eval(l1,a1,a2) * Eval(l2,a1,a2);
	}
	
	template<class A1, class A2, class A3>
	typename RET_TYPE3<LambdaMultiplies,A1,A2,A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{
		return Eval(l1,a1,a2,a3) * Eval(l2,a1,a2,a3);
	}
	
private:
	const L1& l1;
	const L2& l2;
};

template<class L1, class L2>
typename ENABLE_IF<HAS_OPERATOR_CALL<L1>::r or HAS_OPERATOR_CALL<L2>::r, LambdaMultiplies<L1,L2> >::R operator*(const L1& l1, const L2& l2){
	return LambdaMultiplies<L1,L2>(l1,l2);
}




template<class L1, class L2>
class LambdaDivides{
public:
	LambdaDivides(const L1& _l1, const L2& _l2) : l1(_l1), l2(_l2){}
	
	template<class A1>
	typename RET_TYPE1<LambdaDivides,A1>::R operator()(const A1& a1)const{
		return Eval(l1,a1) / Eval(l2,a1);
	}
	
	template<class A1, class A2>
	typename RET_TYPE2<LambdaDivides,A1,A2>::R operator()(const A1& a1, const A2& a2)const{
		return Eval(l1,a1,a2) / Eval(l2,a1,a2);
	}
	
	template<class A1, class A2, class A3>
	typename RET_TYPE3<LambdaDivides,A1,A2,A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{
		return Eval(l1,a1,a2,a3) / Eval(l2,a1,a2,a3);
	}
	
private:
	const L1& l1;
	const L2& l2;
};

template<class L1, class L2>
typename ENABLE_IF<HAS_OPERATOR_CALL<L1>::r or HAS_OPERATOR_CALL<L2>::r, LambdaDivides<L1,L2> >::R operator/(const L1& l1, const L2& l2){
	return LambdaDivides<L1,L2>(l1,l2);
}














template<class L1, class L2>
class LambdaPlusAssign{
public:
	LambdaPlusAssign(L1& _l1, const L2& _l2) : l1(_l1), l2(_l2){}
	
	template<class A1> typename RET_TYPE1<LambdaPlusAssign,A1>::R operator()(const A1& a1)const{return Eval(l1,a1) += Eval(l2,a1);}
	template<class A1> typename RET_TYPE1<LambdaPlusAssign,A1>::R operator()(      A1& a1)const{return Eval(l1,a1) += Eval(l2,a1);}
	
	template<class A1, class A2> typename RET_TYPE2<LambdaPlusAssign,const A1,const A2>::R operator()(const A1& a1, const A2& a2)const{return Eval(l1,a1,a2) += Eval(l2,a1,a2);}
	template<class A1, class A2> typename RET_TYPE2<LambdaPlusAssign,const A1,      A2>::R operator()(const A1& a1,       A2& a2)const{return Eval(l1,a1,a2) += Eval(l2,a1,a2);}
	template<class A1, class A2> typename RET_TYPE2<LambdaPlusAssign,      A1,const A2>::R operator()(      A1& a1, const A2& a2)const{return Eval(l1,a1,a2) += Eval(l2,a1,a2);}
	template<class A1, class A2> typename RET_TYPE2<LambdaPlusAssign,      A1,      A2>::R operator()(      A1& a1,       A2& a2)const{return Eval(l1,a1,a2) += Eval(l2,a1,a2);}
	
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaPlusAssign,A1,A2,A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{return Eval(l1,a1,a2,a3) += Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaPlusAssign,A1,A2,A3>::R operator()(const A1& a1, const A2& a2,       A3& a3)const{return Eval(l1,a1,a2,a3) += Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaPlusAssign,A1,A2,A3>::R operator()(const A1& a1,       A2& a2, const A3& a3)const{return Eval(l1,a1,a2,a3) += Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaPlusAssign,A1,A2,A3>::R operator()(const A1& a1,       A2& a2,       A3& a3)const{return Eval(l1,a1,a2,a3) += Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaPlusAssign,A1,A2,A3>::R operator()(      A1& a1, const A2& a2, const A3& a3)const{return Eval(l1,a1,a2,a3) += Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaPlusAssign,A1,A2,A3>::R operator()(      A1& a1, const A2& a2,       A3& a3)const{return Eval(l1,a1,a2,a3) += Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaPlusAssign,A1,A2,A3>::R operator()(      A1& a1,       A2& a2, const A3& a3)const{return Eval(l1,a1,a2,a3) += Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaPlusAssign,A1,A2,A3>::R operator()(      A1& a1,       A2& a2,       A3& a3)const{return Eval(l1,a1,a2,a3) += Eval(l2,a1,a2,a3);}
	
private:
	L1& l1;
	const L2& l2;
};


template<class L1, class L2>
typename ENABLE_IF<IS_LAMBDA<L1>::r or IS_LAMBDA<L2>::r, LambdaPlusAssign<L1,L2> >::R operator+=(L1& l1, const L2& l2){
	return LambdaPlusAssign<L1,L2>(l1,l2);
}







// LOGICAL

template<class L1, class L2>
class LambdaLess{
public:
	LambdaLess(const L1& _l1, const L2& _l2) : l1(_l1), l2(_l2){}
	
	template<class A1>
	typename RET_TYPE1<LambdaLess,A1>::R operator()(const A1& a1)const{
		return Eval(l1,a1) < Eval(l2,a1);
	}
	
	template<class A1, class A2>
	typename RET_TYPE2<LambdaLess,A1,A2>::R operator()(const A1& a1, const A2& a2)const{
		return Eval(l1,a1,a2) < Eval(l2,a1,a2);
	}
	
	template<class A1, class A2, class A3>
	typename RET_TYPE3<LambdaLess,A1,A2,A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{
		return Eval(l1,a1,a2,a3) < Eval(l2,a1,a2,a3);
	}
	
private:
	const L1& l1;
	const L2& l2;
};
template<class L1, class L2>
typename ENABLE_IF<HAS_OPERATOR_CALL<L1>::r or HAS_OPERATOR_CALL<L2>::r, LambdaLess<L1,L2> >::R operator<(const L1& l1, const L2& l2){
	return LambdaLess<L1,L2>(l1,l2);
}





template<class L1, class L2>
class LambdaGreater{
public:
	LambdaGreater(const L1& _l1, const L2& _l2) : l1(_l1), l2(_l2){}
	
	template<class A1>
	typename RET_TYPE1<LambdaGreater,A1>::R operator()(const A1& a1)const{
		return Eval(l1,a1) > Eval(l2,a1);
	}
	
	template<class A1, class A2>
	typename RET_TYPE2<LambdaGreater,A1,A2>::R operator()(const A1& a1, const A2& a2)const{
		return Eval(l1,a1,a2) > Eval(l2,a1,a2);
	}
	
	template<class A1, class A2, class A3>
	typename RET_TYPE3<LambdaGreater,A1,A2,A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{
		return Eval(l1,a1,a2,a3) > Eval(l2,a1,a2,a3);
	}
	
private:
	const L1& l1;
	const L2& l2;
};

template<class L1, class L2>
typename ENABLE_IF<HAS_OPERATOR_CALL<L1>::r or HAS_OPERATOR_CALL<L2>::r, LambdaGreater<L1,L2> >::R operator>(const L1& l1, const L2& l2){
	return LambdaGreater<L1,L2>(l1,l2);
}






template<class L1, class L2>
class LambdaDotStar{
public:
	LambdaDotStar(L1& _l1, const L2& _l2) : l1(_l1), l2(_l2) {}
	
	template<class A1> typename RET_TYPE1<LambdaDotStar,const A1>::R operator()(const A1& a1)const{return Eval(l1,a1).*Eval(l2,a1);}
	template<class A1> typename RET_TYPE1<LambdaDotStar,      A1>::R operator()(      A1& a1)const{return Eval(l1,a1).*Eval(l2,a1);}
	
	template<class A1, class A2> typename RET_TYPE2<LambdaDotStar,const A1,const A2>::R operator()(const A1& a1, const A2& a2)const{return Eval(l1,a1,a2).*Eval(l2,a1,a2);}
	template<class A1, class A2> typename RET_TYPE2<LambdaDotStar,const A1,      A2>::R operator()(const A1& a1,       A2& a2)const{return Eval(l1,a1,a2).*Eval(l2,a1,a2);}
	template<class A1, class A2> typename RET_TYPE2<LambdaDotStar,      A1,const A2>::R operator()(      A1& a1, const A2& a2)const{return Eval(l1,a1,a2).*Eval(l2,a1,a2);}
	template<class A1, class A2> typename RET_TYPE2<LambdaDotStar,      A1,      A2>::R operator()(      A1& a1,       A2& a2)const{return Eval(l1,a1,a2).*Eval(l2,a1,a2);}
	
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaDotStar,const A1,const A2,const A3>::R operator()(const A1& a1, const A2& a2, const A3& a3)const{return Eval(l1,a1,a2,a3).*Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaDotStar,const A1,const A2,      A3>::R operator()(const A1& a1, const A2& a2,       A3& a3)const{return Eval(l1,a1,a2,a3).*Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaDotStar,const A1,      A2,const A3>::R operator()(const A1& a1,       A2& a2, const A3& a3)const{return Eval(l1,a1,a2,a3).*Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaDotStar,const A1,      A2,      A3>::R operator()(const A1& a1,       A2& a2,       A3& a3)const{return Eval(l1,a1,a2,a3).*Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaDotStar,      A1,const A2,const A3>::R operator()(      A1& a1, const A2& a2, const A3& a3)const{return Eval(l1,a1,a2,a3).*Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaDotStar,      A1,const A2,      A3>::R operator()(      A1& a1, const A2& a2,       A3& a3)const{return Eval(l1,a1,a2,a3).*Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaDotStar,      A1,      A2,const A3>::R operator()(      A1& a1,       A2& a2, const A3& a3)const{return Eval(l1,a1,a2,a3).*Eval(l2,a1,a2,a3);}
	template<class A1, class A2, class A3> typename RET_TYPE3<LambdaDotStar,      A1,      A2,      A3>::R operator()(      A1& a1,       A2& a2,       A3& a3)const{return Eval(l1,a1,a2,a3).*Eval(l2,a1,a2,a3);}

private:
	L1& l1;
	const L2& l2;
};

// bv == bind var
template<class PTR, class T>
LambdaDotStar<const T,PTR> bv(const PTR& ptr, const T& t){
	return LambdaDotStar<const T,PTR>(t,ptr);
}
template<class PTR, class T>
LambdaDotStar<T,PTR> bv(const PTR& ptr, T& t){
	return LambdaDotStar<T,PTR>(t,ptr);
}








// is lambda (before evaluation)
template<> struct IS_LAMBDA<Lambda1>{static const bool r = true;};
template<> struct IS_LAMBDA<Lambda2>{static const bool r = true;};
template<> struct IS_LAMBDA<Lambda3>{static const bool r = true;};
template<class A,class B> struct IS_LAMBDA<LambdaPlus<A,B> >{static const bool r = true;};
template<class A,class B> struct IS_LAMBDA<LambdaMinus<A,B> >{static const bool r = true;};
template<class A,class B> struct IS_LAMBDA<LambdaMultiplies<A,B> >{static const bool r = true;};
template<class A,class B> struct IS_LAMBDA<LambdaDivides<A,B> >{static const bool r = true;};
template<class A,class B> struct IS_LAMBDA<LambdaPlusAssign<A,B> >{static const bool r = true;};
// TODO
















// lambda return types

/*
template<class A1> struct RET_TYPE1<Lambda1,A1>{typedef A1 R;};

template<class A1, class A2> struct RET_TYPE2<Lambda1,A1,A2>{typedef A1 R;};
template<class A1, class A2> struct RET_TYPE2<Lambda2,A1,A2>{typedef A2 R;};
*/
template<class A1, class A2, class A3> struct RET_TYPE3<Lambda1,A1,A2,A3>{typedef A1& R;};
template<class A1, class A2, class A3> struct RET_TYPE3<Lambda2,A1,A2,A3>{typedef A2& R;};
template<class A1, class A2, class A3> struct RET_TYPE3<Lambda3,A1,A2,A3>{typedef A3& R;};




#ifdef _MSC_VER
	#define __typeof decltype
#endif



template<class L1, class L2, class A1, class A2, class A3> struct RET_TYPE3<LambdaPlus<L1,L2>,A1,A2,A3>{
	typedef __typeof(TYPE(RRC,typename RET_TYPE3<L1,A1,A2,A3>::R)() + TYPE(RRC,typename RET_TYPE3<L2,A1,A2,A3>::R)()) R;
};

template<class L1, class L2, class A1, class A2, class A3> struct RET_TYPE3<LambdaMinus<L1,L2>,A1,A2,A3>{
	typedef __typeof(TYPE(RRC,typename RET_TYPE3<L1,A1,A2,A3>::R)() - TYPE(RRC,typename RET_TYPE3<L2,A1,A2,A3>::R)()) R;
};

template<class L1, class A1, class A2, class A3> struct RET_TYPE3<LambdaUnaryMinus<L1>,A1,A2,A3>{
	typedef __typeof(- TYPE(RRC,typename RET_TYPE3<L1,A1,A2,A3>::R)()) R;
};

template<class L1, class L2, class A1, class A2, class A3> struct RET_TYPE3<LambdaMultiplies<L1,L2>,A1,A2,A3>{
	typedef __typeof(*(typename RRC<typename RET_TYPE3<L1,A1,A2,A3>::R>::R*)nullptr * *(typename RRC<typename RET_TYPE3<L2,A1,A2,A3>::R>::R*)nullptr) R;
};

template<class L1, class L2, class A1, class A2, class A3> struct RET_TYPE3<LambdaDivides<L1,L2>,A1,A2,A3>{
	typedef __typeof(typename RRC<typename RET_TYPE3<L1,A1,A2,A3>::R>::R() / typename RRC<typename RET_TYPE3<L2,A1,A2,A3>::R>::R()) R;
};



template<class L1, class L2, class A1, class A2, class A3> struct RET_TYPE3<LambdaPlusAssign<L1,L2>,A1,A2,A3>{
	typedef __typeof(*(new typename RRC<typename RET_TYPE3<L1,A1,A2,A3>::R>::R) += *(new typename RRC<typename RET_TYPE3<L2,A1,A2,A3>::R>::R))& R;
};




template<class L1, class L2, class A1, class A2, class A3> struct RET_TYPE3<LambdaLess<L1,L2>,A1,A2,A3>{
	typedef __typeof(*(new typename RRC<typename RET_TYPE3<L1,A1,A2,A3>::R>::R) < *(new typename RRC<typename RET_TYPE3<L2,A1,A2,A3>::R>::R)) R;
};
template<class L1, class L2, class A1, class A2, class A3> struct RET_TYPE3<LambdaGreater<L1,L2>,A1,A2,A3>{
	typedef __typeof(*(new typename RRC<typename RET_TYPE3<L1,A1,A2,A3>::R>::R) > *(new typename RRC<typename RET_TYPE3<L2,A1,A2,A3>::R>::R)) R;
};



template<class L1, class L2, class A1, class A2, class A3> struct RET_TYPE3<LambdaDotStar<L1,L2>,A1,A2,A3>{
	typedef __typeof(*(new typename RRC<typename RET_TYPE3<L1,A1,A2,A3>::R>::R) .* *(new typename RRC<typename RET_TYPE3<L2,A1,A2,A3>::R>::R)) R;
};






template<class F, class L1>
class LambdaBinder1{
public:
	LambdaBinder1(const F& _f, const L1& _l1) : f(_f), l1(_l1) {}
	
	template<class A1>
	int operator()(const A1& a1){
		return f(Eval(l1,a1));
	}
	template<class A1, class A2>
	int operator()(const A1& a1, const A2& a2){
		return f(Eval(l1,a1,a2));
	}
	template<class A1, class A2, class A3>
	int operator()(const A1& a1, const A2& a2, const A3& a3){
		return f(Eval(l1,a1,a2,a3));
	}
	
private:
	const F& f;
	const L1& l1;
};

template<typename F, class L1>
LambdaBinder1<F,L1> bf(const F& f, const L1& l1){
	return LambdaBinder1<F,L1>(f,l1);
}



template<class F, class L1, class L2>
class LambdaBinder2{
public:
	LambdaBinder2(const F& _f, const L1& _l1, const L2& _l2) : f(_f), l1(_l1), l2(_l2) {}
	
	template<class A1>
	int operator()(const A1& a1){
		return f(Eval(l1,a1),Eval(l2,a1));
	}
	template<class A1, class A2>
	int operator()(const A1& a1, const A2& a2){
		return f(Eval(l1,a1,a2),Eval(l2,a1,a2));
	}
	template<class A1, class A2, class A3>
	int operator()(const A1& a1, const A2& a2, const A3& a3){
		return f(Eval(l1,a1,a2,a3),Eval(l2,a1,a2,a3));
	}
	
private:
	const F& f;
	const L1& l1;
	const L2& l2;
};

template<typename F, class L1, class L2>
LambdaBinder2<F,L1,L2> bf(const F& f, const L1& l1, const L2& l2){
	return LambdaBinder2<F,L1,L2>(f,l1,l2);
}



template<class F, class L1, class L2, class L3>
class LambdaBinder3{
public:
	LambdaBinder3(const F& _f, const L1& _l1, const L2& _l2, const L3& _l3) : f(_f), l1(_l1), l2(_l2), l3(_l3) {}
	
	template<class A1>
	int operator()(const A1& a1){
		return f(Eval(l1,a1),Eval(l2,a1),Eval(l3,a1));
	}
	template<class A1, class A2>
	int operator()(const A1& a1, const A2& a2){
		return f(Eval(l1,a1,a2),Eval(l2,a1,a2),Eval(l3,a1,a2));
	}
	template<class A1, class A2, class A3>
	int operator()(const A1& a1, const A2& a2, const A3& a3){
		return f(Eval(l1,a1,a2,a3),Eval(l2,a1,a2,a3),Eval(l3,a1,a2,a3));
	}
	
private:
	const F& f;
	const L1& l1;
	const L2& l2;
	const L3& l3;
};

template<typename F, class L1, class L2, class L3>
LambdaBinder3<F,L1,L2,L3> bf(const F& f, const L1& l1, const L2& l2, const L3& l3){
	return LambdaBinder3<F,L1,L2,L3>(f,l1,l2,l3);
}












}



