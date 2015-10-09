#pragma once

// atablash
#include <atablash/linalg/3/vector.hpp>
#include <atablash/linalg/3/matrix.hpp>
#include <atablash/linalg/2/matrix.hpp>

// std
#include <vector>


namespace ab{


// build per-vertex normals, averaged over incident triangles
// polygons counter-clockwise, left handed coord system
//
// todo: weights proportional to angles? when averaging
//
template<class VS, class PS>
inline std::vector<v3f> buildNormalsExternal(const VS& vs, const PS& ps){
	std::vector<v3f> result(vs.size());
	
	for(int i=0; i<(int)ps.size(); ++i){
		ui32 ia = ps[i][0];
		ui32 ib = ps[i][1];
		ui32 ic = ps[i][2];
		
		v3f a = vs[ia].pos();
		v3f b = vs[ib].pos();
		v3f c = vs[ic].pos();
		
		v3f normal = (b-a)^(c-a);
		normal.normalize();
		
		result[ia] += normal;
		result[ib] += normal;
		result[ic] += normal;
	}
	
	for(int i=0; i<(int)vs.size(); ++i){
		result[i].normalize();
	}
	
	return result;
}

template<class VS, class PS>
inline void buildNormals(const VS& vs, const PS& ps, bool clear = true){
	
	if(clear) for(int i=0; i<(int)vs.size(); ++i){
		vs[i].normal = v3f(0,0,0);
	}
	
	for(int i=0; i<(int)ps.size(); ++i){
		ui32 ia = ps[i][0];
		ui32 ib = ps[i][1];
		ui32 ic = ps[i][2];
		
		v3f a = vs[ia];
		v3f b = vs[ib];
		v3f c = vs[ic];
		
		v3f normal = (b-a)^(c-a);
		normal.normalize();
		
		vs[ia].normal += normal;
		vs[ib].normal += normal;
		vs[ic].normal += normal;
	}
	
	for(int i=0; i<(int)vs.size(); ++i){
		vs[i].normal.normalize();
	}
}



// todo: check effects with both tangents sent to shader
template<class VS, class PS, class NS>
inline std::vector<v3f> buildTangentsExternal(const VS& vs, const PS& ps, const NS& ns){
	std::vector<v3f> result(vs.size());
	
	for(int i=0; i<(int)ps.size(); ++i){
		ui32 ia = ps[i][0];
		ui32 ib = ps[i][1];
		ui32 ic = ps[i][2];
		
		v3f a = vs[ia].pos();
		v3f b = vs[ib].pos();
		v3f c = vs[ic].pos();

		v2f uv_a = vs[ia].uv();
		v2f uv_b = vs[ib].uv();
		v2f uv_c = vs[ic].uv();

		auto trispace_to_uv = Matrix2<float>::Cols(uv_b - uv_a, uv_c - uv_a);
		if (trispace_to_uv.det() == 0) continue;
		
		// optim todo: invert() computes det() again
		Matrix32<float> mat = Matrix32<float>::Cols(b - a, c - a) * trispace_to_uv.invert();
		
		v3f tangentU = mat * Vector2<float>(1,0);
		//v3 tangentV = mat * Vector2<float>(0,1);
		
		result[ia] += tangentU;
		result[ib] += tangentU;
		result[ic] += tangentU;
	}
	
	for(int i=0; i<(int)vs.size(); ++i){
		result[i].normalize();
	}
	
	return result;
}




}

