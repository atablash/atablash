#pragma once



#include "solid.hpp"
#include "triangle.hpp"

#include "raycast.hpp"


#include <atablash/debug.hpp>

#include <atablash/3d/boolean-eps.hpp>



namespace ab{








// where is V in triangle cut at U?
//
// WherePoint::AB = triangle ABU
// WherePoint::BC = triangle BCU
// WherePoint::CA = triangle CAU
// WherePoint::A  = segment AU
// WherePoint::B  = segment BU
// WherePoint::C  = segment CU
//
template<class F>
inline WherePoint barycentricTricutOrient(const Vector2<F>& U, const Vector2<F>& V,
	// for `float`: 1e-6 not working on simple rotated boxes
	// 1e-5 ok
	const F eps = EpsBoolean<F>().tricutOrient
){
	if((U-V).lengthSqr() < 1e-12){
		LOGD("barycentricTricutOrient: U and V (almost?) the same! " + std::to_string(U) + " vs " + std::to_string(V));
	}

	auto det_UV = det(U,V);
	auto det_UV_x = (U.x-1)*V.y - U.y * (V.x-1);
	auto det_UV_y = U.x*(V.y-1) - (U.y-1)*V.x;
	if(det_UV < -eps){ // below AU
		if(det_UV_x > eps){ // below BU (so in ABU
			return WherePoint::AB;
		}
		else if(det_UV_x < -eps){ // above BU (so in BCU)
			return WherePoint::BC;
		}
		else{ // on BU
			return WherePoint::B;
		}
	}
	else if(det_UV > eps){ // above AU
		if(det_UV_y > eps){ // to the right of CU
			return WherePoint::BC;
		}
		else if(det_UV_y < -eps){ // to the left of CU
			return WherePoint::CA;
		}
		else{ // on CU
			return WherePoint::C;
		}
	}
	else{ // on AU or in BCU
		if(det_UV_x > 0){ // below BU (on AU)
			return WherePoint::A;
		}
		else{ // in BCU
			return WherePoint::BC;
		}
	}
}




template<bool tear = true, class TRIMESH>
inline void edgeMark_2sided(TRIMESH& mesh, ui32 edge, bool flipCutNormal){
	ASS(mesh.isUsingELinks());

	auto rev = mesh.elinks[edge];
	mesh.eflags[edge] |= flipCutNormal ? 1 : 2; // mark outside
	//if (mesh.eflags[edge] == 3) __debugbreak();
	if (rev != TRIMESH::IIDX_INT(-1)){
		// tear
		if (tear){
			mesh.elinks[edge] = (TRIMESH::IIDX_INT)(-1);
			mesh.elinks[rev] = (TRIMESH::IIDX_INT)(-1);
		}

		mesh.eflags[rev] |= flipCutNormal ? 2 : 1; // mark inside
	}
}


template<bool tear = true, class TRIMESH>
inline void edgeMark_1sided(TRIMESH& mesh, ui32 edge, bool flipCutNormal){
	ASS(mesh.isUsingELinks());

	auto rev = mesh.elinks[edge];
	mesh.eflags[edge] |= flipCutNormal ? 1 : 2; // mark outside
	//if (mesh.eflags[edge] == 3) __debugbreak();
	if (rev != TRIMESH::IIDX_INT(-1)){
		// tear
		if (tear){
			mesh.elinks[edge] = (TRIMESH::IIDX_INT)(-1);
			mesh.elinks[rev] = (TRIMESH::IIDX_INT)(-1);
		}

		// mesh.eflags[rev] |= flipCutNormal ? 2 : 1; // mark inside
	}
}



// blend vertex `iV` attributes over barycentric coords `V_coords` (B-A, C-A) of triangle ABC
// used usually after triCut
// can't move to triCut because sometimes we blend over some larger triangle existing before previous cut
template<class TRIMESH>
inline void vertexAttribBlend(TRIMESH& mesh, int iA, int iB, int iC, int iV, const Vector2<float>& V_coords){
	mesh.vs(iV).lerpAttributes(mesh.vs(iA), mesh.vs(iB), mesh.vs(iC), V_coords);
}



// Cut and mark polygon
// mesh a: poly i, triangle t1
template<class TRIMESH>
inline void cutAndMarkIterationA(TRIMESH& a, uint i,
	const Vector3<typename TRIMESH::POS_FLOAT>& U,
	const Vector3<typename TRIMESH::POS_FLOAT>& V,
	const Triangle3<typename TRIMESH::POS_FLOAT>& t,
	const typename CutInfo<typename TRIMESH::POS_FLOAT>::PerTriangle& pt)
{
	if (pt.U_where == WherePoint::inside){
		BarycentricCoords<typename TRIMESH::POS_FLOAT> coords(t);
		auto U_coords = coords(U);

		auto iA = a.ps()[i][0];
		auto iB = a.ps()[i][1];
		auto iC = a.ps()[i][2];

		triCut(a, i, U);
		vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, U_coords);

		if (pt.V_where == WherePoint::inside){
			auto V_coords = coords(V);
			auto where = barycentricTricutOrient(U_coords, V_coords);
			switch (where){
			case WherePoint::AB:
				triCut(a, i, V);
				vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);
				edgeMark_2sided(a, (a.ps().size()-2)*3 + 1, pt.flipNormal);
				break;
			case WherePoint::BC:
				triCut(a, a.ps().size() - 2, V);
				vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);
				edgeMark_2sided(a, (a.ps().size() - 2) * 3 + 1, pt.flipNormal);
				break;
			case WherePoint::CA:
				triCut(a, a.ps().size() - 1, V);
				vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);
				edgeMark_2sided(a, (a.ps().size() - 2) * 3 + 1, pt.flipNormal);
				break;
			case WherePoint::A:{
				auto r = edgeCut(a, 3*i + 2, V);
				edgeMark_2sided(a, r.AV, pt.flipNormal);
				}
				break;
			case WherePoint::B:{
				auto r = edgeCut(a, 3*(a.ps().size()-2) + 2, V);
				edgeMark_2sided(a, r.AV, pt.flipNormal);
				}
				break;
			case WherePoint::C:{
				auto r = edgeCut(a, 3*(a.ps().size()-1) + 2, V);
				edgeMark_2sided(a, r.AV, pt.flipNormal);
				}
				break;
			}
		}
		else if (pt.V_where == WherePoint::AB){
			auto r = edgeCut(a, 3*i + 0, V);
			edgeMark_2sided(a, r.VC, !pt.flipNormal);
		}
		else if (pt.V_where == WherePoint::BC){
			auto r = edgeCut(a, 3 * (a.ps().size() - 2) + 0, V);
			edgeMark_2sided(a, r.VC, !pt.flipNormal);
		}
		else if (pt.V_where == WherePoint::CA){
			auto r = edgeCut(a, 3 * (a.ps().size() - 1) + 0, V);
			edgeMark_2sided(a, r.VC, !pt.flipNormal);
		}
		else if (pt.V_where == WherePoint::A){
			edgeMark_2sided(a, 3 * i + 2, pt.flipNormal);
		}
		else if (pt.V_where == WherePoint::B){
			edgeMark_2sided(a, 3 * (a.ps().size() - 2) + 2, pt.flipNormal);
		}
		else if (pt.V_where == WherePoint::C){
			edgeMark_2sided(a, 3 * (a.ps().size() - 1) + 2, pt.flipNormal);
		}
		else{
			ASS(false);
		}
	}
	else if (pt.U_where == WherePoint::AB){
		if (pt.V_where == WherePoint::inside){
			BarycentricCoords<typename TRIMESH::POS_FLOAT> coords(t);
			auto V_coords = coords(V);

			auto iA = a.ps()[i][0];
			auto iB = a.ps()[i][1];
			auto iC = a.ps()[i][2];

			triCut(a, i, V);
			vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);

			auto r = edgeCut(a, 3*i + 0, U);
			edgeMark_2sided(a, r.VC, pt.flipNormal);
		}
		else{
			auto r = edgeCut(a, 3 * i + 0, U);

			if (pt.V_where == WherePoint::AB){
				if ((U - t.a).lengthSqr() < (V - t.a).lengthSqr()){
					auto s = edgeCut(a, r.VB, V);
					edgeMark_1sided(a, s.AV, pt.flipNormal); // dzia³a dla overlap box test
				}
				else{
					auto s = edgeCut(a, r.AV, V);
					edgeMark_1sided(a, s.VB, !pt.flipNormal); // dzia³a dla overlap box test
				}
			}
			else if (pt.V_where == WherePoint::BC){
				auto s = edgeCut(a, r.BC, V);
				edgeMark_2sided(a, s.VC, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::CA){
				auto s = edgeCut(a, r.CA, V);
				edgeMark_2sided(a, s.VC, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::A){
				edgeMark_1sided(a, 3 * i + 0, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::B){
				edgeMark_1sided(a, 3 * (a.ps().size() - 2) + 0, pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::C){
				edgeMark_2sided(a, 3 * i + 1, pt.flipNormal);
			}
			else{
				ASS(false);
			}
		}
	}
	else if (pt.U_where == WherePoint::BC){
		if (pt.V_where == WherePoint::inside){
			BarycentricCoords<typename TRIMESH::POS_FLOAT> coords(t);
			auto V_coords = coords(V);

			auto iA = a.ps()[i][0];
			auto iB = a.ps()[i][1];
			auto iC = a.ps()[i][2];

			triCut(a, i, V);
			vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);

			auto r = edgeCut(a, 3 * (a.ps().size() - 2) + 0, U);
			edgeMark_2sided(a, r.VC, pt.flipNormal);
		}
		else{
			auto r = edgeCut(a, 3 * i + 1, U);

			if (pt.V_where == WherePoint::BC){
				if ((U - t.b).lengthSqr() < (V - t.b).lengthSqr()){
					auto s = edgeCut(a, r.VB, V);//''
					edgeMark_1sided(a, s.AV, pt.flipNormal);
				}
				else{
					auto s = edgeCut(a, r.AV, V);
					edgeMark_1sided(a, s.VB, !pt.flipNormal); // dupa?
				}
			}
			else if (pt.V_where == WherePoint::CA){
				auto s = edgeCut(a, r.BC, V);
				edgeMark_2sided(a, s.VC, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::AB){
				auto s = edgeCut(a, r.CA, V);
				edgeMark_2sided(a, s.VC, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::B){
				edgeMark_1sided(a, 3 * i + 1, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::C){
				edgeMark_1sided(a, 3 * (a.ps().size() - 2) + 0, pt.flipNormal);//
			}
			else if (pt.V_where == WherePoint::A){
				edgeMark_2sided(a, 3 * i + 2, pt.flipNormal);
			}
			else{
				ASS(false);
			}
		}
	}
	else if (pt.U_where == WherePoint::CA){
		if (pt.V_where == WherePoint::inside){
			BarycentricCoords<typename TRIMESH::POS_FLOAT> coords(t);
			auto V_coords = coords(V);

			auto iA = a.ps()[i][0];
			auto iB = a.ps()[i][1];
			auto iC = a.ps()[i][2];

			triCut(a, i, V);
			vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);

			auto r = edgeCut(a, 3 * (a.ps().size() - 1) + 0, U);
			edgeMark_2sided(a, r.VC, pt.flipNormal);
		}
		else{
			auto r = edgeCut(a, 3 * i + 2, U);

			if (pt.V_where == WherePoint::CA){
				if ((U - t.c).lengthSqr() < (V - t.c).lengthSqr()){
					auto s = edgeCut(a, r.VB, V);
					edgeMark_1sided(a, s.AV, pt.flipNormal);
				}
				else{
					auto s = edgeCut(a, r.AV, V);
					edgeMark_1sided(a, s.VB, !pt.flipNormal);//
				}
			}
			else if (pt.V_where == WherePoint::AB){
				auto s = edgeCut(a, r.BC, V);
				edgeMark_2sided(a, s.VC, !pt.flipNormal);//'
			}
			else if (pt.V_where == WherePoint::BC){
				auto s = edgeCut(a, r.CA, V);
				edgeMark_2sided(a, s.VC, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::C){
				edgeMark_1sided(a, 3 * i + 2, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::A){
				edgeMark_1sided(a, 3 * (a.ps().size() - 2) + 0, pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::B){
				edgeMark_2sided(a, 3 * i + 0, pt.flipNormal);////
			}
			else{
				ASS(false);
			}
		}
	}
	else if (pt.U_where == WherePoint::A){
		if (pt.V_where == WherePoint::inside){
			BarycentricCoords<typename TRIMESH::POS_FLOAT> coords(t);
			auto V_coords = coords(V);

			auto iA = a.ps()[i][0];
			auto iB = a.ps()[i][1];
			auto iC = a.ps()[i][2];

			triCut(a, i, V);
			vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);

			edgeMark_2sided(a, 3 * i + 2, !pt.flipNormal);
		}
		else{
			if (pt.V_where == WherePoint::AB){
				auto r = edgeCut(a, 3*i + 0, V);
				edgeMark_1sided(a, r.AV, pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::BC){
				auto r = edgeCut(a, 3*i + 1, V);
				edgeMark_2sided(a, r.VC, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::CA){
				auto r = edgeCut(a, 3*i + 2, V);
				edgeMark_1sided(a, r.VB, !pt.flipNormal);
			}
			//else if (pt.V_where == WherePoint::A){
			//}
			else if (pt.V_where == WherePoint::B){
				edgeMark_1sided(a, 3 * i + 0, pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::C){
				edgeMark_1sided(a, 3 * i + 2, !pt.flipNormal);
			}
			else{
				ASS(false);
			}
		}
	}
	else if (pt.U_where == WherePoint::B){
		if (pt.V_where == WherePoint::inside){
			BarycentricCoords<typename TRIMESH::POS_FLOAT> coords(t);
			auto V_coords = coords(V);

			auto iA = a.ps()[i][0];
			auto iB = a.ps()[i][1];
			auto iC = a.ps()[i][2];

			triCut(a, i, V);
			vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);

			edgeMark_2sided(a, 3 * (a.ps().size()-2) + 2, !pt.flipNormal);
		}
		else{
			if (pt.V_where == WherePoint::BC){
				auto r = edgeCut(a, 3*i + 1, V);
				edgeMark_1sided(a, r.AV, pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::CA){
				auto r = edgeCut(a, 3*i + 2, V);
				edgeMark_2sided(a, r.VC, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::AB){
				auto r = edgeCut(a, 3*i + 0, V);
				edgeMark_1sided(a, r.VB, !pt.flipNormal);//
			}
			//else if (pt.V_where == WherePoint::B){
			//}
			else if (pt.V_where == WherePoint::C){
				edgeMark_1sided(a, 3 * i + 1, pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::A){
				edgeMark_1sided(a, 3 * i + 0, !pt.flipNormal);
			}
			else{
				ASS(false);
			}
		}

	}
	else if (pt.U_where == WherePoint::C){

		if (pt.V_where == WherePoint::inside){
			BarycentricCoords<typename TRIMESH::POS_FLOAT> coords(t);
			auto V_coords = coords(V);

			auto iA = a.ps()[i][0];
			auto iB = a.ps()[i][1];
			auto iC = a.ps()[i][2];

			triCut(a, i, V);
			vertexAttribBlend(a, iA, iB, iC, a.vs().size() - 1, V_coords);

			edgeMark_2sided(a, 3 * (a.ps().size() - 1) + 2, !pt.flipNormal);
		}
		else{
			if (pt.V_where == WherePoint::CA){
				auto r = edgeCut(a, 3*i + 2, V);
				edgeMark_1sided(a, r.AV, pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::AB){
				auto r = edgeCut(a, 3*i + 0, V);
				edgeMark_2sided(a, r.VC, !pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::BC){
				auto r = edgeCut(a, 3 * i + 1, V);
				edgeMark_1sided(a, r.VB, !pt.flipNormal);
			}
			//else if (pt.V_where == WherePoint::C){
			//}
			else if (pt.V_where == WherePoint::A){
				edgeMark_1sided(a, 3 * i + 2, pt.flipNormal);
			}
			else if (pt.V_where == WherePoint::B){
				edgeMark_1sided(a, 3 * i + 1, !pt.flipNormal);
			}
			else{
				ASS(false);
			}
		}

	}
	else{
		ASS(false);
	}
}




// Cut and mark polygon pair
// mesh a: poly i (triangle t1)
// mesh b: poly j (triangle t2)
template<class TRIMESH>
inline void cutAndMarkIteration(TRIMESH& a, TRIMESH& b,
	uint i, uint j,
	const Triangle3<typename TRIMESH::POS_FLOAT>& t1,
	const Triangle3<typename TRIMESH::POS_FLOAT>& t2,
	std::vector<std::pair<uint, ab::Vector3<typename TRIMESH::POS_FLOAT>>> vertexMove[2])
{
	static int dbg = 0;
	++dbg;

	CutInfo<TRIMESH::POS_FLOAT> cutInfo = triangle3Intersection(t1,t2);
	if(!cutInfo.cut) return;

	if (cutInfo.coplanar){
		if(a.isUsingPFlags()) a.pflags[i] = 1;
		if(b.isUsingPFlags()) b.pflags[j] = 1;
		return;
	}

	//ASS((cutInfo.U - cutInfo.V).lengthSqr() > 1e-12);

	// vertexMove for `a`
	switch (cutInfo.t[0].U_where){
	case WherePoint::A:
		vertexMove[0].push_back(std::make_pair(a.indices[i * 3 + 0], cutInfo.U));
		break;
	case WherePoint::B:
		vertexMove[0].push_back(std::make_pair(a.indices[i * 3 + 1], cutInfo.U));
		break;
	case WherePoint::C:
		vertexMove[0].push_back(std::make_pair(a.indices[i * 3 + 2], cutInfo.U));
		break;
	}

	switch (cutInfo.t[0].V_where){
	case WherePoint::A:
		vertexMove[0].push_back(std::make_pair(a.indices[i * 3 + 0], cutInfo.V));
		break;
	case WherePoint::B:
		vertexMove[0].push_back(std::make_pair(a.indices[i * 3 + 1], cutInfo.V));
		break;
	case WherePoint::C:
		vertexMove[0].push_back(std::make_pair(a.indices[i * 3 + 2], cutInfo.V));
		break;
	}

	// vertexMove for `b`
	switch (cutInfo.t[1].U_where){
	case WherePoint::A:
		vertexMove[1].push_back(std::make_pair(b.indices[j * 3 + 0], cutInfo.U));
		break;
	case WherePoint::B:
		vertexMove[1].push_back(std::make_pair(b.indices[j * 3 + 1], cutInfo.U));
		break;
	case WherePoint::C:
		vertexMove[1].push_back(std::make_pair(b.indices[j * 3 + 2], cutInfo.U));
		break;
	}

	switch (cutInfo.t[1].V_where){
	case WherePoint::A:
		vertexMove[1].push_back(std::make_pair(b.indices[j * 3 + 0], cutInfo.V));
		break;
	case WherePoint::B:
		vertexMove[1].push_back(std::make_pair(b.indices[j * 3 + 1], cutInfo.V));
		break;
	case WherePoint::C:
		vertexMove[1].push_back(std::make_pair(b.indices[j * 3 + 2], cutInfo.V));
		break;
	}

	// cut mesh a, b
	cutAndMarkIterationA(a, i, cutInfo.U, cutInfo.V, t1, cutInfo.t[0]);
	cutAndMarkIterationA(b, j, cutInfo.U, cutInfo.V, t2, cutInfo.t[1]);
}


template<class TRIMESH>
inline void cutAndMark(TRIMESH& a, TRIMESH& b){

	auto t0 = std::chrono::high_resolution_clock::now();

	std::vector<std::pair<uint, ab::Vector3<typename TRIMESH::POS_FLOAT>>> vertexMove[2];

	static int dbg = 0;

	// TODO: not n^2
	//int dbg = 0;
	for(uint i=0; i<a.ps().size(); ++i){
		for (uint j = 0; j<b.ps().size(); ++j){
			++dbg;
			//if (dbg == 1641){
			//	a.eflags[3 * i] = 69;
			//	b.eflags[3 * j] = 69;
			//	break;
			//}
			Triangle3<TRIMESH::POS_FLOAT> t1(a.vs()[a.ps()[i][0]].pos(), a.vs()[a.ps()[i][1]].pos(), a.vs()[a.ps()[i][2]].pos());
			Triangle3<TRIMESH::POS_FLOAT> t2(b.vs()[b.ps()[j][0]].pos(), b.vs()[b.ps()[j][1]].pos(), b.vs()[b.ps()[j][2]].pos());

			//LOGD("");
			//LOGD(std::to_string(t1));
			//LOGD(std::to_string(t2));
			
			cutAndMarkIteration(a,b,i,j,t1,t2,vertexMove);
			

			//if (!checkSolid(a)){
			//	break;
			//}

			++dbg;
		}
		//if (!checkSolid(a)){
		//	break;
		//}
		//if (dbg == 1641)break;
	}

	// apply vertexMove
	for (uint i = 0; i < vertexMove[0].size(); ++i){
		a.vs(vertexMove[0][i].first).pos() = vertexMove[0][i].second;
	}
	for (uint i = 0; i < vertexMove[1].size(); ++i){
		b.vs(vertexMove[1][i].first).pos() = vertexMove[1][i].second;
	}
	// re-fix vremap
	for (uint i = 0; i < a.vs().size(); ++i){
		if (a.vremap[i] != i) a.vs(i).pos() = a.vs(a.vremap[i]).pos();
	}
	for (uint i = 0; i < b.vs().size(); ++i){
		if (b.vremap[i] != i) b.vs(i).pos() = b.vs(b.vremap[i]).pos();
	}
	
	auto deltaTime = std::chrono::high_resolution_clock::now() - t0;
	auto seconds = std::chrono::duration_cast<std::chrono::duration<long double>>(deltaTime);
	LOGI("cutAndMark() time: " + std::to_string(seconds.count()) + " seconds.");
}








// dfs and count inside/outside eflags in each connected component
template<class TRIMESH>
inline void classifyByEFlags(const TRIMESH& a, std::vector<i8>& cls){
	Timer timer;

	// each connected component is identified by dfs starting poly index
	// clsmap maps connected component to cls
	std::vector<i8> clsmap(a.ps().size());
	std::vector<ui32> vis(a.ps().size());
	for (uint i = 0; i < a.ps().size(); ++i){
		if (vis[i]) continue;

		int numInside = 0;
		int numOutside = 0;

		int numCoplanar = 0;
		int numNormal = 0;

		std::vector<ui32> stack;
		stack.push_back(i);
		vis[i] = i + 1;
		while (!stack.empty()){
			int cpoly = stack.back();
			stack.pop_back();

			if (a.eflags[cpoly * 3 + 0] & 1) ++numInside;
			if (a.eflags[cpoly * 3 + 0] & 2) ++numOutside;
			if (a.eflags[cpoly * 3 + 1] & 1) ++numInside;
			if (a.eflags[cpoly * 3 + 1] & 2) ++numOutside;
			if (a.eflags[cpoly * 3 + 2] & 1) ++numInside;
			if (a.eflags[cpoly * 3 + 2] & 2) ++numOutside;

			if (a.pflags[cpoly]) ++numCoplanar;
			else ++numNormal;

			for (int k = 0; k < 3; ++k){
				auto elink = a.elinks[cpoly * 3 + k];
				if (elink == (ui32)(-1))continue;

				auto destpoly = elink / 3; // todo: DIVISION!
				if (vis[destpoly]){
					ASS(vis[destpoly] == i + 1); // if fails, graph is directed
					continue;
				}

				vis[destpoly] = i + 1;
				stack.push_back(destpoly);
			}
		}

		//if (numInside && numOutside){
		//	LOGW("Boolean: connected component flags don't match: " + std::to_string(numInside) + " inside vs " + std::to_string(numOutside) + " outside.");
		//}
		LOGD("classifyByEFlags: " + std::to_string(numInside) + " inside vs " + std::to_string(numOutside) + " outside.");

		// try to improvise
		if (numNormal == 0) clsmap[i] = 2;
		else if (numInside > numOutside) clsmap[i] = -1;
		else if (numInside < numOutside) clsmap[i] = 1;
		else clsmap[i] = 0;

		// mark only when sure
		//if (numInside && numOutside == 0) clsmap[i] = -1;
		//else if(numOutside && numInside == 0) clsmap[i] = 1;
		//else if(numOutside && numInside) clsmap[i] = 2; // assume this part is overlapping surface
		//else clsmap[i] = 0;
	}

	// apply clsmap
	for (uint i = 0; i < a.ps().size(); ++i){
		cls[i] = clsmap[vis[i] - 1];
	}

	LOGI("classifyByEFlags() completed in " + std::to_string(timer));
}




// raycast and dfs isolated triangles
template<class TRIMESH>
void classifyByRaycast(const TRIMESH& a, const TRIMESH& b, std::vector<i8>& cls){
	Timer timer;
	for (uint i = 0; i<a.ps().size(); ++i){
		if (cls[i]) continue;

		//LOGD("Boolean raycasting");
		// TODO: raycast from poly center and in slightly different direction!!!
		// but it's easier to debug this way
		auto rr = raycast(b, a.vs(a.ps(i)[0]).pos(), { 1, 0, 0 }, (TRIMESH::POS_FLOAT) - 1e-6);

		if (rr.travelSteps < 1e-6){
			LOGD("RayPos in triangle " + std::to_string(i) + ". Awkward.");
			continue; // rayPos in triangle - can't deduct anything
		}

		if (rr.poly == (ui32)(-1)){
			cls[i] = 1;
			LOGD("Poly " + std::to_string(i) + " outside by raycast (no poly hit).");
		}
		else{
			auto normal = b.ps(rr.poly).triangle().normalDir();
			if (normal * Vector3 <typename TRIMESH::POS_FLOAT> {1, 0, 0} > 0){
				cls[i] = -1;
				LOGD("Poly " + std::to_string(i) + " inside by raycast.");
			}
			else{
				cls[i] = 1;
				LOGD("Poly " + std::to_string(i) + " outside by raycast.");
			}
		}

		// dfs
		std::vector<ui32> stack;
		stack.push_back(i);
		while (!stack.empty()){
			ui32 poly = stack.back();
			stack.pop_back();
			for (int k = 0; k < 3; ++k){
				auto e = a.elinks[3 * poly + k];
				if (e == TRIMESH::IIDX_INT(-1)) continue;

				auto destpoly = e / 3;
				if (cls[destpoly] != 0) continue;

				cls[destpoly] = cls[poly];
				stack.push_back(destpoly);
			}
		}
	}
	LOGI("classifyByRaycast() completed in " + std::to_string(timer));
}



// store result in `a`
// leaves `b` in unknown state
template<class TRIMESH>
void booleanSubtract(TRIMESH& a, TRIMESH& b){
	typedef typename TRIMESH::POS_FLOAT F;

	// enable vremap for fast checkSolid()
	DE{
		a.enableVRemap();
		b.enableVRemap();
	}

	ASS(checkSolid(a));
	ASS(checkSolid(b));

	// create elinks if not already present
	a.enableELinks();
	b.enableELinks();
	// also create eflags
	a.resetEFlags();
	b.resetEFlags();
	// also create pflags
	a.resetPFlags();
	b.resetPFlags();
	
	cutAndMark(a, b);




	//  0 == unknown
	// -1 == inside
	//  1 == outside
	std::vector<i8> cls_a(a.ps().size());
	classifyByEFlags(a, cls_a);
	//a.disableEFlags();
	classifyByRaycast(a, b, cls_a);

	std::vector<i8> cls_b(b.ps().size());
	classifyByEFlags(b, cls_b);
	//b.disableEFlags();
	classifyByRaycast(b, a, cls_b);

	// elinks no longer needed
	a.disableELinks();
	b.disableELinks();


	//// debug
	//// delete triangles from `a`
	//for (int i = (int)a.ps().size() - 1; i >= 0; --i){
	//	//if ((a.eflags[3*i+0] & 2) || (a.eflags[3*i+1] & 2) || (a.eflags[3*i+2] & 2)){
	//	//if ((a.eflags[3*i+0] & 1) || (a.eflags[3*i+1] & 1) || (a.eflags[3*i+2] & 1)){
	//	if (a.eflags[3*i+0] == 69){
	//	//if(cls_a[i] != 1){
	//		a.ps(i) = a.ps().back();
	//		a.ps().pop_back();
	//		//ASS(checkELinks(a));
	//	}
	//}
	//// delete triangles from `b`
	//for (int i = (int)b.ps().size() - 1; i >= 0; --i){
	//	//if ((a.eflags[3*i+0] & 2) || (a.eflags[3*i+1] & 2) || (a.eflags[3*i+2] & 2)){
	//	//if ((b.eflags[3*i+0] & 1) || (b.eflags[3*i+1] & 1) || (b.eflags[3*i+2] & 1)){
	//	if (b.eflags[3 * i + 0] == 69){
	//	//if(cls_b[i] == 1){
	//		b.ps(i) = b.ps().back();
	//		b.ps().pop_back();
	//		//ASS(checkELinks(a));
	//	}
	//}
	//flipPolys(b);
	//append(a, b);
	//return;



	// delete triangles with vis != 1 from `a`
	for(int i=(int)a.ps().size()-1; i>=0; --i){
		if (cls_a[i] == 1) continue;

		a.ps(i) = a.ps().back();
		a.ps().pop_back();
	}
	removeIsolatedVertices(a);

	// delete triangles with vis != -1 from `b`
	for (int i = (int)b.ps().size() - 1; i >= 0; --i){
		if (cls_b[i] == -1) continue;

		b.ps(i) = b.ps().back();
		b.ps().pop_back();
	}
	removeIsolatedVertices(b);
	

	// flip b's polys
	flipPolys(b);

	// append b's triangles to `a`
	append(a,b);


	DE{
		a.disableVRemap();
		b.disableVRemap();
	}
}




}










