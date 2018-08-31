#pragma once



namespace ab{
	
	
	
	
	

//
//        C
//       /+\
//      / + \
//     /  +  \
//    /   +   \
//   A . .+. . B
//    \   +   /
//     \  +  /
//      \ + /
//       \+/
//        D
//
// removes edge AB, adds edge DC
// ABC -> ADC
// BAD -> BCD
//
// assumes ABC is degenerate
// 
//
struct EdgeFlipResult{
	// new edges
	uint BC = uint(-1);
	uint CD = uint(-1);
	uint DB = uint(-1);

	uint CA = uint(-1);
	uint AD = uint(-1);

	// new triangles
	uint ADC = uint(-1);
	uint BCD = uint(-1);
};
template<class TRIMESH>
inline EdgeFlipResult edgeFlip(TRIMESH& mesh, uint edge){
	ASS(mesh.isUsingELinks());

	ASS(checkELinks(mesh));

	static int dbg = 0;
	++dbg;

	// some helper vars
	uint ABC_old = edge / 3;
	uint AB      = edge;
	uint BC_old  = edge % 3 == 2 ? edge - 2 : edge + 1;
	uint CA      = edge % 3 == 0 ? edge + 2 : edge - 1;

	ASS(mesh.elinks[edge] != TRIMESH::IIDX_INT(-1));
	uint BA      = mesh.elinks[edge];
	uint BAD_old = BA / 3;
	uint AD_old  = BA % 3 == 2 ? BA - 2 : BA + 1;
	uint DB      = BA % 3 == 0 ? BA + 2 : BA - 1;

	uint BC_new = BA;
	uint AD_new = AB;
	uint DC     = BC_old;
	uint CD     = AD_old;


	// "swap" triangles
	mesh.indices[BC_old] = mesh.indices[DB]; // vertex attributes from lower triangle
	mesh.indices[AD_old] = mesh.indices[CA]; // vertex attributes from `C`! TODO: blend `A` and `B` and add a new vertex

	//ASS(isPolyOk(mesh, ABC_old)); // check new ADC
	//ASS(isPolyOk(mesh, BAD_old)); // check new BCD

	// update elinks
	{	// AD <-> DA
		auto x = mesh.elinks[AB] = mesh.elinks[AD_old];
		if (x != TRIMESH::IIDX_INT(-1)) mesh.elinks[x] = AB;
	}
	{	// BC <-> CB
		auto x = mesh.elinks[BA] = mesh.elinks[BC_old];
		if (x != TRIMESH::IIDX_INT(-1)) mesh.elinks[x] = BA;
	}
	//   DC <-> CD
	mesh.elinks[BC_old] = AD_old;
	mesh.elinks[AD_old] = BC_old;


	// update eflags (assume ABC is degenerate, take its flags)
	if (mesh.isUsingEFlags()){
		{	// add AB'a flags to AC
			auto AC = mesh.elinks[CA];
			if (AC != TRIMESH::IIDX_INT(-1)) mesh.eflags[AC] |= mesh.eflags[AB];
		}
		{	// add AB'a flags to CB
			auto CB = mesh.elinks[BC_new];
			if (CB != TRIMESH::IIDX_INT(-1)) mesh.eflags[CB] |= mesh.eflags[AB];
		}
		// add BA's flags to BC and CA
		mesh.eflags[CA]     = mesh.eflags[BA] | mesh.eflags[CA];
		mesh.eflags[BC_new] = mesh.eflags[BA] | mesh.eflags[BC_old];
		// AD flags
		mesh.eflags[AD_new] = mesh.eflags[AD_old];
		// clear flags from DC and CD
		mesh.eflags[DC] = 0;
		mesh.eflags[CD] = 0;
		// DB already ok
	}

	ASS(checkELinks(mesh));

	EdgeFlipResult r;
	r.BC = BC_new;
	r.CD = CD;
	r.DB = DB;
	r.CA = CA;
	r.AD = AD_new;

	r.ADC = AD_new / 3;
	r.BCD = BC_new / 3;

	return r;
}






// cuts `poly`th triangle ABC from `mesh` into 3 triangles (ABV, BCV, CAV)
// ABC -> ABV
struct TriCutResult{
	uint ABV = uint(-1);
	uint BCV = uint(-1);
	uint CAV = uint(-1);
	uint V = uint(-1);
};
template<class TRIMESH>
inline TriCutResult triCut(TRIMESH& mesh, uint poly, const Vector3<typename TRIMESH::POS_FLOAT>& V){
	ASS(mesh.isUsingELinks());
	//ASS(mesh.isUsingEFlags());

	auto& A = mesh.vs(mesh.indices[poly * 3 + 0]);
	auto& B = mesh.vs(mesh.indices[poly * 3 + 1]);
	auto& C = mesh.vs(mesh.indices[poly * 3 + 2]);

	ASS(V != A.pos());
	ASS(V != B.pos());
	ASS(V != C.pos());
	
	int ABV = poly;
	int BCV = mesh.ps().size();
	int CAV = mesh.ps().size()+1;
	int V_idx = mesh.vs().size();

	// debug
	//LOGD("triCut: inserting vertex " + std::to_string(V_idx) + "(" + std::to_string(V) + ") to triangle "
	//	+ std::to_string(poly) + " (" + std::to_string(mesh.ps(poly).triangle()) + ")");
	
	mesh.vs().push_back();
	mesh.vs().back().pos() = V;

	// new vertex attribs
	BarycentricCoords<TRIMESH::POS_FLOAT, float> coords(mesh.ps(poly).triangle());
	mesh.vs().back().lerpAttributes(A,B,C, coords(V));

	mesh.ps().resize(mesh.ps().size()+2);
	
	mesh.ps()[BCV].idx(0) = mesh.ps()[poly].idx(1);
	mesh.ps()[BCV].idx(1) = mesh.ps()[poly].idx(2);
	mesh.ps()[BCV].idx(2) = V_idx;
	//ASS(isPolyOk(mesh, BCV));
	
	mesh.ps()[CAV].idx(0) = mesh.ps()[poly].idx(2);
	mesh.ps()[CAV].idx(1) = mesh.ps()[poly].idx(0);
	mesh.ps()[CAV].idx(2) = V_idx;
	//ASS(isPolyOk(mesh, CAV));

	mesh.ps()[ABV].idx(2) = V_idx;
	//ASS(isPolyOk(mesh, ABV));

	if (mesh.isUsingPFlags()){
		mesh.pflags[CAV] = mesh.pflags[BCV] = mesh.pflags[poly];
	}
	

	//
	// update elinks
	//
	{	// BC <-> CB
		auto x = mesh.elinks[3*BCV] = mesh.elinks[3*poly + 1];
		if(x != TRIMESH::IIDX_INT(-1)) mesh.elinks[x] = 3*BCV;
	}
	{	// CA <-> AC
		auto x = mesh.elinks[3*CAV] = mesh.elinks[3*poly + 2];
		if(x != TRIMESH::IIDX_INT(-1)) mesh.elinks[x] = 3*CAV;
	}
	{	// BV <-> VB
		mesh.elinks[3*ABV + 1] = 3*BCV + 2;
		mesh.elinks[3*BCV + 2] = 3*ABV + 1;
	}
	{	// VA <-> AV
		mesh.elinks[3*ABV + 2] = 3*CAV + 1;
		mesh.elinks[3*CAV + 1] = 3*ABV + 2;
	}
	{	// CV <-> VC
		mesh.elinks[3*BCV + 1] = 3*CAV + 2;
		mesh.elinks[3*CAV + 2] = 3*BCV + 1;
	}
	//if (mesh.elinks.size() >= 61 && mesh.elinks[60] >= INT_MAX) __debugbreak();
	ASS(checkELinks(mesh));


	//
	// update eflags
	//
	if (mesh.isUsingEFlags()){
		mesh.eflags[BCV * 3 + 0] = mesh.eflags[ABV * 3 + 1];
		mesh.eflags[CAV * 3 + 0] = mesh.eflags[ABV * 3 + 2];
		mesh.eflags[ABV * 3 + 1] = 0;
		mesh.eflags[ABV * 3 + 2] = 0;
	}

	// vertex attributes are not blended here! need to call vertexAttribBlend()

	TriCutResult result;
	result.ABV = ABV;
	result.BCV = BCV;
	result.CAV = CAV;
	result.V = V_idx;
	return result;
}



//
//        C
//       / \
//      /   \
//     /     \
//    /       \
//   A---(V)---B
//    \       /
//     \     /
//      \   /
//       \ /
//        D
//
// cuts edge `edge` AB by inserting vertex V inside and splits two triangles ABC and ADB
// ABC -> AVC
// ADB -> ADV
//
// todo optim: avoid computing edge%3 - usually this is known when this function is called
//
// returns two sides of the new edge in triangle ABC
struct EdgeCutResult{
	// new edges
	uint AV = uint(-1);
	uint VC = uint(-1);
	uint CA = uint(-1);
	uint VB = uint(-1);
	uint BC = uint(-1);

