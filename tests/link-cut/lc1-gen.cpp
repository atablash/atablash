//#define FASTIO
#define GEN
#include<templates/my.h>
typedef H1<1000000007> H;


vector<set<int> > tos;
vi parent;

vector<ll> vals;

inline int findroot(int v){
	while(parent[v]!=-1)v = parent[v];
	return v;
}

inline bool sametree(int u, int v){
	return findroot(u) == findroot(v);
}

inline void makeroot(int u){
	if(parent[u]==-1)return;
	
	makeroot(parent[u]);
	parent[parent[u]] = u;
	parent[u] = -1;
}

bool addpath(int u, int v, int what, int from = -1){
	if(u==v){
		vals[v] += what;
		return true;
	}
	
	FE(it,tos[u]) if(*it != from){
		bool r = addpath(*it,v,what,u);
		if(r){
			vals[u] += what;
			return true;
		}
	}
	return false;
}

void addpathroot(int u, int what){
	if(parent[u]!=-1)addpathroot(parent[u], what);
	vals[u] += what;
}

void addtree(int u, int what){
	vals[u] += what;
	FE(it,tos[u])if(*it != parent[u]){
		addtree(*it, what);
	}
}

void tc(){
	int n = 100;
	int q = 25000;
	
	if(argc >= 3) q = atoi(argv[2]);
	
	O<n<" "<q<N;
	
	tos.resize(n);
	parent.resize(n);
	vals.resize(n);
	
	F(i,n)parent[i]=-1;
	
	F(i,q){
		int type = RND%12;
		
		if(type==0){
			int u=0, v=0;
			int iters = 0;
			while(sametree(u,v)){
				if(iters++ == 100)break;
				u = RND%n;
				v = RND%n;
			}
			if(sametree(u,v)){
				--i;
				continue;
			}
			u = findroot(u);
			parent[u] = v;
			tos[u].insert(v);
			tos[v].insert(u);
			O < "link " < u < " " < v < N;
		}
		else if(type==1){
			int u = 0;
			int iters = 0;
			do{
				if(iters++ == 100)break;
				u = RND%n;
			} while(parent[u]==-1);
			
			if(parent[u]==-1){
				--i;
				continue;
			}
			
			tos[u].erase(parent[u]);
			tos[parent[u]].erase(u);
			parent[u] = -1;
			O < "cut " < u < N;
		}
		else if(type==2){
			int v = RND%n;
			makeroot(v);
			
			O < "makeroot " < v < N;
		}
		else if(type==3){
			int v = RND%n;
			int what = RND%100;
			vals[v] = what;
			
			O < "set " < v < " " < what < N;
		}
		else if(type==4){
			int v = RND%n;
			int what = RND%100;
			vals[v] += what;
			
			O < "add " < v < " " < what < N;
		}
		else if(type==5){
			int u,v;
			do{
				u=RND%n;
				v=RND%n;
			}while(!sametree(u,v));
			
			int what = RND%100;
			
			bool r = addpath(u,v,what);
			ASS(r);
			O < "addpath " < u < " " < v < " " < what < N;
		}
		else if(type==6){
			int u = RND%n;
			int what = RND%100;
			
			addpathroot(u,what);
			O < "addpathroot " < u < " " < what < N;
		}
		else if(type==7){
			int u = RND%n;
			int what = RND%100;
			
			addtree(u,what);
			O < "addtree " < u < " " < what < N;
		}
		else if(type==8){
			int u = RND%n;
			
			O < "get " < u < N;
		}
		else if(type==9){
			int u,v;
			do{
				u=RND%n;
				v=RND%n;
			} while(!sametree(u,v));
			O < "getpath " < u < " " < v < N;
		}
		else if(type==10){
			int u = RND%n;
			O < "getpathroot " < u < N;
		}
		else if(type==11){
			int u = RND%n;
			O < "gettree " < u < N;
		}
	}
}




