//#define READ_NUM_TEST_CASES
//#define ASSERTS		// force asserts on online judge
//#define FASTIO
#define IOSTREAM
#include<templates/my.h>
typedef H1<1000000007> H;

#include<atablash/kd.h>
#include<ctime>

const int dims = 10;
const int points = 1000000;

inline vector<int> rnd(){
	vector<int> v(dims);
	F(i,dims) v[i] = rand()%10;
	return v;
}


void tc(){
	srand(time(0));
	
	KD<int> kd(dims);
	
	int t = clock();
	
	O < "Inserting " < points < " points... ";
	F(i,points){
		vector<int> p = rnd();
		kd.insert(p);
	}
	O < "OK. " < 1.0*(clock()-t)/CLOCKS_PER_SEC < 's' < N;
	
	vector<int> x = rnd();
	
	t = clock();
	
	vector<int> r = kd.closest(x).fi;
	
	O < "Search time: " < 1.0*(clock()-t)/CLOCKS_PER_SEC < "s" < N;
	
	O < "Closest point to ";
	F(i,min(dims,2)) O < x[i] < ' ';
	O < "is ";
	F(i,min(dims,2)) O < r[i] < ' ';
	O < N;
}


