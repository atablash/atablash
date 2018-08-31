#pragma once

//#include<type_traits>

namespace ab
{


template<bool cond, typename T, typename F> struct IF {typedef T R;};
template<typename T, typename F> struct IF<false,T,F> {typedef F R;};

template <bool B, class T = void> struct ENABLE_IF {typedef T R;};
template <class T> struct ENABLE_IF<false, T> {};

template <bool B, class T = void> struct DISABLE_IF {typedef T R;};
template <class T> struct DISABLE_IF<true, T> {};



template<class A, class B> struct SAME{ static const bool r = false;};
template<typename T> struct SAME<T,T>{	static const bool r = true;};


template<typename T>
struct IS_CLASS{
	template<typename C> static char (&f(int C::*))[1];
	template<typename C> static char (&f(...))[2];

	static bool const r = sizeof(f<T>(0))==sizeof(char);
};




// remove reference
template<class T> struct RR{typedef T R;};
template<class T> struct RR<T&>{typedef T R;};

//remove const
template<class T> struct RC{typedef T R;};
template<class T> struct RC<const T>{typedef T R;};

// remove const and reference
template<class T> struct RRC{typedef typename RC<typename RR<T>::R>::R R;};


#define TYPE(T,...) typename T<__VA_ARGS__>::R





struct TRU{static const bool r = true;  typedef TRU R;};
struct FAL{static const bool r = false; typedef FAL R;};



// SFINAE test
#define HAS_MEMBER(member_alias, member)																									\
template<typename __T> struct HAS_CLASS_##member_alias {																											\
    struct Fallback { void member(); };																											\
    struct Derived : __T, Fallback { };																											\
																											\
    template<typename C, C> struct ChT;																											\
																											\
    template<typename C> static char (&f(ChT<void (Fallback::*)(), &C::member>*))[1];													\
    template<typename C> static char (&f(...))[2];																								\
																											\
    static const bool r = sizeof(f<Derived>(0)) == 2;																						\
};																										\
template<typename __T>																											\
struct HAS_##member_alias																											\
{																											\
	typedef typename IF<IS_CLASS<__T>::r, HAS_CLASS_##member_alias<__T>, FAL>::R R;														\
	static const bool r = R::r;																									\
};



#pragma once


// Dzia³a dok³adnie, uwzglêdnia typ (const, referencje) wyniku i argumentów, uwzglêdnia const membera
// Przyk³ad dla non-static klasy A:
// typedef int (A::*sign1)(unsigned short&); 
// Przyk³ad dla static dowolnej klasy:
// typedef int (*sign1)(unsigned short&); 
#define HAS_MEMBER_SIG(member_name, member_signature)																			\
template<typename CLS>																							\
struct HAS_SIG_##member_name																															\
{																									\
    template<typename C, C> struct ChT;																													\
	template<typename C> static char (&f(ChT<member_signature, &C::member_name>*))[1];																\
	template<typename C> static char (&f(...))[2];																									\
	static const bool r = sizeof(f<CLS>(0)) == sizeof(char);																							\
};




HAS_MEMBER(OPERATOR_CALL, operator())




/*
// get &T::x or &T::first
template<class T,class=void>struct GET_X{};
template<class T>struct GET_X<T,typename std::enable_if<std::is_object<decltype(T::x)>::value>::type>{typedef decltype(&T::x) R;};
template<class T>struct GET_X<T,typename std::enable_if<std::is_object<decltype(T::first)>::value>::type>{typedef decltype(&T::first) R;};

// get &T::y or &T::second
template<class T,class=void>struct GET_Y{};
template<class T>struct GET_Y<T,typename std::enable_if<std::is_object<decltype(T::y)>::value>::type>{typedef decltype(&T::y) R;};
template<class T>struct GET_Y<T,typename std::enable_if<std::is_object<decltype(T::second)>::value>::type>{typedef decltype(&T::second) R;};
*/

}






#include<utility>

namespace ab{

	struct Identity{
		template<class TT>
		TT operator()(TT&& x)const {
			return std::forward<TT>(x);
		}
		template<class TT>
		TT operator[](TT&& x) const {
			return std::forward<TT>(x);
		}
	};

}



