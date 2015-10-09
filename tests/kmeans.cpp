//#define READ_NUM_TEST_CASES
//#define ASSERTS		// force asserts on online judge
//#define FASTIO
#define IOSTREAM
#include<templates/my.h>
typedef H1<1000000007> H;

#include<atablash/optim/kmeans.h>
#include<atablash/stlext.h>
#include<ctime>




const int dims = 2;
const int points = 1000;

const int clusters = 3;


const int size = 70;

inline vector<double> rnd(){
	vector<double> v(dims);
	F(i,dims) v[i] = rand()%size;
	return v;
}

char m[size][size+1];

void tc(){
	srand(time(0));
	
	
	F(i,size) F(j,size) m[i][j] = ' ';
	
	vector<vector<double> > pts;
	
	F(i,points){
		vector<double> p = rnd();
		pts.pub(p);
		m[si p[0]][si p[1]] = 'x';
	}
	
	auto res = kmeans(pts, clusters, dims);
	
	
	F(i,res.se.siz){
		m[si pts[i][0]][si pts[i][1]] = 'A' + res.se[i];
	}
	
	
	F(i,size) O < m[i] < N;
}



