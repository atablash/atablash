//#define READ_NUM_TEST_CASES
//#define ASSERTS		// force asserts on online judge
#include<templates/my.h>

#include<atablash/linalg/matrix.h>
#include<atablash/linalg/simplex.h>

inline void tc2()
{
	Matrix<double> mat(3,6);
	
	// b
	mat(0,0) = 10;
	mat(1,0) = 15;
	
	// A
	mat(0,1) = 3;
	mat(0,2) = 2;
	mat(0,3) = 1;
	mat(0,4) = 1;
	mat(0,5) = 0;
	
	mat(1,1) = 2;
	mat(1,2) = 5;
	mat(1,3) = 3;
	mat(1,4) = 0;
	mat(1,5) = 1;
	
	mat(2,1) = -2;
	mat(2,2) = -3;
	mat(2,3) = -4;
	
	E << "input" << N;
	E << mat << N;
	
	double result = Simplex<double>::dantzig(mat);
	
	
	O << "result: " << result << N;
	// -20
}


inline void tc()
{
	Matrix<double> mat(3,4);
	
	// b
	mat(0,0) = 10;
	mat(1,0) = 15;
	
	// A
	mat(0,1) = 3;
	mat(0,2) = 2;
	mat(0,3) = 1;
	
	mat(1,1) = 2;
	mat(1,2) = 5;
	mat(1,3) = 3;
	
	mat(2,0) = 0;
	mat(2,1) = -2;
	mat(2,2) = -3;
	mat(2,3) = -4;
	
	E << "input" << N;
	E << mat << N;
	
	double result = Simplex<double>::dantzig(mat);
	
	
	O << "result: " << result << N;
	// -18.57142857142857295116
}

