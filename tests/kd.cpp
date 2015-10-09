//#define READ_NUM_TEST_CASES
//#define ASSERTS		// force asserts on online judge
//#define FASTIO
#include<templates/my.h>
typedef H1<1000000007> H;

#include<atablash/kd.h>
#include<ctime>

const int dims = 2;
const int points = 100;


const int size = 80;

inline vector<int> rnd(){
	vector<int> v(dims);
	F(i,dims) v[i] = rand()%size;
	return v;
}

char m[size][size+1];

void tc(){
	srand(time(0));
	
	KD<int> kd(dims);
	
	F(i,size) F(j,size) m[i][j] = ' ';
	
	O < "Inserting " < points < " points... ";
	F(i,points){
		vector<int> p = rnd();
		kd.insert(p);
		m[p[0]][p[1]] = 'x';
	}
	O < "OK." < N;
	
	vector<int> x = rnd();
	
	if(m[x[0]][x[1]] == ' ') m[x[0]][x[1]] = '?';
	else m[x[0]][x[1]] = '!';
	
	vector<int> r = kd.closest(x).fi;
	
	m[r[0]][r[1]] = 'O';
	
	O < "Closest point to ";
	F(i,dims) O < x[i] < ' ';
	O < "is ";
	F(i,dims) O < r[i] < ' ';
	O < N;
	
	F(i,size) O < m[i] < N;
}


