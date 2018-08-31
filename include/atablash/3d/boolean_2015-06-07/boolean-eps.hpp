#pragma once



template<class> struct EpsBoolean{};


template<> struct EpsBoolean < float > {
	typedef float F;
	F tricutOrient = (F)1e-5;
};

template<> struct EpsBoolean < double > {
	typedef float F;
	F tricutOrient = (F)1e-5;
};



template<class> struct Triangle3Eps{};

template<> struct Triangle3Eps<float>{
	typedef float F;
	// if distance to the plane is less, assume point is on plane
	// for `float`: 1e-5 not working on BooleanTest_box_overlap_1, test 0 (seed 0+1); returned `inside` instead of `edge`
	// curr		nt tested: 1e-4 5e-3?
	F pointOnPlane = (F)1e-4;

	F pointOnPlaneSmallMult = /* eps.pointOnPlane times */ (F)1e-1;

	// if segment resulti		g from intersection is shorter, as		ume no collision
	// current t		sted: 1e-6 ... 1e-3
	F pointCollision = (F)1e-4;

	// weld triangle contours (usua		ly very close edges) to intersect at the same point
	// for `float`: 1e-5 		ot working for some tests with 		imple rotated boxes
	// cur		ent tested: 1e-4, 1e-3
	F contourWeld = (F)1e-4;

	F degenerate = (F)1e-12;
};
template<> struct Triangle3Eps<double>{
	typedef double F;
	F pointOnPlane = (F)1e-8;
	F pointOnPlaneSmallMult = /* eps.pointOnPlane times */ (F)1e-1;
	F pointCollision = (F)1e-8;
	F contourWeld = (F)1e-8;
	F degenerate = (F)1e-16;
};









template<> struct Triangle3Eps<long double>{
	typedef long double F;
	F pointOnPlane = (F)1e-8;
	F pointOnPlaneSmallMult = /* eps.pointOnPlane times */ (F)1e-1;
	F pointCollision = (F)1e-8;
	F contourWeld = (F)1e-8;
	F degenerate = (F)1e-12;
};
template<> struct EpsBoolean < long double > {
	typedef float F;
	F tricutOrient = (F)1e-8;
};






