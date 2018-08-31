#pragma once


#include <atablash/linalg/3/matrix.hpp>


namespace ab{


template<class F>
struct Segment3{
	Vector3<F> fr, to;

	AABB<ab::Vector3<F>> aabb() const {
		AABB<ab::Vector3<F>> aabb;
		aabb.fr = fr;
		aabb.to = to;
		if (aabb.fr[0] > aabb.to[0]) std::swap(aabb.fr[0], aabb.to[0]);
		if (aabb.fr[1] > aabb.to[1]) std::swap(aabb.fr[1], aabb.to[1]);
		if (aabb.fr[2] > aabb.to[2]) std::swap(aabb.fr[2], aabb.to[2]);
		return aabb;
	}
};






template<class F>
struct Intersection_segment_point_result{
	F distSqr;
};
template<class F>
Intersection_segment_point_result<F> intersection(const Segment3<F>& segment, const Vector3<F>& point){
	ASS(segment.to != segment.fr);
	auto BA = segment.to - segment.fr;
	auto PA = point - segment.fr;
	auto BA_scale = BA*PA / BA.lengthSqr();
	Intersection_segment_point_result<F> result;
	if(BA_scale < 0 || BA_scale > 1){
		result.distSqr = std::numeric_limits<F>::max();
	}
	else{
		auto v = PA - BA * BA_scale;
		result.distSqr = v.lengthSqr();
	}
	return result;
}




template<class F>
struct Intersection_segment_result{
	F distSqr = std::numeric_limits<F>::max();
	Vector3<F> point;
};
template<class F>
Intersection_segment_result<F> intersection(const Segment3<F>& a, const Segment3<F>& b){
	Intersection_segment_result<F> result;
	auto BA3 = a.to - a.fr;
	auto DC3 = b.to - b.fr;
	auto m = Matrix23<F>::Rows(BA3,DC3);
	auto A = m * a.fr;
	auto B = m * a.to;
	auto C = m * b.fr;
	auto D = m * b.to;
	auto BA = B - A;
	auto DC = D - C;
	auto CA = C - A;
	auto BC = B - C;
	auto DA = D - A;
	auto den = det(BA,DC);
	if(den == 0) return result;
	
	auto alpha = det(CA,DA) / den;
	if(alpha < 0 || alpha > 1) return result;
	
	auto beta = det(CA,BC) / den;
	if(beta < 0 || beta > 1) return result;
	
	auto p0 = a.fr + BA3 * alpha;
	auto p1 = b.fr + DC3 * beta;
	result.distSqr = (p1-p0).lengthSqr();
	result.point = (p0+p1)*(F)0.5;
	return result;
}



}


