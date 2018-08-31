#pragma once

#include <atablash/linalg/3/vector.hpp>
#include <atablash/3d/aabb.hpp>

namespace ab{





//#include <atablash/3d/boolean-eps.hpp>






template<typename TFLOAT>
class Triangle3
{
public:
	Triangle3(const ab::Vector3<TFLOAT>& _a, const ab::Vector3<TFLOAT>& _b, const ab::Vector3<TFLOAT>& _c) : a(_a), b(_b), c(_c) {}
	ab::Vector3<TFLOAT> a, b, c;

	ab::Vector3<TFLOAT>& operator[](unsigned int idx)
	{
		ASS(0 <= idx && idx < 3);
		return *(&a + idx);
	}
	const ab::Vector3<TFLOAT>& operator[](unsigned int idx) const
	{
		ASS(0 <= idx && idx < 3);
		return *(&a + idx);
	}

	Vector3<TFLOAT> normalDir() const {
		return (b - a) ^ (c - a);
	}

	Vector3<TFLOAT> normal() const {
		return normalDir().normalized();
	}

	AABB<ab::Vector3<TFLOAT>> aabb()const{
		AABB<ab::Vector3<TFLOAT>> result;
		result.fr.x = std::min({ a.x, b.x, c.x });
		result.fr.y = std::min({ a.y, b.y, c.y });
		result.fr.z = std::min({ a.z, b.z, c.z });
		result.to.x = std::max({ a.x, b.x, c.x });
		result.to.y = std::max({ a.y, b.y, c.y });
		result.to.z = std::max({ a.z, b.z, c.z });
		return result;
	}

	bool isDegenerate(const TFLOAT& eps) const {
		auto BA = b - a;
		auto CA = c - a;

		if (BA.lengthSqr() == 0) return true;
		if (CA.lengthSqr() == 0) return true;

		CA.normalize();
		BA.normalize();

		// A, B, C on the same line
		if ((BA ^ CA).lengthSqr() < eps){
			TFLOAT x = (BA ^ CA).lengthSqr();
			return true;
		}
		return false;
	}
};








// http://www.blackpawn.com/texts/pointinpoly/
// takes F precision, gives only CF precision
template<class F, class CF = F>
class BarycentricCoords{
public:
	BarycentricCoords(const Triangle3<F>& t){
		vA = t.a;     
		vAC = t.c - t.a;
		vAB = t.b - t.a;

		dot_AB_AB = vAB * vAB;
		dot_AC_AC = vAC * vAC;
		dot_AB_AC = vAB * vAC;

		invDenom = (CF)1 / (dot_AC_AC * dot_AB_AB - dot_AB_AC * dot_AB_AC);
	}
	
