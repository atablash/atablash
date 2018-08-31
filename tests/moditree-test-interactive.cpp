//#define READ_NUM_TEST_CASES
//#define ASSERTS		// force asserts on online judge
//#define OLD_COMPILER
//#define FASTIO
#include<templates/my.h>
typedef H1<1000000007> H;

int n,m,z;

AU(tree, (getITreeMod<int,int>(8, max2<int>,0,   _1 += _2, 0)));

void tc(){
	
	for(;;){
		string s = rstr;
		
		if(s=="add"){
			int fr=ri, to=ri, num=ri;
			tree.plus(fr,to,num);
		}
		else if(s=="get"){
			int fr=ri, to=ri;
			O < tree.get(fr,to) < N;
		}
		else if(s=="print"){
			E<tree<N;
		}
	}
}