	// new vertex
	uint V = uint(-1);
	uint V2 = uint(-1);
	
	// new polys
	uint AVC = uint(-1);
	uint VBC = uint(-1);
	uint ADV = uint(-1);
	uint DBV = uint(-1);
};
template<bool weld_VD_VC = true, class TRIMESH>
EdgeCutResult edgeCut(TRIMESH& mesh, uint edge,
	const Vector3<typename TRIMESH::POS_FLOAT>& V
){
	EdgeCutResult r;
	ASS(mesh.isUsingELinks());
	ASS(checkELinks(mesh));

	static int dbg = 0;

	uint AV = edge;
	uint VC = edge % 3 == 2 ? edge - 2 : edge + 1;
	uint CA = edge % 3 == 0 ? edge + 2 : edge - 1;

	uint VB;
	uint BC;

	ASS(V != mesh.vs(mesh.vremap[mesh.indices[AV]]).pos());
	ASS(V != mesh.vs(mesh.vremap[mesh.indices[VC]]).pos());
	ASS(V != mesh.vs(mesh.vremap[mesh.indices[CA]]).pos());


	// new vertex
	auto V_idx = (TRIMESH::IDX_INT) mesh.vs().size();
	mesh.vs().push_back();
	mesh.vs().back().pos() = V;

	r.V = V_idx;

	// debug
	//LOGD("edgeCut: inserting vertex " + std::to_string(V_idx) + "(" + std::to_string(V) + ") to edge "
	//	+ std::to_string(edge) + " (" + std::to_string(mesh.vs(mesh.indices[edge]).pos()) + ", " + std::to_string(mesh.vs(mesh.indices[VC]).pos()) + ";  C=="
	//	+ std::to_string(mesh.vs(mesh.indices[CA]).pos()) + ")");




	// blend V's vertex attributes
	auto iA = mesh.indices[AV];
	auto iB = mesh.indices[VC];
	const auto A = (v3f)mesh.vs(iA).pos();
	const auto B = (v3f)mesh.vs(iB).pos();
	const auto AB = B - A;
	float t = ((v3f)V - A)*AB / AB.lengthSqr();
	mesh.vs(V_idx).lerpAttributes(mesh.vs(iA), mesh.vs(iB), t);


	// VBC - new triangle
	uint VBC = mesh.ps().size();
	VB = VBC * 3 + 0;
	BC = VBC * 3 + 1;
	uint CV = VBC * 3 + 2;
	mesh.ps().push_back();
	mesh.ps()[VBC].idx(0) = V_idx;
	mesh.ps()[VBC].idx(1) = mesh.indices[VC];
	mesh.ps()[VBC].idx(2) = mesh.indices[CA];
	//ASS(isPolyOk(mesh, VBC));

	r.VBC = VBC;

	// ABC -> AVC
	mesh.indices[VC] = V_idx;
	//ASS(isPolyOk(mesh, VC / 3));

	if (mesh.isUsingPFlags()){
		mesh.pflags[VBC] = mesh.pflags[edge/3];
	}


	//
	// update elinks
	//
	{	// BC <-> CB
		auto x = mesh.elinks[VBC * 3 + 1] = mesh.elinks[VC];
		if (x != TRIMESH::IIDX_INT(-1)) mesh.elinks[x] = VBC * 3 + 1;
	}
	{	// VC <-> CV
		mesh.elinks[VC] = CV;
		mesh.elinks[CV] = VC;
	}


	//
	// update eflags
	//
	if (mesh.isUsingEFlags()){
		mesh.eflags[VBC * 3 + 0] = mesh.eflags[edge];
		mesh.eflags[VBC * 3 + 1] = mesh.eflags[VC];
		mesh.eflags[VC] = 0;
	}

	// cut neighbour triangle if we have elink
	uint VA = mesh.elinks[AV];
	if (VA != (ui32)(-1)){

		//LOGD("D==" + std::to_string(mesh.vs(mesh.indices[DV]).pos()));

		r.ADV = VA / 3;

		uint AD = VA % 3 == 2 ? VA - 2 : VA + 1;
		uint DV = VA % 3 == 0 ? VA + 2 : VA - 1;

		ASS(V != mesh.vs(mesh.vremap[mesh.indices[DV]]).pos()); // no V==D allowed

		//if (){
		//	ASS();
		//}

		// clone V if A or B is cloned
		bool doubleVertices = mesh.indices[AD] != mesh.indices[edge] || mesh.ps()[VBC].idx(1) != mesh.indices[VA];

		// cloned V
		auto V2_idx = V_idx;
		if (doubleVertices){
			V2_idx = mesh.vs().size();
			mesh.vs().push_back();
			mesh.vs().back().pos() = V;
			if (mesh.isUsingVRemap()) mesh.vremap[V2_idx] = V_idx;

			// blend V2's vertex attributes
			iA = mesh.indices[AD];
			iB = mesh.indices[VA];
			mesh.vs(V2_idx).lerpAttributes(mesh.vs(iA), mesh.vs(iB), ((v3f)V - A)*AB / AB.lengthSqr());
		}
		r.V2 = V2_idx;

		// DBV - new triangle
		uint DBV = mesh.ps().size();
		r.DBV = DBV;
		mesh.ps().push_back();
		mesh.ps()[DBV].idx(0) = mesh.indices[DV];
		mesh.ps()[DBV].idx(1) = mesh.indices[VA];
		mesh.ps()[DBV].idx(2) = V2_idx;
		//ASS(isPolyOk(mesh, DBV));

		// ADB -> ADV
		mesh.indices[VA] = V2_idx;
		//ASS(isPolyOk(mesh, VA / 3)); // triggers

		if (mesh.isUsingPFlags()){
			mesh.pflags[DBV] = mesh.pflags[edge / 3];
		}

		//
		// update elinks
		//
		{	// DB <-> BD
			auto x = mesh.elinks[DBV * 3 + 0] = mesh.elinks[DV];
			if (x != TRIMESH::IIDX_INT(-1)) mesh.elinks[x] = DBV * 3 + 0;
		}
		{	// DV <-> VD
			mesh.elinks[DV] = DBV * 3 + 2;
			mesh.elinks[DBV * 3 + 2] = DV;
		}
		{	// VB <-> BV
			mesh.elinks[VBC * 3 + 0] = DBV * 3 + 1;
			mesh.elinks[DBV * 3 + 1] = VBC * 3 + 0;
		}


		//
		// update eflags
		//
		if (mesh.isUsingEFlags()){
			mesh.eflags[DBV * 3 + 0] = mesh.eflags[DV];
			mesh.eflags[DV] = 0;
		}


		// debug
		if (doubleVertices){
			// condition possible but very rare
			// if not prepared test for this, assertion fail may indicate bug anyway
			//ASS(mesh.vs(V_idx).pos() != mesh.vs(V2_idx).pos() || mesh.vs(V_idx).uv() != mesh.vs(V2_idx).uv());
		}
	}

	ASS(checkELinks(mesh));
	++dbg;

	r.AV = AV;
	r.VC = VC;
	r.CA = CA;
	r.VB = VB;
	r.BC = BC;
	
	
	r.AVC = AV/3;
	
	return r;
}








// flips edge if V==C or V==D
// cuts edge at V otherwise
//
// todo: test representant vertex index instead of V position
template<class TRIMESH>
EdgeCutResult edgeCutOrFlip(TRIMESH& mesh, uint edge,
	const Vector3<typename TRIMESH::POS_FLOAT>& V
	){
	ASS(mesh.isUsingVRemap());
	ASS(mesh.isUsingELinks());

	EdgeCutResult result;

	// V == C?
	uint AB = edge;
	uint CA = edge % 3 == 0 ? edge + 2 : edge - 1;
	if (V == mesh.vs(mesh.vremap[mesh.indices[CA]]).pos()){
		auto r = edgeFlip(mesh,AB);
		result.ADV = r.ADC;
		result.DBV = r.BCD;
		// todo: map edges?
		return result;
	}

	// V == D?
	uint BA = mesh.elinks[AB];
	if (BA != typename TRIMESH::IIDX_INT(-1)){
		uint DB = BA % 3 == 0 ? BA + 2 : BA - 1;
		if (V == mesh.vs(mesh.vremap[mesh.indices[DB]]).pos()){
			auto r = edgeFlip(mesh,BA);
			result.AVC = r.BCD;
			result.VBC = r.ADC;
			// todo: map edges?
			return result;
		}
	}

	return edgeCut(mesh, edge, V);
}



// not legal if V==C or V==D
template<class TRIMESH>
bool edgeCutLegal(TRIMESH& mesh, uint edge,
	const Vector3<typename TRIMESH::POS_FLOAT>& V
){
	ASS(mesh.isUsingVRemap());
	ASS(mesh.isUsingELinks());

	// V == C?
	uint AB = edge;
	uint CA = edge % 3 == 0 ? edge + 2 : edge - 1;
	if (V == mesh.vs(mesh.vremap[mesh.indices[CA]]).pos()){
		return false;
	}

	// V == D?
	uint BA = mesh.elinks[AB];
	if (BA != typename TRIMESH::IIDX_INT(-1)){
		uint DB = BA % 3 == 0 ? BA + 2 : BA - 1;
		if (V == mesh.vs(mesh.vremap[mesh.indices[DB]]).pos()){
			return false;
		}
	}

	return true;
}





}