#pragma once


#include <atablash/linalg/2/vector.hpp>
#include <atablash/linalg/3/vector.hpp>
#include <atablash/linalg/4/vector.hpp>

#include <atablash/patterns/ecs.hpp>

#include <type_traits>
#include <atablash/templates.hpp>


#include "trimesh-common.hpp"


namespace ab{





template<VExtra vextra> struct GET_V_PART{};
template<> struct GET_V_PART<VExtra::RGB>{ab::Vector3<float> rgb;};
template<> struct GET_V_PART<VExtra::ALPHA>{float alpha;};
template<> struct GET_V_PART<VExtra::UV>{ab::Vector2<float> uv;};
template<> struct GET_V_PART<VExtra::NORMAL>{ab::Vector3<float> normal;};








#ifdef __GNUC__

inline constexpr VExtra operator|(const VExtra& lhs, const VExtra& rhs){
	return VExtra((int)lhs | (int)rhs);
}

inline constexpr VExtra operator^(const VExtra& lhs, const VExtra& rhs){
	return VExtra((int)lhs ^ (int)rhs);
}

#endif





// 0001010110 -> 0001000000
template<int x> struct FIRST_BIT{static const int r = FIRST_BIT<(x>>1)>::r << 1;};
template<> struct FIRST_BIT<1>{static const int r=1;};



// GET_V_TYPE
template<int dimensions, VExtra vextras, class posPrecision = float>
struct GET_V_TYPE : GET_V_TYPE<dimensions,VExtra((int)vextras^FIRST_BIT<(int)vextras>::r),posPrecision>, GET_V_PART<(VExtra)FIRST_BIT<(int)vextras>::r> {
};

template<class posPrecision> struct GET_V_TYPE<2,(VExtra)0,posPrecision>{ab::Vector2<posPrecision> pos;};
template<class posPrecision> struct GET_V_TYPE<3,(VExtra)0,posPrecision>{ab::Vector3<posPrecision> pos;};

HAS_MEMBER(UV, uv)
HAS_MEMBER(RGB, rgb)
HAS_MEMBER(NORMAL, normal)
HAS_MEMBER(TANGENT_U, tangentU)

// static version
template<class _V>
class TriMesh : public Entity {
public:
	typedef _V V;
	typedef struct {
		ui32 is[3];
		ui32& operator[](int i){ return is[i]; }
		const ui32& operator[](int i)const{ return is[i]; }
	} P; // ui16 indices?

	TriMesh() = default;
	TriMesh(const TriMesh&) = delete;

public:

	// sfinae offsets
	// uv
	template<class T=int> typename std::enable_if<HAS_UV<V>::r,T>::type uvOffset(){
		V v;
		return (ui8*)&v.uv - (ui8*)&v;
	}
	template<class T=int> typename std::enable_if<!HAS_UV<V>::r,T>::type uvOffset(){
		return 0;
	}

	// rgb
	template<class T=int> typename std::enable_if<HAS_RGB<V>::r,T>::type rgbOffset(){
		V v;
		return (ui8*)&v.rgb - (ui8*)&v;
	}
	template<class T = int> typename std::enable_if<!HAS_RGB<V>::r, T>::type rgbOffset(){
		return 0;
	}

	// normal
	template<class T = int> typename std::enable_if<HAS_NORMAL<V>::r, T>::type normalOffset(){
		V v;
		return (ui8*)&v.normal - (ui8*)&v;
	}
	template<class T = int> typename std::enable_if<!HAS_NORMAL<V>::r, T>::type normalOffset(){
		return 0;
	}

	// tangentU
	template<class T = int> typename std::enable_if<HAS_TANGENT_U<V>::r, T>::type tangentUOffset(){
		V v;
		return (ui8*)&v.tangentU - (ui8*)&v;
	}
	template<class T = int> typename std::enable_if<!HAS_TANGENT_U<V>::r, T>::type tangentUOffset(){
		return 0;
	}

	int sizeofV(){
		return sizeof(V);
	}
	
public:
	std::vector<V> vs;
	std::vector<P> ps;
};







} // namespace ab















