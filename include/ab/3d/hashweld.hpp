#pragma once


#include <atablash/linalg/2/vector.hpp>
#include <atablash/linalg/3/vector.hpp>
#include <atablash/types.hpp>
#include <atablash/stlext.hpp>
#include <atablash/union-find.hpp>


namespace ab{



// welds vertices of const TRIMESH by position only
// returns vRemap
struct HashWeldResult{
	ab::UnionFind<ui32> vremap;
	ui32 numUniqueVertices;
};
template<class TRIMESH>
inline HashWeldResult hashWeldExact(const TRIMESH& mesh){
	HashWeldResult result;
	result.vremap.resize(mesh.vs().size());
	
	typedef Vector3<typename TRIMESH::POS_FLOAT> Key;
	umap<Key, ui32> weldMap;
	
	for (ui32 i = 0; i<mesh.vs().size(); ++i){
		Key key(mesh.vs()[i].pos());
		auto insertResult = weldMap.insert({key,i});
		result.vremap[i] = insertResult.first->second;
	}
	
	result.numUniqueVertices = weldMap.size();

	LOGD("hashWeldExact: " + std::to_string(mesh.vs().size()) + " -> " + std::to_string(weldMap.size()));
	
	return result;
}






// welds vertices with same pos and uv
template<class TRIMESH>
inline void hashWeldExactOptimize(TRIMESH& mesh)
{
	auto old_vs_size = mesh.vs().size();
	typedef std::pair<Vector3<typename TRIMESH::POS_FLOAT>,Vector2<float>> Key;
	umap<Key,int> weldMap;
	
	std::vector<ui32> remap(mesh.vs().size());
	for(int i=0; i<(int)remap.size(); ++i) remap[i] = i;
	
	bool justMoved = false;
	for(int i=0; i<(int)mesh.vs().size(); ++i){
		Key key(mesh.vs()[i].pos(), mesh.vs()[i].uv());
		auto insertResult = weldMap.insert({key,i});
		if(!insertResult.second){
			remap[justMoved ? mesh.vs().size() : i] = insertResult.first->second;
			mesh.vs()[i] = mesh.vs().back();
			mesh.vs().pop_back();
			if(i != mesh.vs().size()) remap[mesh.vs().size()] = i;
			--i;
			justMoved = true;
		}
		else justMoved = false;
	}
	
	// apply remap
	for(int i=0; i<(int)mesh.ps().size(); ++i){
		mesh.ps(i).idx(0) = remap[mesh.ps()[i].idx(0)];
		mesh.ps(i).idx(1) = remap[mesh.ps()[i].idx(1)];
		mesh.ps(i).idx(2) = remap[mesh.ps()[i].idx(2)];
	}

	LOGI("hashWeldExactOptimize: " + std::to_string(old_vs_size) + " -> " + std::to_string(mesh.vs().size()));
}





// welds vertices with almost same pos and uv
template<class TRIMESH>
inline void hashWeldOptimize(TRIMESH& mesh,
	const typename TRIMESH::POS_FLOAT& weldPosEpsilon = (typename TRIMESH::POS_FLOAT)0.001,
	const float& weldUVEpsilon=0.001f,
	const ab::Vector3<typename TRIMESH::POS_FLOAT>& posOffset = {0.18573454f, 0.30495732f, 0.6384303f},
	const ab::Vector2<float>& uvOffset = {0.3746012f, 0.3842012f})
{
	typedef std::pair<Vector3<i32>,Vector2<i32>> Key;
	umap<Key,int> weldMap;
	
	typename TRIMESH::POS_FLOAT weldPosEpsilonRec = 1 / weldPosEpsilon;
	float weldUVEpsilonRec = 1 / weldUVEpsilon;
	
	std::vector<ui32> remap(mesh.vs().size());
	for(int i=0; i<(int)remap.size(); ++i) remap[i] = i;
	
	bool justMoved = false;
	for(int i=0; i<(int)mesh.vs().size(); ++i){
		ab::Vector3<i32> ipos = (ab::Vector3<i32>)(mesh.vs()[i].pos() * weldPosEpsilonRec + posOffset);
		ab::Vector2<i32> iuv  = (ab::Vector2<i32>)(mesh.vs()[i].uv()  * weldUVEpsilonRec  + uvOffset);
		Key key(ipos,iuv);
		auto insertResult = weldMap.insert({key,i});
		if(!insertResult.second){
			remap[justMoved ? mesh.vs().size() : i] = insertResult.first->second;
			mesh.vs()[i] = mesh.vs().back();
			mesh.vs().pop_back();
			if(i != mesh.vs().size()) remap[mesh.vs().size()] = i;
			--i;
			justMoved = true;
		}
		else justMoved = false;
	}
	
	// apply remap
	for(int i=0; i<(int)mesh.ps().size(); ++i){
		mesh.ps()[i][0] = remap[mesh.ps()[i][0]];
		mesh.ps()[i][1] = remap[mesh.ps()[i][1]];
		mesh.ps()[i][2] = remap[mesh.ps()[i][2]];
	}
}






}