	Vector2<CF> operator()(const Vector3<F>& p) const {
		Vector2<CF> result;
		Vector3<CF> vAP = p - vA; // (*)
		CF dot_AC_AP = vAC * vAP;
		CF dot_AB_AP = vAB * vAP;
		result.x = (dot_AC_AC * dot_AB_AP - dot_AB_AC * dot_AC_AP) * invDenom;
		result.y = (dot_AB_AB * dot_AC_AP - dot_AB_AC * dot_AB_AP) * invDenom;
		return result;
	}
	
private:
	Vector3<F> vA; // F precision because of subtraction (*)
	Vector3<CF> vAB, vAC;
	CF dot_AB_AB;
	CF dot_AC_AC;
	CF dot_AB_AC;
	CF invDenom;
};












enum class WherePoint : ui8 {inside,AB,BC,CA,A,B,C};


//
// returned by triangle intersection function
//
// cut: do triangles intersect?
template<class F>
struct CutInfo{
	bool cut = false;
	bool coplanar = false;
	struct PerTriangle {
		WherePoint U_where;
		WherePoint V_where;
		bool flipNormal;
	} t[2];
	Vector3<F> U; // todo: disable initialization to zero
	Vector3<F> V; // todo: disable initialization to zero
};







namespace detail{
		
		
	// UVProj
	// projects new vertices U and V onto triangles intersection line
	// U_proj < V_proj
	template<class F>
	struct UVProjResult{
		F U_proj;
		F V_proj;
		WherePoint U_where;
		WherePoint V_where;
		bool flipNormal = false;
	};
	template<class F, class EPS>
	inline UVProjResult<F> triangle3Intersection_UVProj(const int numAbove, const int numBelow,
		const F A_d, const F B_d, const F C_d,
		const F A_proj, const F B_proj, const F C_proj,
		const EPS& eps)
	{
		UVProjResult<F> result;
		// set cutInfo for the first triangle
		if(numAbove == 1 && numBelow == 2){ // standard situation - one vertex above
			if (A_d > eps.pointOnPlane){ // vertex A above
				result.U_proj = lerp(A_proj, B_proj, A_d/(A_d-B_d));
				result.U_where = WherePoint::AB;
				result.V_proj = lerp(A_proj, C_proj, A_d/(A_d-C_d));
				result.V_where = WherePoint::CA;
			}
			else if(B_d > eps.pointOnPlane){ // vertex B above
				result.U_proj = lerp(B_proj, C_proj, B_d/(B_d-C_d));
				result.U_where = WherePoint::BC;
				result.V_proj = lerp(B_proj, A_proj, B_d/(B_d-A_d));
				result.V_where = WherePoint::AB;
			}
			else if(C_d > eps.pointOnPlane){ // vertex C above
				result.U_proj = lerp(C_proj, A_proj, C_d/(C_d-A_d));
				result.U_where = WherePoint::CA;
				result.V_proj = lerp(C_proj, B_proj, C_d/(C_d-B_d));
				result.V_where = WherePoint::BC;
			}
		}
		else if(numAbove == 2 && numBelow == 1){ // standard situation - one vertex below
			if(A_d < -eps.pointOnPlane){ // vertex A below
				result.V_proj = lerp(A_proj, B_proj, A_d/(A_d-B_d));
				result.V_where = WherePoint::AB;
				result.U_proj = lerp(A_proj, C_proj, A_d/(A_d-C_d));
				result.U_where = WherePoint::CA;
			}
			else if(B_d < -eps.pointOnPlane){ // vertex B below
				result.V_proj = lerp(B_proj, C_proj, B_d/(B_d-C_d));
				result.V_where = WherePoint::BC;
				result.U_proj = lerp(B_proj, A_proj, B_d/(B_d-A_d));
				result.U_where = WherePoint::AB;
			}
			else if(C_d < -eps.pointOnPlane){ // vertex C below
				result.V_proj = lerp(C_proj, A_proj, C_d/(C_d-A_d));
				result.V_where = WherePoint::CA;
				result.U_proj = lerp(C_proj, B_proj, C_d/(C_d-B_d));
				result.U_where = WherePoint::BC;
			}
		}
		else if(numAbove == 1 && numBelow == 1){
			if(A_d >= -eps.pointOnPlane && A_d <= eps.pointOnPlane){ // vertex A on plane
				result.U_proj = A_proj;
				result.U_where = WherePoint::A;
				result.V_proj = lerp(B_proj, C_proj, B_d/(B_d-C_d));
				result.V_where = WherePoint::BC;
				result.flipNormal = B_d > 0;
			}
			else if(B_d >= -eps.pointOnPlane && B_d <= eps.pointOnPlane){ // vertex B on plane
				result.U_proj = B_proj;
				result.U_where = WherePoint::B;
				result.V_proj = lerp(A_proj, C_proj, A_d/(A_d-C_d));
				result.V_where = WherePoint::CA;
				result.flipNormal = C_d > 0;
			}
			else if(C_d >= -eps.pointOnPlane && C_d <= eps.pointOnPlane){ // vertex C on plane
				result.U_proj = C_proj;
				result.U_where = WherePoint::C;
				result.V_proj = lerp(B_proj, A_proj, B_d/(B_d-A_d));
				result.V_where = WherePoint::AB;
				result.flipNormal = A_d > 0;
			}
		}
		else if(numAbove == 1){
			ASS(numBelow == 0);
			if(A_d > eps.pointOnPlane){
				result.U_proj = B_proj;
				result.U_where = WherePoint::B;
				result.V_proj = C_proj;
				result.V_where = WherePoint::C;
			}
			else if(B_d > eps.pointOnPlane){
				result.U_proj = C_proj;
				result.U_where = WherePoint::C;
				result.V_proj = A_proj;
				result.V_where = WherePoint::A;
			}
			else if(C_d > eps.pointOnPlane){
				result.U_proj = A_proj;
				result.U_where = WherePoint::A;
				result.V_proj = B_proj;
				result.V_where = WherePoint::B;
			}
		}
		else if(numBelow == 1){
			ASS(numAbove == 0);
			if(A_d < -eps.pointOnPlane){
				result.V_proj = B_proj;
				result.V_where = WherePoint::B;
				result.U_proj = C_proj;
				result.U_where = WherePoint::C;
			}
			else if(B_d < -eps.pointOnPlane){
				result.V_proj = C_proj;
				result.V_where = WherePoint::C;
				result.U_proj = A_proj;
				result.U_where = WherePoint::A;
			}
			else if(C_d < -eps.pointOnPlane){
				result.V_proj = A_proj;
				result.V_where = WherePoint::A;
				result.U_proj = B_proj;
				result.U_where = WherePoint::B;
			}
		}
		else{
			LOGE("WTF?");
		}
		
		if(result.U_proj > result.V_proj){
			std::swap(result.U_proj, result.V_proj);
			std::swap(result.U_where, result.V_where);
			result.flipNormal = !result.flipNormal;
		}
		
		return result;
	}
	
	
	template<class F>
	Vector3<F> triangle3Intersection_ComputePoint(const WherePoint wherePoint, const Triangle3<F>& t, const F A_d, const F B_d, const F C_d){
		ASS(wherePoint != WherePoint::inside);
		switch(wherePoint){
			case WherePoint::AB:
				return lerp(t.a, t.b, A_d/(A_d-B_d));
			case WherePoint::BC:
				return lerp(t.b, t.c, B_d/(B_d-C_d));
			case WherePoint::CA:
				return lerp(t.c, t.a, C_d/(C_d-A_d));
			case WherePoint::A:
				return t.a;
			case WherePoint::B:
				return t.b;
			case WherePoint::C:
				return t.c;
		}
		ASS(false);
		return Vector3<F>();
	}
	
