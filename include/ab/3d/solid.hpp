#pragma once







namespace ab{


template<bool checkShape = true, class TRIMESH>
inline bool checkDegeneratePolys(const TRIMESH& mesh) {
	for (uint i = 0; i < mesh.ps().size(); ++i) {
		if (mesh.isUsingPFlags() && (mesh.pflags[i] & TRIMESH::P_DELETE)) continue;

		if (!isPolyOk<checkShape>(mesh, i)) {
			LOGI("checkSolid: degenerate poly: " + std::to_string(i));
			return false;
		}
	}
	return true;
}


template<bool usingVRemap, class TRIMESH, class VREMAP>
bool checkSolid_detail(const TRIMESH& mesh, const VREMAP& vremap){
	std::unordered_multiset<pii> edges;
	for(uint i=0; i<mesh.ps().size(); ++i){
		for (int j = 0; j < 3; ++j){
			auto a = mesh.ps(i)[j].idx();
			auto b = mesh.ps(i)[j==2 ? 0 : j+1].idx();
			if (usingVRemap){
				a = vremap[a];
				b = vremap[b];
			}
			auto itr = edges.find(pii(b,a));
			if(itr == edges.end()) edges.insert(pii(a,b));
			else{
				edges.erase(itr);
			}
		}
	}

	if (!edges.empty()) {
		for (auto& e : edges) {
			LOGI("checkSolid: not matched edge: " + std::to_string(e.first) + " " + std::to_string(e.second));
		}
		return false;
	}


	if (!checkDegeneratePolys<false>(mesh)) return false;
	

	{	// isolated vertices
		std::vector<ui8> vok(mesh.vs().size());
		for (uint i = 0; i < mesh.ps().size(); ++i){
			vok[mesh.ps(i)[0].idx()] = true;
			vok[mesh.ps(i)[1].idx()] = true;
			vok[mesh.ps(i)[2].idx()] = true;
		}
		//if (std::count(vok.begin(), vok.end(), true) != vok.size()) {
		for (uint i = 0; i < vok.size(); ++i) if(!vok[i]) {
			LOGI("checkSolid: found isolated vertex: " + std::to_string(i));
			return false;
		}
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



