#pragma once

namespace ab
{

	template<typename T1, typename T2>
	struct Tuple2
	{
		inline Tuple2(const T1& _v1, const T2& _v2) : v1(_v1), v2(_v2) {}
		T1 v1;
		T2 v2;
	};

	template<typename T1, typename T2, typename T3>
	struct Tuple3
	{
		inline Tuple3(const T1& _v1, const T2& _v2, const T3& _v3) : v1(_v1), v2(_v2), v3(_v3) {}
		T1 v1;
		T2 v2;
		T3 v3;
	};

	template<typename T1, typename T2>
	inline Tuple2<T1,T2> MakeTuple(const T1& v1, const T2& v2)
	{
		return Tuple2<T1,T2>(v1,v2);
	}

	template<typename T1, typename T2, typename T3>
	inline Tuple3<T1,T2,T3> MakeTuple(const T1& v1, const T2& v2, const T3& v3)
	{
		return Tuple3<T1,T2,T3>(v1,v2,v3);
	}

}
