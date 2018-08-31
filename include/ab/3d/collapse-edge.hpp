#pragma once


#include <atablash/union-find.hpp>

// for debug
#include <atablash/3d/solid.hpp>
#include <atablash/3d/build-elinks.hpp>


namespace ab{

// collapse edge (a,b) if shorter than eps and a<b
// uses vweights to keep welded groups stronger; to make collapse order irrevelant
// vmoves is the upper bound of total vertex move
template<typename TRIMESH>
struct collapseEdge_result {
	bool collapsed = false; // was collapse performed?
	typename TRIMESH::POS_FLOAT newMaxMoved = 0; // new upper bound of move (vmoves)
};
template<bool use_vweights, bool use_vmoves, class TRIMESH>
inline collapseEdge_result<TRIMESH> collapseEdge(TRIMESH& mesh, uint poly, uint ith,
	const typename TRIMESH::POS_FLOAT& eps,
	std::vector<uint>& vweights = std::vector<uint>(),
	std::vector<typename TRIMESH::POS_FLOAT>& vmoves = std::vector<typename TRIMESH::POS_FLOAT>())
{
	collapseEdge_result<TRIMESH> result;
	// poly might be degenerate - lazy delete it
	bool valid = !(mesh.isUsingPFlags() && (mesh.pflags[poly] & TRIMESH::P_DELETE));
	if (valid && isDegenerateAndFix(mesh, poly)) {
		valid = false;
	}

	// if poly is deleted, try the other side of the elink
	if (!valid) {
		if (!mesh.isUsingELinks()) return result;

		auto otherEdge = mesh.elinks[poly * 3 + ith];
		if (otherEdge == uint(-1)) return result;

		auto otherPoly = otherEdge / 3;

		// other poly might be degenerate - lazy delete it
		bool otherValid = !(mesh.isUsingPFlags() && mesh.pflags[otherPoly] & TRIMESH::P_DELETE);
		if (otherValid && isDegenerateAndFix(mesh, otherPoly)) {
			otherValid = false;
		}

		// other poly is also deleted. no-op.
		if (!otherValid) return result;

		// switch to the poly on the other side of the edge
		poly = otherPoly;
		ith = otherEdge % 3;
	}

	uint iaa = poly * 3 + ith;
	uint ibb = ith == 2 ? poly * 3 + 0 : poly * 3 + ith + 1;
	uint aa = mesh.indices[iaa];
	uint bb = mesh.indices[ibb];

	uint a = mesh.vremap[aa];
	uint b = mesh.vremap[bb];
	if (a == b) return result;

	if (mesh.elinks[iaa] != uint(-1) && b < a) return result; // optim

	auto distSqr = (mesh.vs(a).pos() - mesh.vs(b).pos()).lengthSqr();
	if (distSqr > eps*eps) return result;

	// do collapse

	//LOGD("collapse edge " + std::to_string(poly) + ":" + std::to_string(ith));

	result.collapsed = true;

	TRIMESH::POS_FLOAT alpha = 0.5;
	if (use_vweights) {
		vweights.resize(mesh.vs().size(), 1);
		alpha = (TRIMESH::POS_FLOAT)vweights[a] / (vweights[a] + vweights[b]);

		vweights[b] += vweights[a];
	}

	mesh.vs(b).pos() = alpha * mesh.vs(a).pos() + (1 - alpha) * mesh.vs(b).pos();
	mesh.vs(bb).lerpAttributes(mesh.vs(bb), mesh.vs(aa), alpha);

	if (use_vmoves) {
		vmoves.resize(mesh.vs().size());
		auto dist = sqrt(distSqr);
		vmoves[b] = std::max(vmoves[a] + dist*(1-alpha), vmoves[b] + dist*alpha);
		result.newMaxMoved = vmoves[b];
	}

	// not using hard remap: duplicate bb's attributes also to aa
	if(!mesh.isUsing_vremapExact()) mesh.vs(aa).lerpAttributes(mesh.vs(bb), mesh.vs(aa), alpha);


	connect(mesh, poly * 3 + (ith + 1) % 3, poly * 3 + (ith + 2) % 3);

	ASS((mesh.pflags[poly] & TRIMESH::P_DELETE) == 0);
	mesh.pflags[poly] |= TRIMESH::P_DELETE;
	//LOGD("delete poly " + std::to_string(poly));

	for (;;) {
		uint otherEdge = mesh.elinks[iaa];
		if (otherEdge == uint(-1)) break;

		ASS(mesh.elinks[otherEdge] == iaa);

		auto otherPoly = otherEdge / 3;
		if (isDegenerateAndFix(mesh, otherPoly)) {
			continue;
		}

		uint jth = otherEdge % 3;
		ASS((mesh.pflags[otherPoly] & TRIMESH::P_DELETE) == 0);
		mesh.pflags[otherPoly] |= TRIMESH::P_DELETE;
		//LOGD("delete other poly " + std::to_string(otherPoly));

		// other edge attributes merge
		if (mesh.isUsing_vremapExact()) {
			auto bb_2 = mesh.indices[otherPoly * 3 + jth];
			auto aa_2 = mesh.indices[otherPoly * 3 + (jth + 1) % 3];
			// todo: could do some lerpAttributes
			mesh.vremapExact[aa_2] = bb_2;
		}

		connect(mesh, otherPoly * 3 + (jth + 1) % 3, otherPoly * 3 + (jth + 2) % 3);
		break;
	}

	mesh.vremap[a] = b;
	if (mesh.isUsing_vremapExact()) mesh.vremapExact[aa] = bb; // should use b's attributes

	// may fail if we just collapsed edge that has other polys vremapped to it
	// need to make sure that other instances are also collapsed
	// (no way to chain them here using current mesh representation)
	//ASS(checkDegeneratePolys(mesh));

	return result;
}



// performs welded vertices deletion or delays outside this function to vremapHard
// returns number of collapses
// does not use vmoves
template<bool delayRemap = false, class TRIMESH>
inline uint collapseEdges(TRIMESH& mesh, const typename TRIMESH::POS_FLOAT& eps,
	std::vector<uint>& vweights = std::vector<uint>())
{
	uint result = 0;
	bool change = true;
	uint numIters = 0;
	while (change) {
		++numIters;
		change = false;

		for (uint p = 0; p < mesh.ps().size(); ++p) {
			if (mesh.pflags[p] & TRIMESH::P_DELETE)continue;
			for (int i = 0; i < 3; ++i) {
				auto r = collapseEdge<true, false>(mesh, p, i, eps, vweights);
				if (r.collapsed) {
					++result;
					change = true;
					break;
				}
			}
		}
	}
	if (numIters > 2) LOGW("Performance warning: collapseEdges: number of passes: " + std::to_string(numIters));

	if (!delayRemap) {
		mesh.apply_vremapExact();
	}
	return result;
}



}


