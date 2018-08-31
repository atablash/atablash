#pragma once

#include<queue>
#include<unordered_map>
#include<atablash/interval-tree.hpp>

namespace ab{




// aggregates values dynamically on a tree
// FUN must be associative and commutative
// there must be hash<T> defined
template<class T, class FUN> class Aggregator{
public:
	Aggregator(const FUN& _fun) : csize(0), tree(_fun){}
	Aggregator(const Aggregator& o) : csize(o.csize), tree(o.tree), whereis(o.whereis), free(o.free){}
	/*Aggregator& operator=(const Aggregator& o){
		csize = o.csize;
		tree = o.tree;
		whereis = o.whereis;
		free = o.free;
		return *this;
	}*/

	void insert(const T& a){
		int where;
		if(free.empty()) where=csize++;
		else where=free.back(), free.pop_back();

		tree.set(where,a);
		whereis.insert(make_pair(a,where));
	}

	void erase(const T& a){
		__typeof(whereis.be) it = whereis.find(a);
		ASS(it != whereis.end());
		int where = it->second;
		whereis.erase(it);
		tree.set(where,T());
		free.push_back(where);
	}

	const T& get(){
		return tree.root();
	}

private:
	uint csize;

	ITree<T,FUN> tree;
	umultimap<T,int> whereis;
	std::vector<int> free;
};





}
