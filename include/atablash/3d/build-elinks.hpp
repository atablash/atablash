#pragma once



#include <atablash/templates.hpp>


namespace ab{
	



// allow multiple triangles on the same edge. link them in pairs arbitrarily
template<bool useRemap, class TRIMESH, class VREMAP>
inline std::vector<ui32> buildELinks_detail(const TRIMESH& mesh, const VREMAP& vremap){
	Timer timer;
	bool isSolid = true;

	if (!useRemap) LOGW("buildELinks() not using vremap. assuming unique vertex positions.");
	
	std::vector<ui32> eLinks(mesh.ps().size()*3, ui32(-1));
	
	// multimap to allow multiple triangle pairs on the same edge
	std::unordered_multimap<pii,int> emap;
	for(uint i=0; i<mesh.ps().size(); ++i){
		auto ia = mesh.ps()[i][0];
		auto ib = mesh.ps()[i][1];
		auto ic = mesh.ps()[i][2];
		if (useRemap){
			ia = vremap[ia];
			ib = vremap[ib];
			ic = vremap[ic];
		}

		// asserts for single triangle pair at every edge - TEMPORARY! todo
		ASS(emap.find(pii(ia,ib)) == emap.end());
		ASS(emap.find(pii(ib,ic)) == emap.end());
		ASS(emap.find(pii(ic,ia)) == emap.end());

		emap.insert(std::make_pair(pii(ia, ib), 3 * i + 0));
		emap.insert(std::make_pair(pii(ib, ic), 3 * i + 1));
		emap.insert(std::make_pair(pii(ic, ia), 3 * i + 2));
	}
	
	for(uint i=0; i<mesh.ps().size(); ++i){
		int ia = mesh.ps()[i][0];
		int ib = mesh.ps()[i][1];
		int ic = mesh.ps()[i][2];
		if (useRemap){
			ia = vremap[ia];
			ib = vremap[ib];
			ic = vremap[ic];
		}

		auto r = emap.find(pii(ib, ia));
		if (r != emap.end()){
			eLinks[3 * i + 0] = r->second;
			emap.erase(r);
		}
		else{
			isSolid = false;
		}

		r = emap.find(pii(ic, ib));
		if (r != emap.end()){
			eLinks[3 * i + 1] = r->second;
			emap.erase(r);
		}
		else{
			isSolid = false;
		}

		r = emap.find(pii(ia, ic));
		if (r != emap.end()){
			eLinks[3 * i + 2] = r->second;
			emap.erase(r);
		}
		else{
			isSolid = false;
		}
	}

	// unmatched edges left
	if (!emap.empty()){
		isSolid = false;
	}

	if (!isSolid){
		LOGD("buildELinks() called on non-solid or solid with T-junctions. Some edges don't have links");
	}
	
	//LOGI("buildELinks_detail() completed in " + std::to_string(timer));

	return eLinks;
}

// vremap array explicitly given
template<class TRIMESH, class VREMAP>
inline std::vector<ui32> buildELinks(const TRIMESH& mesh, const VREMAP& vremap){
	return buildELinks_detail<true>(mesh, vremap);
}

// vremap not given but mesh.vremap exists
template<bool usingVRemap = true, class TRIMESH>
inline std::vector<ui32> buildELinks(const TRIMESH& mesh, typename std::enable_if<HAS_vremap<TRIMESH>::r,void>::type* = nullptr){
	if(usingVRemap && mesh.isUsingVRemap()) return buildELinks_detail<true>(mesh, mesh.vremap);
	else if (usingVRemap){
		LOGW("Performance: creating vremap ad-hoc.");
		auto vremap = hashWeldExact(mesh).vremap;
		return buildELinks_detail<true>(mesh, vremap);
	}
	else return buildELinks_detail<false>(mesh, Identity());
}

// vremap not given and mesh.vremap does not exist
template<bool usingVRemap = true, class TRIMESH>
inline std::vector<ui32> buildELinks(const TRIMESH& mesh, typename std::enable_if<!HAS_vremap<TRIMESH>::r, void>::type* = nullptr){
	if (usingVRemap){
		LOGW("Performance: creating vremap ad-hoc.");
		auto vremap = hashWeldExact(mesh).vremap;
		return buildELinks_detail<true>(mesh, vremap);
	}
	else return buildELinks_detail<false>(mesh, Identity());
}






// DEBUG
template<class TRIMESH>
inline bool checkELinks(const TRIMESH& mesh){
	return true;
	auto elinks = buildELinks(mesh);
	if (elinks.size() != mesh.elinks.size()){
		return false;
	}
	for(uint i=0; i<mesh.elinks.size(); ++i){
		if(mesh.elinks[i] != ui32(-1)){ // needed only if tear is enabled
			if (mesh.elinks[mesh.elinks[i]] != i){
				return false;
			}
			if (mesh.elinks[i] != elinks[i]){
				return false;
			}
		}
	}
	return true;
}
	
	
	
	
}


