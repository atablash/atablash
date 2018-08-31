#pragma once




#include "trimesh-common.hpp"



namespace ab{




template<bool usingVRemap, class TRIMESH, class VREMAP>
bool checkSolid_detail(const TRIMESH& mesh, const VREMAP& vremap){
	uset<pii> edges;
	for(uint i=0; i<mesh.ps().size(); ++i){
		for (int j = 0; j < 3; ++j){
			auto a = mesh.ps()[i][j];
			auto b = mesh.ps()[i][j==2 ? 0 : j+1];
			if (usingVRemap){
				a = vremap[a];
				b = vremap[b];
			}
			auto itr = edges.find(pii(b,a));
			if(itr == edges.end()){
				auto r = edges.insert(pii(a,b));
				if(!r.second){
					break;
				}
			}
			else{
				edges.erase(itr);
			}
		}
	}
	if (!edges.empty()) return false;


	// degenerate triangles
	for (uint i = 0; i < mesh.ps().size(); ++i){
		if (!isPolyOk(mesh,i)) return false;
	}


	{	// isolated vertices
		std::vector<bool> vok(mesh.vs().size());
		for (uint i = 0; i < mesh.ps().size(); ++i){
			vok[mesh.ps(i)[0]] = true;
			vok[mesh.ps(i)[1]] = true;
			vok[mesh.ps(i)[2]] = true;
		}
		if (std::count(vok.begin(), vok.end(), true) != vok.size()) return false;
	}


	return true;
}

// vremap array explicitly given
template<class TRIMESH, class VREMAP>
inline bool checkSolid(const TRIMESH& mesh, const VREMAP& vremap){
	return checkSolid_detail<true>(mesh, vremap);
}


// vremap not given but mesh.vremap exists
template<bool usingVRemap = true, class TRIMESH>
inline bool checkSolid(const TRIMESH& mesh, typename std::enable_if<HAS_vremap<TRIMESH>::r, void>::type* = nullptr){
	if (usingVRemap && mesh.isUsingVRemap()) return checkSolid_detail<true>(mesh, mesh.vremap);
	else if (usingVRemap){
		LOGW("Performance: creating vremap ad-hoc.");
		auto vremap = hashWeldExact(mesh).vremap;
		return checkSolid_detail<true>(mesh, vremap);
	}
	else return checkSolid_detail<false>(mesh, Identity());
}

// vremap not given and mesh.vremap does not exist
template<bool usingVRemap = true, class TRIMESH>
inline bool checkSolid(const TRIMESH& mesh, typename std::enable_if<!HAS_vremap<TRIMESH>::r, void>::type* = nullptr){
	if (usingVRemap){
		LOGW("Performance: creating vremap ad-hoc.");
		auto vremap = hashWeldExact(mesh).vremap;
		return checkSolid_detail<true>(mesh, vremap);
	}
	else checkSolid_detail<false>(mesh, Identity());
}




}



