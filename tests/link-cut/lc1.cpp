//#define READ_NUM_TEST_CASES
//#define ASSERTS		// force asserts on online judge
//#define FASTIO
#include<templates/my.h>
typedef H1<1000000007> H;


#include<atablash/link-cut.h>

template<int defaultVal = 0>
struct Int{
	Int() : a(defaultVal) {}
	Int(int _a) : a(_a){}
	
	operator int&(){return a;}
	operator const int&()const{return a;}
	
private:
	int a;
};

namespace std{
template<int defaultVal> struct hash<Int<defaultVal> >{
	size_t operator()(const Int<defaultVal>& mi)const{
		return hash<int>()(mi);
	}
};
}




typedef Int<-INT_MAX/2> Intt;
AU(lc, (getLinkCut<Intt,int>( max2<Intt>,  _1 += _2,  _1 += _2,  -_1)));


void tc(){
	int n=ri;
	F(i,n) lc.set(i,0);
	int q=ri;
	
	map<string,int> types;
	types["link"]=0;
	types["cut"]=1;
	types["makeroot"]=2;
	types["set"]=3;
	types["add"]=4;
	types["addpath"]=5;
	types["addpathroot"]=6;
	types["addtree"]=7;
	types["get"]=8;
	types["getpath"]=9;
	types["getpathroot"]=10;
	types["gettree"]=11;
	types["debug"]=100;
	
	F(i,q){
		int type = types[rstr];
		
		if(type==0){
			int u=ri, v=ri;
			
			lc.link(u,v);
		}
		else if(type==1){
			int u = ri;
			
			lc.cut(u);
		}
		else if(type==2){
			int v = ri;
			lc.makeRoot(v);
		}
		else if(type==3){
			int v = ri;
			int what = ri;
			lc.set(v, what);
		}
		else if(type==4){
			int v = ri;
			int what = ri;
			lc.mod(v, what);
		}
		else if(type==5){
			int u=ri, v=ri, what=ri;
			
			lc.modPath(u, v, what);
		}
		else if(type==6){
			int u = ri;
			int what = ri;
			
			// to root
			lc.modPath(u, what);
		}
		else if(type==7){
			int u = ri;
			int what = ri;
			
			lc.modTree(u, what);
		}
		else if(type==8){
			int u = ri;
			O < lc.get(u) < N;
		}
		else if(type==9){
			int u=ri, v=ri;
			O < lc.path(u, v) < N;
		}
		else if(type==10){
			int u = ri;
			O < lc.path(u) < N;
		}
		else if(type==11){
			int u = ri;
			O < lc.tree(u) < N;
		}
		else if(type==100){
			int u = ri;
			lc.debug(u);
		}
	}
}
