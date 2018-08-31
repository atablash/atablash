#pragma once


#include <atablash/3d/stickify.hpp>







namespace ab{








// store result in `a`
// leaves `b` in unknown state
template<class TRIMESH>
void booleanSubtract(TRIMESH& a, TRIMESH& b){
	typedef typename TRIMESH::POS_FLOAT F;
	
	flipPolys(b);
	append(a,b);
	
	a.enableVRemap();
	a.enableELinks();
	a.enablePFlags();

	stickify(a);
	//stickify(a);
}






}