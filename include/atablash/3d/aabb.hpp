#pragma once




template<class VECTOR>
struct AABB{
	VECTOR fr,to;
	
	template<class F>
	inline void grow(const F& a){
		for(uint i=0; i<fr.size(); ++i) fr[i] -= a;
		for(uint i=0; i<to.size(); ++i) to[i] += a;
	}
};


	
	
template<class VECTOR>
bool intersect(const AABB<VECTOR>& a, const AABB<VECTOR>& b){
	ASS(a.fr.size() == a.to.size());
	ASS(b.fr.size() == b.to.size());
	ASS(b.fr.size() == a.fr.size());
	const auto dims = a.fr.size();
	for(uint i=0; i<dims; ++i){
		if(a.to[i] <= b.fr[i]  ||  b.to[i] <= a.fr[i]) return false;
	}
	return true;
}


template<class VECTOR>
bool intersect(const AABB<VECTOR>& a, const VECTOR& point){
	ASS(a.fr.size() == a.to.size());
	ASS(point.size() == a.fr.size());
	const auto dims = a.fr.size();
	for(uint i=0; i<dims; ++i){
		if(a.to[i] <= point[i]  ||  point[i] < a.fr[i]) return false;
	}
	return true;
}




