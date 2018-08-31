//#define READ_NUM_TEST_CASES
//#define ASSERTS		// force asserts on online judge
//#define OLD_COMPILER
//#define FASTIO
#include<templates/my.h>
typedef H1<1000000007> H;



void tc(){

	{
		pii p(69,83);
		O < "pii p(69,83);   bv(&pii::first, p)(0) ===> " < bv(&pii::first, p)(0) < N;
	}

	{
		O < "bv(&pii::first, pii(69,83))(0) ===> " < bv(&pii::first, pii(69,83))(0) < N;
	}
	
	{
		pii p(69,83);
		O < "pii p(69,83);   bv(&pii::second, _1)(p) ===> " < bv(&pii::second, _1)(p) < N;
	}
	
	{
		O < "bv(&pii::second, _1)(pii p(69,83)) ===> " < bv(&pii::second, _1)(pii(69,83)) < N;
	}
	
	{
		O < "bv(_1,_2)(&pii::second, pii(69,83)) ===> " < bv(_1,_2)(&pii::second, pii(69,83)) < N;
	}
	
	{
		O < "(2*_1 < _2)(5,9) ===> " < (2*_1 < _2)(5,9) < N;
	}
	
	{
		pii p(69,83);
		O < "pii p(69,83);   (bv(&pii::fi,_1) + 1000)(p) ===> " < (bv(&pii::fi,_1) + 1000)(p) < N;
	}
	
	
	
	int n = 20;
	vector<pii> v;
	F(i,n)v.pub(mp(rand()%10, rand()%10));
	
	//sort(ALL(v), _1 > _2);
	sort(ALL(v), bv(&pii::se,_1) < bv(&pii::se,_2) );
	
	FE(it,v)O<it->fi<' '<it->se<N;
	//for_each(ALL(v), O<_1<' '<_2<N);
	
	
}



