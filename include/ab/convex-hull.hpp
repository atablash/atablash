#pragma once

#include<atablash/templates.hpp>
#include<atablash/linalg/2/matrix.hpp>

namespace ab{


// separate struct to deal with "mangling typeof"
template<class POINT_ITER>
struct convexHull_types{
	typedef typename RR<DECL(*POINT_ITER())>::R POINT;
};


// doesn't include points on boundary of resulting convex hull (returns always strictly convex hull)
// alreadySorted: set to true if input is sorted by (x,y) (first by `x`, then by `y`)
// assumes operator< sorts lexicographically by (x,y)
template<class POINT_ITER>
inline std::vector<typename convexHull_types<POINT_ITER>::POINT>
convexHull(const POINT_ITER& fr, const POINT_ITER& to, bool alreadySorted = false){
	typedef typename convexHull_types<POINT_ITER>::POINT POINT;
	typedef DECL(POINT_ITER()->x) T;
	
	if(!alreadySorted){
		std::vector<POINT> copy(fr,to);
		std::sort(copy.begin(), copy.end());//,
			//[](const POINT& a, const POINT& b){if(a.x==b.x)return a.y < b.y;return a.x < b.x;});
		return convexHull(copy.begin(), copy.end(), true);
	}
	
	std::vector<POINT> result;
	if(fr == to) return result;
	
	result.push_back(*fr);
	POINT_ITER iter = fr;
	++iter;
	if(iter == to) return result;
	
	// left to right
	for(; iter != to; ++iter){
		while(result.size()>1){
			if(Matrix2<T>::rows(
				result.back() - result[result.size()-2],
				*iter - result[result.size()-2]
			).det() > 0) break;
			
			result.pop_back();
		}
		result.push_back(*iter);
	}
	
	// right to left
	ASS(iter == to);
	size_t sizeBeforePass = result.size();
	for(;;){
		--iter;
		while(result.size() > sizeBeforePass){
			if(Matrix2<T>::rows(
				result.back() - result[result.size()-2],
				*iter - result[result.size()-2]
			).det() > 0) break;
			
			result.pop_back();
		}
		
		if(iter == fr)break;
		result.push_back(*iter);
	}
	return result;
}

	
	
	
}


