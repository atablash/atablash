﻿

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
    static bool const RET = sizeof(f<Derived>(0)) == 2;																						\
};																										\
template<typename __T>																											\
struct HAS_##member_alias																											\
{																											\
	typedef typename IF<IS_CLASS<__T>::RET, HAS_CLASS_##member_alias<__T>, FALSE>::RET _r;														\
	static const bool RET = _r::RET;																									\
};



#pragma once


// Działa dokładnie, uwzględnia typ (const, referencje) wyniku i argumentów, uwzględnia const membera
// Przykład dla non-static klasy A:
// typedef int (A::*sign1)(unsigned short&); 
// Przykład dla static dowolnej klasy:
// typedef int (*sign1)(unsigned short&); 
#define HAS_MEMBER_SIGNATURE(member_name, member_signature)																			\
template<typename CLS>																							\
struct HAS_SIG_##member_name																															\
{																									\
    template<typename C, C> struct ChT;																													\
	template<typename C> static char (&f(ChT<member_signature,&C::member_name>*))[1];																\
	template<typename C> static char (&f(...))[2];																									\
	static const bool RET = sizeof(f<CLS>(0)) == sizeof(char);																							\
};


HAS_MEMBER(OPERATOR_STAR, operator*);
HAS_MEMBER(OPERATOR_ARROW,operator->);
HAS_MEMBER(ERASE,erase);


template<typename container, typename iterator>
inline void containerErase(container& cont, iterator& itr, typename ENABLE_IF<HAS_OPERATOR_STAR<iterator>::RET && HAS_OPERATOR_ARROW<iterator>::RET /*&& HAS_ERASE<container>::RET*/ >::RET* = 0)
{
	cont.erase(itr);
}


template<typename container>
inline void containerErase(container& cont, unsigned int key)
{
	cont.erase(key);
}