	// template version because msvc doesn't support constexpr
	template<WherePoint a, WherePoint b> struct WPB_PACK{
		static const uint r = (ui8(a) - 1) * 6 + (ui8(b) - 1);
	};

	inline uint wpb_pack(WherePoint a, WherePoint b) {
		return (ui8(a) - 1) * 6 + (ui8(b) - 1);
	}

	inline WherePoint wherePointBetween(WherePoint a, WherePoint b){
		if (a == WherePoint::inside || b == WherePoint::inside) return WherePoint::inside; // almost always

		ASS(!(a == WherePoint::A && b == WherePoint::A));
		ASS(!(a == WherePoint::B && b == WherePoint::B));
		ASS(!(a == WherePoint::C && b == WherePoint::C));

		switch (wpb_pack(a, b)){
		case WPB_PACK<WherePoint::AB, WherePoint::AB>::r:
		case WPB_PACK<WherePoint::AB, WherePoint::A>::r:
		case WPB_PACK<WherePoint::AB, WherePoint::B>::r:
			return WherePoint::AB;
		case WPB_PACK<WherePoint::BC, WherePoint::BC>::r:
		case WPB_PACK<WherePoint::BC, WherePoint::B>::r:
		case WPB_PACK<WherePoint::BC, WherePoint::C>::r:
			return WherePoint::BC;
		case WPB_PACK<WherePoint::CA, WherePoint::CA>::r:
		case WPB_PACK<WherePoint::CA, WherePoint::A>::r:
		case WPB_PACK<WherePoint::CA, WherePoint::C>::r:
			return WherePoint::CA;

		case WPB_PACK<WherePoint::A, WherePoint::AB>::r:
			return WherePoint::AB;
		case WPB_PACK<WherePoint::A, WherePoint::CA>::r:
			return WherePoint::CA;
		case WPB_PACK<WherePoint::A, WherePoint::B>::r:
			return WherePoint::AB;
		case WPB_PACK<WherePoint::A, WherePoint::C>::r:
			return WherePoint::CA;

		case WPB_PACK<WherePoint::B, WherePoint::AB>::r:
			return WherePoint::AB;
		case WPB_PACK<WherePoint::B, WherePoint::BC>::r:
			return WherePoint::CA;
		case WPB_PACK<WherePoint::B, WherePoint::A>::r:
			return WherePoint::AB;
		case WPB_PACK<WherePoint::B, WherePoint::C>::r:
			return WherePoint::BC;

		case WPB_PACK<WherePoint::C, WherePoint::BC>::r:
			return WherePoint::BC;
		case WPB_PACK<WherePoint::C, WherePoint::CA>::r:
			return WherePoint::CA;
		case WPB_PACK<WherePoint::C, WherePoint::A>::r:
			return WherePoint::CA;
		case WPB_PACK<WherePoint::C, WherePoint::B>::r:
			return WherePoint::BC;
		}
		return WherePoint::inside;
	}
	
	
} // namespace detail






// fast triangle-triangle intersection
// http://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf // almost
// does not support degenerate triangles!
template<class F, class EPS>
inline CutInfo<F> triangle3Intersection(const Triangle3<F>& t1, const Triangle3<F>& t2,
	const EPS& eps = EPS()){
	CutInfo<F> cutInfo;

	const F epsPointOnPlaneSmall = eps.pointOnPlane * eps.pointOnPlaneSmallMult;

	// todo: this is really slow!
	//if (t1.isDegenerate() || t2.isDegenerate()){
	//	return cutInfo;
	//}


	// TODO: put these two in one function:
	
	
	// triangle 2 normal
	Vector3<F> n2 = (t2.b - t2.a) ^ (t2.c - t2.a);	// not normalized!
	n2.normalize(); // TODO: optim!
	
	//if (n2.LengthSqr() < degenEps)
	//	return false;
	
	F d2 = -(n2 * t2.a); // d2 from plane equation

	// first triangle's vertices distance from the second triangle's plane
	F d1a = n2*t1.a + d2;
	F d1b = n2*t1.b + d2;
	F d1c = n2*t1.c + d2;
	
	int numAbove1 = 0;
	int numBelow1 = 0;
	
	if (d1a < -eps.pointOnPlane) ++numBelow1;
	else if (d1a > eps.pointOnPlane) ++numAbove1;
	if (d1b < -eps.pointOnPlane) ++numBelow1;
	else if (d1b > eps.pointOnPlane) ++numAbove1;
	if (d1c < -eps.pointOnPlane) ++numBelow1;
	else if (d1c > eps.pointOnPlane) ++numAbove1;
	
	// all above / all below / point intersection
	if (numAbove1 == 3 || numBelow1 == 3 || (numAbove1 == 2 && numBelow1 == 0) || (numAbove1 == 0 && numBelow1 == 2)){
		// cutInfo.cut = false;
		return cutInfo;
	}

	if (numAbove1 == 0 && numBelow1 == 0){
		// LOGD("Unsupported coplanar triangles!");
		// cutInfo.cut = false;
		cutInfo.cut = true;
		cutInfo.coplanar = true;
		return cutInfo;
	}
	
	// fix - much smaller epsilons to avoid collisions of distant almost coplanar triangles
	if (numAbove1 == 0){
		ASS(numBelow1 == 1);
		if (d1a < -epsPointOnPlaneSmall && d1b < -epsPointOnPlaneSmall && d1c < -epsPointOnPlaneSmall){
			return cutInfo;
		}
	}
	
	
	
	
	
	
	// triangle 1 normal
	Vector3<F> n1 = (t1.b - t1.a) ^ (t1.c - t1.a);	// not normalized!
	n1.normalize(); // TODO: optim

	F d1 = -n1 * t1.a;	// d1 from plane equation

	// odleglosci drugiego od plaszczyzny pierwszego
	F d2a = n1*t2.a + d1;
	F d2b = n1*t2.b + d1;
	F d2c = n1*t2.c + d1;
	
	int numAbove2 = 0;
	int numBelow2 = 0;
	
	if (d2a < -eps.pointOnPlane) ++numBelow2;
	else if (d2a > eps.pointOnPlane) ++numAbove2;
	if (d2b < -eps.pointOnPlane) ++numBelow2;
	else if (d2b > eps.pointOnPlane) ++numAbove2;
	if (d2c < -eps.pointOnPlane) ++numBelow2;
	else if (d2c > eps.pointOnPlane) ++numAbove2;
	if (numAbove2 == 3 || numBelow2 == 3 || (numAbove2 == 2 && numBelow2 == 0) || (numAbove2 == 0 && numBelow2 == 2)){
		// cutInfo.cut = false;
		return cutInfo;
	}

	if (numAbove2 == 0 && numBelow2 == 0){
		// LOGD("Unsupported overlapping triangles!");
		// cutInfo.cut = false;
		cutInfo.cut = true;
		cutInfo.coplanar = true;
		return cutInfo;
	}

	// fix - much smaller epsilons to avoid collisions of distant almost coplanar triangles
	if (numAbove2 == 0){
		ASS(numBelow2 == 1);
		if (d2a < -epsPointOnPlaneSmall && d2b < -epsPointOnPlaneSmall && d2c < -epsPointOnPlaneSmall){
			return cutInfo;
		}
	}
	
	
	
	
	



	// prosta L przeciecia plaszczyzn trojkatow (jej kierunek)
	Vector3<F> D = n1 ^ n2;	// TODO: mozna sprobowac nie liczyc tego, tylko bezposrednio porownac, wzdluz ktorych osi sa n1 i n2
	// todo optim: project onto largest axis, not onto D
	
	
	auto t1r = detail::triangle3Intersection_UVProj(numAbove1, numBelow1, d1a, d1b, d1c, D*t1.a, D*t1.b, D*t1.c, eps);
	auto t2r = detail::triangle3Intersection_UVProj(numAbove2, numBelow2, d2a, d2b, d2c, D*t2.a, D*t2.b, D*t2.c, eps);
	
	// sorry, no cut after all
	// eps helps to cancel point collitions
	if (t1r.V_proj <= t2r.U_proj + eps.pointCollision || t2r.V_proj <= t1r.U_proj + eps.pointCollision){
		return cutInfo;
	}
	
	
	
	
	// we have a cut
	// now U and V map to interval on D that intersect triangle t1 or t2
	cutInfo.t[0].U_where = t1r.U_where;
	cutInfo.t[0].V_where = t1r.V_where;
	cutInfo.t[0].flipNormal = t1r.flipNormal;
	cutInfo.t[1].U_where = t2r.U_where;
	cutInfo.t[1].V_where = t2r.V_where;
	cutInfo.t[1].flipNormal = t2r.flipNormal;
	
	
	
	// shift U and V to map to interval on D that intersects both triangles
	if (t1r.U_proj + eps.contourWeld < t2r.U_proj){
		cutInfo.t[0].U_where = detail::wherePointBetween(cutInfo.t[0].U_where, cutInfo.t[0].V_where);
		cutInfo.U = detail::triangle3Intersection_ComputePoint(cutInfo.t[1].U_where, t2, d2a, d2b, d2c);
	}
	else if (t2r.U_proj + eps.contourWeld < t1r.U_proj){
		cutInfo.t[1].U_where = detail::wherePointBetween(cutInfo.t[1].U_where, cutInfo.t[1].V_where);
		cutInfo.U = detail::triangle3Intersection_ComputePoint(cutInfo.t[0].U_where, t1, d1a, d1b, d1c);
	}
	else{
		auto cand1 = detail::triangle3Intersection_ComputePoint(cutInfo.t[0].U_where, t1, d1a, d1b, d1c);
		auto cand2 = detail::triangle3Intersection_ComputePoint(cutInfo.t[1].U_where, t2, d2a, d2b, d2c);
		cutInfo.U = (cand1 + cand2) * (F)0.5; // average candidates
		// todo: maybe select winner? maybe favor WherePoint::A,B,C
	}
	
	

	// shift U and V to map to interval on D that intersects both triangles
	if (t2r.V_proj + eps.contourWeld < t1r.V_proj){
		cutInfo.t[0].V_where = detail::wherePointBetween(cutInfo.t[0].U_where, cutInfo.t[0].V_where);
		cutInfo.V = detail::triangle3Intersection_ComputePoint(cutInfo.t[1].V_where, t2, d2a, d2b, d2c);
	}
	else if (t1r.V_proj + eps.contourWeld < t2r.V_proj){
		cutInfo.t[1].V_where = detail::wherePointBetween(cutInfo.t[1].U_where, cutInfo.t[1].V_where);
		cutInfo.V = detail::triangle3Intersection_ComputePoint(cutInfo.t[0].V_where, t1, d1a, d1b, d1c);
	}
	else{
		auto cand1 = detail::triangle3Intersection_ComputePoint(cutInfo.t[0].V_where, t1, d1a, d1b, d1c);
		auto cand2 = detail::triangle3Intersection_ComputePoint(cutInfo.t[1].V_where, t2, d2a, d2b, d2c);
		cutInfo.V = (cand1 + cand2) * (F)0.5; // average candidates
		// todo: maybe select winner? maybe favor WherePoint::A,B,C
	}
	
	
	
	cutInfo.cut = true;
	return cutInfo;
}







}






// to_string
namespace std{
	template<class F>
	string to_string(const ab::Triangle3<F>& t){
		return "{" + to_string(t.a) + ", " + to_string(t.b) + ", " + to_string(t.c) + "}";
	}
}





