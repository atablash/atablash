#pragma once

#include<cstring>
#include<vector>
#include<algorithm>

#include<atablash/types.hpp>

namespace ab{

////////////////////////////////// UNION FIND //////////////////////////////////

// TODO: check without ranks
// TODO: aggregate values
class UnionFindRanks{
public:
	size_t find(size_t x){
		if(x >= parents.size()) return x;

		size_t root = x;
		while(parents[root] != root) root = parents[root];
		while(x != root){
			size_t& curr = parents[x];
			x = parents[x];
			curr = root;
		}
		return root;
	}

	bool unio(size_t a, size_t b){ // false if union already made
		check(std::max(a,b));
		a = find(a);  b = find(b);
		if(a==b) return false;

		if(ranks[a] < ranks[b]) parents[a] = b, ++ranks[b];
		else parents[b] = a, ++ranks[a];
		return true;
	}

private:
	std::vector<size_t> parents;
	std::vector<size_t> ranks;

	void check(size_t where){
		if(where < parents.size()) return;
		parents.resize(where+1);
		ranks.resize(where+1);
	}
};
////////////////////////////////////////////////////////////////////////////////



// no ranks!
template<class T = size_t>
class UnionFind{
	mutable std::vector<T> parents;

	inline void check(T where){
		if(where < parents.size()) return;
		resize(where+1);
	}
	
public:

	class Element{
		UnionFind<T>& uf;
		T ith;
	public:
		Element(UnionFind& uf_, T ith_) : uf(uf_), ith(ith_){}
		operator T(){
			return uf.find(ith);
		}
		Element& operator=(T a){
			uf.unio(ith, a);
			return *this;
		}
		Element& operator=(const Element& o) {
			return (*this) = o.uf.find(o.ith);
		}
	};

	class ElementConst{
		const UnionFind<T>& uf;
		T ith;
	public:
		ElementConst(const UnionFind& uf_, T ith_) : uf(uf_), ith(ith_){}
		operator T(){
			return uf.find(ith);
		}
	};
	
	Element operator[](T ith){
		return Element{*this,ith};
	}
	ElementConst operator[](T ith) const {
		return ElementConst{*this, ith};
	} 
	
	T find(T x) const {
		if(x >= parents.size()) return x;

		T root = x;
		while(parents[root] != root) root = parents[root];
		while(x != root){
			T& curr = parents[x];
			x = parents[x];
			curr = root;
		}
		return root;
	}

	bool unio(T a, T b){ // false if union already made
		check(std::max(a,b));
		a = find(a);
		b = find(b);
		if(a==b) return false;

		parents[a] = b;
		return true;
	}
	
	void resize(T new_size){
		auto old_size = parents.size();
		parents.resize(new_size);
		for(uint i=old_size; i<new_size; ++i){
			parents[i] = i;
		}
	}

	template<class V>
	void applyRename(const V& m) {
		auto oldParents = std::move(parents);
		clear();
		resize(oldParents.size());
		for (uint i = 0; i < oldParents.size(); ++i) {
			(*this)[m[i]] = (*this)[m[oldParents[i]]];
		}
	}
	
	size_t size()const{
		return parents.size();
	}

	void pop_back(){
		parents.pop_back();
	}
	
	void clear(){
		parents.clear();
	}

private:
};
////////////////////////////////////////////////////////////////////////////////






}


