#pragma once

#include<atablash/linalg/matrix.hpp>

namespace ab{

template<typename F = double, class MAT = Matrix<F>>
struct Simplex{



// Original Dantzig's algorithm (slow)
//
// minimize c*x   w.r.t.:
//   A*x = b
//   x >= 0
//
//     +---+
// a = |b A|
//     |0 c|
//     +---+
//
static F dantzig(MAT& a, const F& epsilon = numeric_limits<F>::epsilon()*8.0){
	int constrs = a.size().first-1;
	int vars = a.size().second-1;
	//D if(a.capacityN() < n+1) L("Performance warning");
	//a.resizeN(n+1);

	// find any base
	DE if(a.size().first + 1 > a.capacity().first ||
		a.size().second + constrs > a.capacity().second) LOG("Performance warning");
	a.resize(a.size().first + 1, a.size().second + constrs);

	for(int j=0; j<constrs; ++j){
		a(j,1+vars+j) = 1;
	}
	// price out (sum all rows)
	for(int i=0; i<constrs; ++i){
		for(int j=0; j<1+vars; ++j){
			a(1+constrs,j) -= a(i,j);
		}
	}
	E<"PHASE 1"<EN;
	F r = dantzig_canonical<2>(a,epsilon);

	// no solution
	if(fabs(r) > epsilon) return numeric_limits<F>::quiet_NaN();

	E<"PHASE 2"<EN;
	a.resize(1+constrs, 1+vars);
	return dantzig_canonical<1>(a,epsilon);
}

template<int noPivotRowsNum = 1>
static F dantzig_canonical(MAT& a, const F& epsilon = numeric_limits<F>::epsilon()*8.0){
	int constrs = a.size().first-1;
	int vars = a.size().second-1;

	for(;;){
		E<a<EN;
		// entering var selection
		int entering = -1;
		for(int j=1; j<1+vars; ++j){
			F val = a(constrs,j);
			if(val < -epsilon /*0*/){
				entering = j;	// pierwsza lepsza
				break;
			}
		}

		E<"entering "<entering<EN;

		// we have a solution
		if(entering == -1) return -a(constrs,0);

		// leaving var selection
		int leaving = -1;
		F m = numeric_limits<F>::max();
		for(int i=0; i<a.size().first - noPivotRowsNum; ++i){
			if(a(i,entering) <= epsilon /* 0 */) continue;

			F val = a(i,0) / a(i,entering);
			if(val < m){
				m = val;
				leaving = i;
			}
		}

		E<"leaving "<leaving<EN;

		// unbounded
		if(leaving == -1) return -numeric_limits<F>::max();	// minimum

		//pivot
		a(leaving) *= 1.0 / a(leaving,entering);
		for(int i=0; i<leaving; ++i){
			F mult = a(i,entering);
			for(int j=0; j<1+vars; ++j){
				a(i,j) -= a(leaving,j)*mult;
			}
		}
		for(int i=leaving+1; i<constrs+1; ++i){
			F mult = a(i,entering);
			for(int j=0; j<1+vars; ++j){
				a(i,j) -= a(leaving,j)*mult;
			}
		}
	}
}







};

}
