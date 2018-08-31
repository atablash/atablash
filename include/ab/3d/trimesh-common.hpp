#pragma once


#include <atablash/templates.hpp>
#include <atablash/timer.hpp>
#include <atablash/union-find.hpp>


namespace ab{

// VExtras
enum class VExtra           { RGB = 1, ALPHA = 2, UV = 4, NORMAL = 8, TANGENT_U = 16, TANGENT_V = 32, terminator };
const ui8 VExtraSizes[] =   { 3,	   1,         8,      12,          12,             12, };




template<class VS, class TRANSFORM>
inline void transformVertices(VS& vs, const TRANSFORM& t){
	for(uint i=0; i<vs.size(); ++i){
		vs[i].pos() *= t;
	}
}



// check if poly remaps to two or more same indices
// if that is the case, fix
template<class TRIMESH>
inline bool isDegenerateAndFix(TRIMESH& mesh, uint poly,
	UnionFind<uint>& vremapHard = UnionFind<uint>())
{
	auto iA0 = mesh.ps(poly)[0].idx();
	auto iB0 = mesh.ps(poly)[1].idx();
	auto iC0 = mesh.ps(poly)[2].idx();

	auto iA = iA0;
	auto iB = iB0;
	auto iC = iC0;

	if (mesh.isUsingVRemap()) {
		iA = mesh.vremap[iA];
		iB = mesh.vremap[iB];
		iC = mesh.vremap[iC];
	}
	
	if (iA != iB && iB != iC && iC != iA) {
		return false;
	}

	if (mesh.isUsingPFlags()) {
		ASS((mesh.pflags[poly] & TRIMESH::P_DELETE) == 0);
		mesh.pflags[poly] |= TRIMESH::P_DELETE;
	}

	if (iA == iB && iB == iC) {
		// no need to fix elinks
		if (mesh.isUsing_vremapExact()) {
			mesh.vremapExact[iA] = iC;
			mesh.vremapExact[iB] = iC;
		}
		return true;
	}

	if (iA == iB) {
		if (mesh.isUsing_vremapExact()) mesh.vremapExact[iA] = iB;
		connect(mesh, poly*3+1, poly*3+2);
	}
	else if (iB == iC) {
		if (mesh.isUsing_vremapExact()) mesh.vremapExact[iB] = iC;
		connect(mesh, poly*3+2, poly*3+0);
	}
	else { // if(iC == iA)
		if (mesh.isUsing_vremapExact()) mesh.vremapExact[iC] = iA;
		connect(mesh, poly*3+0, poly*3+1);
	}
	return true;
}



template<bool checkShape = true, class TRIMESH>
inline bool isPolyOk(const TRIMESH& mesh, int poly)
{
	const auto eps = (typename TRIMESH::POS_FLOAT)1e-12; // squared
	
	auto iA = mesh.ps(poly)[0].idx();
	auto iB = mesh.ps(poly)[1].idx();
	auto iC = mesh.ps(poly)[2].idx();

	// not needed. only for debug break
	if (iA == iB || iB == iC || iC == iA) {
		LOGD("duplicate index before remap");
		return false;
	}

	if (mesh.isUsingVRemap()) {
		iA = mesh.vremap[iA];
		iB = mesh.vremap[iB];
		iC = mesh.vremap[iC];
	}

	if (iA == iB || iB == iC || iC == iA) {
		LOGD("duplicate index after remap");
		return false;
	}

	if (!checkShape) return true;
	
	const auto& A = mesh.vs(iA).pos();
	const auto& B = mesh.vs(iB).pos();
	const auto& C = mesh.vs(iC).pos();
	
	auto BA = B - A;
	auto CA = C - A;
	auto CB = C - B;

	if (BA.lengthSqr() < eps){
		typename TRIMESH::POS_FLOAT x = BA.lengthSqr();
		LOGD("BA too short");
		return false;
	}
	if (CB.lengthSqr() < eps){
		typename TRIMESH::POS_FLOAT x = CB.lengthSqr();
		LOGD("CB too short");
		return false;
	}
	if (CA.lengthSqr() < eps){
		typename TRIMESH::POS_FLOAT x = CA.lengthSqr();
		LOGD("CA too short");
		return false;
	}

	// A, B, C on the same line
	CA.normalize();
	BA.normalize();
	if ((BA ^ CA).lengthSqr() < 1e-12){
		typename TRIMESH::POS_FLOAT x = (BA ^ CA).lengthSqr();
		LOGD("A, B, C on the same line");
		return false; // allow degenerate triangles or not?
	}
	
	return true;
}



// append `b` to `a` (copy)
// does not weld vertices (vremap)
template<class TRIMESH>
inline void append(TRIMESH& a, const TRIMESH& b){

	auto t0 = std::chrono::high_resolution_clock::now();

	auto voffset = a.vs().size();
	auto poffset = a.ps().size();
	a.vs().resize(a.vs().size() + b.vs().size());
	a.vremap.resize(a.vs().size());
	a.ps().resize(a.ps().size() + b.ps().size());
	
	for(ui32 i=0; i<b.vs().size(); ++i){
		a.vs(voffset + i) = b.vs(i);
		if(a.isUsingVRemap()) a.vremap[voffset + i] = b.vremap[i] + voffset; // TODO optim: move outside the loop
	}
	
	for (ui32 i = 0; i<b.ps().size(); ++i){
		a.ps(poffset + i).idx(0) = b.ps(i).idx(0) + voffset;
		a.ps(poffset + i).idx(1) = b.ps(i).idx(1) + voffset;
		a.ps(poffset + i).idx(2) = b.ps(i).idx(2) + voffset;
	}
	
	// TODO: elinks, eflags


	auto deltaTime = std::chrono::high_resolution_clock::now() - t0;
	auto seconds = std::chrono::duration_cast<std::chrono::duration<long double>>(deltaTime);
	LOGI("append() time: " + std::to_string(seconds.count()) + " seconds.");
}


template<class TRIMESH>
inline void flipPolys(TRIMESH& mesh){

	auto t0 = std::chrono::high_resolution_clock::now();


	for(ui32 i=0; i<mesh.ps().size(); ++i){
		std::swap(mesh.ps(i).idx(1), mesh.ps(i).idx(2));
		
		if(mesh.isUsingELinks()){
			std::swap(mesh.elinks[i*3 + 0], mesh.elinks[i*3 + 2]);
		}
	}
	ASS(!mesh.isUsingELinks() || checkELinks(mesh));


	auto deltaTime = std::chrono::high_resolution_clock::now() - t0;
	auto seconds = std::chrono::duration_cast<std::chrono::duration<long double>>(deltaTime);
	LOGI("flipPolys() time: " + std::to_string(seconds.count()) + " seconds.");
}




template<class TRIMESH>
inline void removeIsolatedVertices(TRIMESH& mesh){
	Timer timer;

	auto prevVsSize = mesh.vs().size();
	std::vector<bool> used(mesh.vs().size());
	for (ui32 i = 0; i < mesh.ps().size(); ++i){
		used[mesh.ps(i)[0]] = true;
		used[mesh.ps(i)[1]] = true;
		used[mesh.ps(i)[2]] = true;
	}

	std::vector<ui32> remap(mesh.vs().size());
	for (int i = 0; i<(int)remap.size(); ++i) remap[i] = i;

	// invariants:
	//  [0,i)    already processed, contains only used vertices
	//  [j,SIZE) already processed, contains only unused vertices
	//  [i,j) left to process
	{
		ui32 i = 0;
		ui32 j = mesh.vs().size();
		while (true){
			while (i < j &&  used[i])   ++i;
			while (i < j && !used[j-1]) --j;
			if (i == j) break;

			mesh.vs(i) = mesh.vs(j-1);
			remap[i] = ui32(-1); // neede only if (mesh.isUsingVRemap())
			remap[j - 1] = i;
			ASS(i+1 != j);
			++i;
			--j;
		}
		mesh.vs().resize(i);
	}

	// apply remap to indices
	for (int i = 0; i<(int)mesh.ps().size(); ++i){
		mesh.ps()[i][0] = remap[mesh.ps()[i][0]];
		mesh.ps()[i][1] = remap[mesh.ps()[i][1]];
		mesh.ps()[i][2] = remap[mesh.ps()[i][2]];
	}

	// apply remap to vremap
	if (mesh.isUsingVRemap()) for (ui32 i = 0; i < mesh.vs().size(); ++i){
		auto& x = remap[mesh.vremap[i]];
		if (x == ui32(-1)){
			x = i; // vertex remapped to does not exist, set a new representant `i`
		}
		mesh.vremap[i] = x;
	}

	LOGI("removeIsolatedVertices(): " + std::to_string(prevVsSize) + " -> " + std::to_string(mesh.vs().size()) + " (" + std::to_string(timer) + ")");
}





// shuffle mesh vertices and polys for debug
// does not need to be optimized
template<class TRIMESH>
TRIMESH random_shuffle(const TRIMESH& mesh)
{
	TRIMESH result(mesh);

	// build vertices permutation
	std::vector<int> vperm(mesh.vs().size());
	for (uint i = 0; i < vperm.size(); ++i) vperm[i] = i;
	std::random_shuffle(vperm.begin(), vperm.end());

	// build polys permutation
	std::vector<int> pperm(mesh.ps().size());
	for (uint i = 0; i < pperm.size(); ++i) pperm[i] = i;
	std::random_shuffle(pperm.begin(), pperm.end());

	// shuffle vertices
	for (uint i = 0; i < vperm.size(); ++i) result.vs(vperm[i]) = mesh.vs(i);

	// shuffle polys
	for (uint i = 0; i < pperm.size(); ++i) result.ps(pperm[i]) = mesh.ps(i);

	// update indices
	for (uint i = 0; i < result.indices.size(); ++i) result.indices[i] = vperm[result.indices[i]];

	// todo: update vremap, elinks, eflags

	return result;
}





// update positions
template<class TRIMESH>
inline void apply_vremap(TRIMESH& mesh){
	ASS(mesh.isUsingVRemap());

	for (uint i = 0; i < mesh.vs().size(); ++i){
		if (mesh.vremap[i] == i) continue;

		mesh.vs(i).pos() = mesh.vs(mesh.vremap[i]).pos();
	}
}




template<class T, class TRIMESH>
inline std::vector<T> get_vdegrees(const TRIMESH& mesh) {
	std::vector<T> r(mesh.vs().size());
	for (uint i = 0; i < mesh.indices.size(); ++i) {
		++r[mesh.indices[i]];
	}
	return r;
}


// true for used vertices
template<class T, class TRIMESH>
inline std::vector<T> get_vdegrees_any(const TRIMESH& mesh) {
	std::vector<T> r(mesh.vs().size());
	for (uint i = 0; i < mesh.indices.size(); ++i) {
		r[mesh.indices[i]] = true;
	}
	return r;
}



// removes vertices for which v[i]==0
template<class TRIMESH, class V>
inline void delete_not_marked(TRIMESH& mesh, const V& v) {
	std::vector<uint> moves(mesh.vs().size());
	for (uint i = 0; i<moves.size(); ++i) moves[i] = i;
	uint moveFrom = mesh.vs().size() - 1;

	for (uint i = 0; i < mesh.vs().size(); ++i) {
		// invariant: moveFrom is the upper bound of last valid element
		if (v[i]) continue;

		while (moveFrom >= i && !v[moveFrom]) --moveFrom;
		if (moveFrom <= i) {
			break;
		}

		mesh.vs(i) = mesh.vs(moveFrom);
		//LOGD("vertex move: " + std::to_string(moveFrom) + " " + std::to_string(i));

		ASS(moves[moveFrom] == moveFrom);
		std::swap(moves[moveFrom], moves[i]);
		--moveFrom;
	}
	mesh.vs().resize(moveFrom + 1);
	mesh.vremap.applyRename(moves);
	mesh.vremap.resize(mesh.vs().size());

	for (uint i = 0; i < mesh.indices.size(); ++i) {
		mesh.indices[i] = moves[mesh.indices[i]];
		ASS(mesh.indices[i] < mesh.vs().size());
	}
}


// delete vertices that are not used by any poly
template<class TRIMESH>
inline void delete_isolated_vertices(TRIMESH& mesh) {
	auto vdegrees = get_vdegrees_any<bool>(mesh); // optim todo: try ui8 instead of bool
	delete_not_marked(mesh, vdegrees);
}



template<class TRIMESH>
void apply_pdelete(TRIMESH& mesh){
	for (uint p = 0; p < mesh.ps().size(); ++p){
		if (mesh.pflags[p] & TRIMESH::P_DELETE){
			mesh.ps(p) = mesh.ps().back();
			mesh.ps().pop_back();
			--p;
		}
	}
}




template<class TRIMESH>
inline void fix_degenerate(TRIMESH& mesh) {
	for (uint p = 0; p < mesh.ps().size(); ++p) {
		if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;

		isDegenerateAndFix(mesh, p);
	}
}



// connect rev edges by elinks
template<class TRIMESH>
inline void connect(TRIMESH& mesh, uint e1, uint e2){
	auto& rev1 = mesh.elinks[e1];
	auto& rev2 = mesh.elinks[e2];
	bool link1 = rev1 != TRIMESH::IIDX_INT(-1);
	bool link2 = rev2 != TRIMESH::IIDX_INT(-1);
	if (!link1 || !link2){
		// no link
		if (link1) mesh.elinks[rev1] = TRIMESH::IIDX_INT(-1);
		if (link2) mesh.elinks[rev2] = TRIMESH::IIDX_INT(-1);
	}
	else{
		mesh.elinks[rev1] = rev2;
		mesh.elinks[rev2] = rev1;
	}

	rev1 = TRIMESH::IIDX_INT(-1);
	rev2 = TRIMESH::IIDX_INT(-1);
}

// mark degenerate triangles (based on vremap) as deleted, update elinks
// call fun() for each deleted triangle
template<class TRIMESH, class FUN>
inline void degenerate_delete_mark(TRIMESH& mesh, const FUN& fun = [](uint){}){
	ASS(mesh.isUsingPFlags());
	for (uint p = 0; p < mesh.ps().size(); ++p){
		if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;
		uint a = mesh.vremap[mesh.ps(p).idx(0)];
		uint b = mesh.vremap[mesh.ps(p).idx(1)];
		uint c = mesh.vremap[mesh.ps(p).idx(2)];
		if (a == b || b == c || c == a){
			mesh.pflags[p] |= TRIMESH::P_DELETE;
			fun(p);
		}
		if (a == b && b == c){}
		else if (mesh.isUsingELinks()){
			if (a == b){
				connect(mesh, p * 3 + 1, p * 3 + 2);
				mesh.ps(p)[0].copyAttributesFrom(mesh.ps(p)[1]); // optional attribute match
			}
			else if (b == c){
				connect(mesh, p * 3 + 2, p * 3 + 0);
				mesh.ps(p)[1].copyAttributesFrom(mesh.ps(p)[2]); // optional attribute match
			}
			else if (c == a){
				connect(mesh, p * 3 + 0, p * 3 + 1);
				mesh.ps(p)[2].copyAttributesFrom(mesh.ps(p)[0]); // optional attribute match
			}
		}
	}
}




template<class TRIMESH>
inline bool checkIndices(const TRIMESH& mesh) {
	for (uint p = 0; p < mesh.ps().size(); ++p) {
		if (mesh.pflags[p] & TRIMESH::P_DELETE) continue;
		if (mesh.ps(p).idx(0) >= mesh.vs().size()) return false;
		if (mesh.ps(p).idx(1) >= mesh.vs().size()) return false;
		if (mesh.ps(p).idx(2) >= mesh.vs().size()) return false;
	}
	return true;
}



// define HAS_vremap
HAS_MEMBER(vremap, vremap)



} // namespace ab






