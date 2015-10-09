//#define FASTIO
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

ll getpath(int u, int v, int from = -1){
	if(u==v) return vals[v];
	FE(it,tos[u])if(*it != from){
		ll r = getpath(*it, v, u);
		if(r!=-LLONG_MAX) return max(r, vals[u]);
	}
	return -LLONG_MAX;
}

ll gettree(int u){
	ll r = -LLONG_MAX;
	FE(it,tos[u]) if(*it != parent[u]){
		r = max(r, gettree(*it));
	}
	return max(r, vals[u]);
}

void tc(){
	int n=ri, q=ri;
	
	tos.resize(n);
	parent.resize(n);
	vals.resize(n);
	
	F(i,n)parent[i]=-1;
	
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
	
	F(i,q){
		int type = types[rstr];
		
		if(type==0){
			int u=ri, v=ri;
			ASS(parent[u]==-1);
			parent[u] = v;
			tos[u].insert(v);
			tos[v].insert(u);
		}
		else if(type==1){
			int u = ri;
			ASS(parent[u] != -1);
			
			tos[u].erase(parent[u]);
			tos[parent[u]].erase(u);
			parent[u] = -1;
		}
		else if(type==2){
			int v = ri;
			makeroot(v);
		}
		else if(type==3){
			int v = ri;
			int what = ri;
			vals[v] = what;
		}
		else if(type==4){
			int v = ri;
			int what = ri;
			vals[v] += what;
		}
		else if(type==5){
			int u=ri, v=ri, what=ri;
			
			bool r = addpath(u,v,what);
			ASS(r);
		}
		else if(type==6){
			int u = ri;
			int what = ri;
			
			addpathroot(u,what);
		}
		else if(type==7){
			int u = ri;
			int what = ri;
			
			addtree(u,what);
		}
		else if(type==8){
			int u = ri;
			O < vals[u] < N;
		}
		else if(type==9){
			int u=ri, v=ri;
			O < getpath(u,v) < N;
		}
		else if(type==10){
			int u = ri;
			O < getpath(u,findroot(u)) < N;
		}
		else if(type==11){
			int u = ri;
			O < gettree(u) < N;
		}
	}
}




