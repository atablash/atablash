#pragma once

#include "triangle.hpp"
#include "segment.hpp"



namespace ab{


template<class T>
inline bool same_sign(const T& a, const T& b){
	return (a > 0) == (b > 0);
}


template<class F, class COORDS_F>
struct Triangle3RayResult{
	bool cut = false;
	F travelSteps;
	Vector3<F> point;
	Vector2<COORDS_F> coords;
};
// 2-sided
template<class F, class COORDS_F = F>
inline Triangle3RayResult<F, COORDS_F> intersection(
	const Triangle3<F>& t,
	const Vector3<F>& rayPos, const Vector3<F>& rayDir,
	const F shiftRayPos = 0, // measured in rayDir lengths
	const F eps = (F)1e-6
){
	Triangle3RayResult<F, COORDS_F> result;

	Vector3<F> normal = (t.b - t.a) ^ (t.c - t.a); // not normalized!
	
	// how far (times normal) does ray travel by going rayDir forward
	F dot_rayDir_normal = rayDir * normal;
	//if (dot_rayDir_normal >= 0) return; // wrong side of triangle

	if (fabs(dot_rayDir_normal) <= 1e-6) return result;


	F d = -(normal * t.a); // plane equation:   normal*some_point + d = 0

	// signed distance in terms of normal length
	F rayPosDist = rayPos*normal + d;

	if (dot_rayDir_normal < eps  &&  dot_rayDir_normal > -eps){ // parallel
		if (rayPosDist < eps && rayPosDist > -eps){
			result.cut = true;
			result.travelSteps = 0;
			result.point = rayPos;
		}
		return result;
	}


	result.travelSteps = rayPosDist / -dot_rayDir_normal;
	if (result.travelSteps < shiftRayPos){ // wrong way
		return result;
	}


	// project ray onto t's plane
	result.point = rayPos + rayDir * result.travelSteps;
	BarycentricCoords<F, COORDS_F> coords(t);
	result.coords = coords(result.point);

	if (result.coords.x < 0 || result.coords.y < 0 || result.coords.x + result.coords.y > 1){
		return result;
	}

	result.cut = true;
	return result;
}




template<class F>
struct Intersection_triangle_segment_result{
	bool cut = false;
	Vector3<F> point;
};
// real-time collision detection, page 186
// 2-sided
template<class F>
inline Intersection_triangle_segment_result<F> intersection(
	const Triangle3<F>& triangle,
	const Segment3<F>& segment
){
	Intersection_triangle_segment_result<F> result;

	auto ab = triangle.b - triangle.a;
	auto ac = triangle.c - triangle.a;
	auto qp = segment.fr - segment.to;

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	auto n = ab ^ ac;

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	F d = qp * n;
	if (d == 0) return result;
	
	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	auto ap = segment.fr - triangle.a;
	auto t = ap * n;
	if (t < 0) return result;
	if (t > d) return result; // For segment; exclude this code line for a ray test

	// Compute barycentric coordinate components and test if within bounds
	auto e = qp ^ ap;
	auto v = ac * e;
	if (v < 0.0f || v > d) return result;

	auto w = -(ab * e);
	if (w < 0.0f || v + w > d) return result;

	// Segment/ray intersects triangle. Perform delayed division and
	// compute the last barycentric coordinate component
	F ood = (F)1 / d;
	t *= ood; // not needed
	v *= ood;
	w *= ood;
	// auto u = (F)1 - v - w;

	result.cut = true;
	result.point = triangle.a + v*ab + w*ac;
	return result;
}






// ignores case when `point`'s projection is outside `triangle`
template<class F>
struct Intersection_triangle_point_result{
	F distSqr = std::numeric_limits<F>::max();
};
template<class F>
Intersection_triangle_point_result<F> intersection(const Triangle3<F>& triangle, const Vector3<F>& point){
	Intersection_triangle_point_result<F> result;

	auto BA = triangle.b - triangle.a;
	auto CA = triangle.c - triangle.a;
	auto normal = BA ^ CA;

	auto AP = triangle.a - point;
	auto BP = triangle.b - point;

	F pab = normal * (AP ^ BP);
	if (pab < 0) return result;

	auto CP = triangle.c - point;

	F pbc = normal * (BP ^ CP);
	if (pbc < 0) return result;

	F pca = normal * (CP ^ AP);
	if (pca < 0) return result;

	// optim todo: use lagrange identity: Real-Time Collision Detection, page 140

	auto R = (triangle.a * pbc + triangle.b * pca + triangle.c * pab) / (pab + pbc + pca);

	result.distSqr = (point - R).lengthSqr();
	return result;
}





}




