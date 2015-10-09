#pragma once

// C++14 introduces tuples!

namespace ab{


////////////////////////////////////////////////////////////////////////////////
// Tuples
template<typename T1, typename T2 = void, typename T3 = void, typename T4 = void>
struct tuple
{
	inline tuple(const T1& _e1, const T2& _e2, const T3& _e3, const T4& _e4) :
		first(_e1),second(_e2),third(_e3),fourth(_e4){}
	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
};

template<typename T1, typename T2, typename T3>
struct tuple<T1,T2,T3,void>
{
	inline tuple(const T1& _e1, const T2& _e2, const T3& _e3) :
		first(_e1),second(_e2),third(_e3){}
	T1 first;
	T2 second;
	T3 third;
};

template<typename T1, typename T2, typename T3, typename T4>
tuple<T1,T2,T3,T4> make_tuple(const T1& _e1, const T2& _e2, const T3& _e3, const T4& _e4)
{
	return tuple<T1,T2,T3,T4>(_e1,_e2,_e3,_e4);
}

template<typename T1, typename T2, typename T3>
tuple<T1,T2,T3> make_tuple(const T1& _e1, const T2& _e2, const T3& _e3)
{
	return tuple<T1,T2,T3>(_e1,_e2,_e3);
}
////////////////////////////////////////////////////////////////////////////////

}

