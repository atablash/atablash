#pragma once



#include "triangle-collision.hpp"


namespace ab{




template<class F>
struct RaycastResult{
	// typedef typename TRIMESH::POS_FLOAT F;
	ui32 poly = (ui32)(-1); // ui32(-1) if no hit
	F travelSteps = std::numeric_limits<F>::max();
	Vector3<F> point;
};

// brute force
// O(num_triangles)
// todo: wersja rozstrzygajaca remisy
template<class TRIMESH>
RaycastResult<typename TRIMESH::POS_FLOAT> raycast(const TRIMESH& mesh,
	const Vector3<typename TRIMESH::POS_FLOAT>& rayPos,
	const Vector3<typename TRIMESH::POS_FLOAT>& rayDir,
	const typename TRIMESH::POS_FLOAT shiftPos = 0,
	const typename TRIMESH::POS_FLOAT eps = 1e-6
){
	RaycastResult<typename TRIMESH::POS_FLOAT> result;
	result.travelSteps = std::numeric_limits<typename TRIMESH::POS_FLOAT>::max();
	result.poly = (ui32)-1;

	for (uint i = 0; i < mesh.ps().size(); ++i){
		auto t = mesh.ps(i).triangle();
		auto r = intersection(t, rayPos, rayDir, shiftPos);
		if (!r.cut) continue;

		if (r.travelSteps < result.travelSteps){
			result.travelSteps = r.travelSteps;
			result.poly = i;
			result.point = r.point;
		}
	}

	return result;
}







}


