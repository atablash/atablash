#pragma once

#include<vector>

#include<atablash/types.hpp>
#include<atablash/debug.hpp>
#include<atablash/lambda.hpp>

namespace ab{


// INTERVAL TREE (simple)
// not tested
template<class T, class AGG_FUN>
class ITree{

private:
	std::vector<T> tree;
	uint tsize;
	const AGG_FUN& aggFun;
	
public:
	ITree(const AGG_FUN& _fun) : tsize(1), aggFun(_fun){
		tree.resize(tsize*2);
	}
	
	ITree(const ITree& o) : tree(o.tree), tsize(o.tsize), aggFun(o.aggFun){}
	ITree& operator=(const ITree& o){
		tree = o.tree;
		tsize = o.tsize;
		aggFun = o.aggFun;
		return *this;
	}
	
	// todo: operator= instead of set
	//       operator[] should return smart object
	void set(uint where, const T& what){
		check(where);
		
		where+=tsize;
		tree[where] = what;
		while(where!=1){
			where>>=1;
			tree[where] = (*aggFun)(tree[where*2],tree[where*2+1]);
		}
	}
	
	const T& get(uint where)const{
		if(where>=tsize) return T();
		return tree[tsize+where];
	}
	
	const T& operator[](uint where)const{
		return get(where);
	}
	
	T get(uint fr, uint to)const{
		ASS(fr<to);
		if(fr>=tsize)return T();
		if(to>tsize) to=tsize;
		
		fr += tsize;  to += tsize-1;
		T lresult = T();
		T rresult = T();
		while(fr<to){
			if((fr&1)==1) lresult = aggFun(tree[fr],lresult), fr>>=1, ++fr;
			else fr>>=1;
			if((to&1)==0) rresult = aggFun(rresult,tree[to]), to>>=1, --to;
			else to>>=1;
		}
		if(fr==to)lresult=aggFun(lresult,tree[fr]);
		return aggFun(lresult,rresult);
	}
	
	const T& root()const{
		return tree[1];
	}
	
	// can only make bigger
	void resize(uint new_size){
		check(new_size-1);
	}
	
private:
	void check(uint where){
		if(where<tsize)return;
		
		uint oldPos = tsize;
		uint moves = 0;
		while(where >= tsize) tsize <<= 1, ++moves;
		
		tree.resize(tsize*2);
		uint newPos = tsize;
		
		while(oldPos){
			for(uint i=0; i<oldPos; ++i){
				tree[newPos+i] = tree[oldPos+i];
				tree[oldPos+i] = T();
			}
			oldPos >>= 1;
			newPos >>= 1;
		}
		
		// update
		while(newPos>0){
			tree[newPos] = aggFun(tree[newPos*2], tree[newPos*2+1]);
			newPos >>= 1;
		}
	}
	
};

template<class T, class AGG_FUN>
ITree<T,AGG_FUN> getITree(const AGG_FUN& aggFun = std::plus<T>()){
	return ITree<T,AGG_FUN>(aggFun);
}





//	INTERVAL TREE with subree modifiers (no propagation)
//
//
//	Example for sum:
//		//					[0,1000000)  VAL agg,    T += MOD*INTERVAL
//		ITreeMod<int,int> tree(1000000,  (_1 + _2), (_1 += _2*_3));
//
//		// add 69 to every modifier in [a,b):
//		tree.alter(fr,to, _1 += 69);
//		// or:
//		tree.plus(fr,to, 69);
//
// tested only for MAX (ignoring subtree_size)
//
//
// TODO: copy to new class ITreePro: propagate modifiers!!
// gives some possibilities like "set value on interval"
// it would require another function: apply modifier to another modifier
//
template<class T, class MOD, class AGG_FUN, class MOD_FUN>
class ITreeMod{
public:
	ITreeMod(const AGG_FUN& _fun, const MOD_FUN& _modFun) : tsize(1), aggFun(_fun), modFun(_modFun){
		tree.resize(tsize*2);
	}
	
	template<class FUN>
	void mod(uint where, const FUN& fun){
		check(where);
		
		where += size;
		fun(tree[where].se);
		modFun(tree[where].fi = T(), tree[where].se);
		updateToRoot(where);
	}
	
	// requires T::operator-
	void set(uint where, const T& what){
		T was = get(where);
		mod(where, what - was);
	}
	
	const T& get(uint where)const{
		if(where >= size) return T();
		
		where += size;
		T result = tree[where].fi;
		while(where != 1){
			where >>= 1;
			modFun(result, tree[where].se, 1);
		}
		return result;
	}
	
	const T& operator[](uint where)const{
		return get(where);
	}
	
	T get(uint fr, uint to)const{
		ASS(fr<to);
		if(fr>=tsize) return T();
		if(to>tsize) to = tsize;
		
		fr+=tsize; to+=tsize-1;
		T lresult = T();
		T rresult = T();
		uint lagg=0;
		uint ragg=0;
		uint lsize = 0;
		uint rsize = 0;
		uint csize = 1;
		while(fr<to){
			if(lagg) modFun(lresult,tree[lagg].second,lsize), lagg>>=1;
			if(ragg) modFun(rresult,tree[ragg].second,rsize), ragg>>=1;
			
			if((fr&1)==1) lresult = aggFun(lresult,tree[fr].first), lsize+=csize, fr>>=1, lagg=fr, ++fr;
			else fr>>=1;
			if((to&1)==0) rresult = aggFun(tree[to].first,rresult), rsize+=csize, to>>=1, ragg=to, --to;
			else to>>=1;
			
			csize <<= 1;
		}
		if(lagg and fr==to and ragg){
			for(;;){
				T cresult = tree[fr].first;
				modFun(lresult,tree[lagg].second,lsize), lagg>>=1;
				modFun(rresult,tree[ragg].second,rsize), ragg>>=1;
				fr>>=1;
				if		(lagg==fr){
					lresult = aggFun(lresult,cresult), lsize += csize;
					break;
				}
				else if	(fr==ragg){
					rresult = aggFun(cresult,rresult), rsize += csize;
					break;
				}
				modFun(cresult,tree[fr].second,csize);
			}
			modFun(lresult,tree[lagg].second,lsize);
			modFun(rresult,tree[ragg].second,rsize);
		}
		else if(lagg and fr==to){
			rresult = tree[fr].first;
			ragg = fr;
			rsize = csize;
			
			modFun(lresult,tree[lagg].second,lsize);
		}
		else if(fr==to and ragg){
			lresult = tree[fr].first;
			lagg = fr;
			lsize = csize;
			
			modFun(rresult,tree[ragg].second,rsize);
		}
		else if(!lagg and !ragg){
			T result = tree[fr].first;
			while(fr!=1){
				fr>>=1;
				modFun(result, tree[fr].second, csize);
			}
			return result;
		}
		else{	// lagg and ragg
			modFun(lresult, tree[lagg].second,lsize);
			modFun(rresult, tree[ragg].second,rsize);
		}
		
		// lagg and ragg
		ASS(lagg and ragg);
		ASS(lagg != ragg);
		for(;;){
			lagg >>= 1;
			ragg >>= 1;
			if(lagg==ragg){
				lresult = aggFun(lresult,rresult);
				lsize += rsize;
				modFun(lresult, tree[lagg].second, lsize);
				break;
			}
			modFun(lresult, tree[lagg].second, lsize);
			modFun(rresult, tree[ragg].second, rsize);
		}
		while(lagg != 1){
			lagg>>=1;
			modFun(lresult, tree[lagg].second, lsize);
		}
		return lresult;
	}
	
	template<class FUN>
	void alter(uint fr, uint to,  const FUN& f){
		ASS(fr<to);
		
		check(to-1);
		
		fr += tsize; to += tsize-1;
		uint lupd = 0, lupd_size=0;
		uint rupd = 0, rupd_size=0;
		uint csize = 1;
		
		{
			if((fr&1)==1){
				f(tree[fr].second);
				modFun(tree[fr].first=T(), tree[fr].second, csize);
				if(!lupd)lupd=fr, lupd_size=csize;
				fr>>=1;
				++fr;
			}
			else fr>>=1;
			
			if((to&1)==0){
				f(tree[to].second);
				modFun(tree[to].first=T(), tree[to].second, csize);
				if(!rupd)rupd=to, rupd_size=csize;
				to>>=1;
				--to;
			}
			else to>>=1;
			
			csize <<= 1;
		}
		
		while(fr<to){
			if((fr&1)==1){
				f(tree[fr].second);
				update(fr,csize);
				if(!lupd)lupd=fr, lupd_size=csize;
				fr>>=1;
				++fr;
			}
			else fr>>=1;
			
			if((to&1)==0){
				f(tree[to].second);
				update(to,csize);
				if(!rupd)rupd=to, rupd_size=csize;
				to>>=1;
				--to;
			}
			else to>>=1;
			
			csize <<= 1;
		}
		if(fr==to){
			f(tree[fr].second);
			update(fr,csize);
			if(!lupd)lupd=fr, lupd_size=csize;
			else if(!rupd)rupd=fr, rupd_size=csize;
		}
		
		if(lupd and rupd){
			while(lupd_size < rupd_size)update(lupd >>= 1, lupd_size <<= 1);
			while(lupd_size > rupd_size)update(rupd >>= 1, rupd_size <<= 1);
			for(;;){
				lupd >>= 1; lupd_size <<= 1;
				rupd >>= 1;
				update(lupd, lupd_size);
				if(lupd == rupd)break;
				update(rupd, lupd_size);
			}
		}
		else if(rupd) lupd=rupd, lupd_size=rupd_size;
		
		while(lupd > 1) update(lupd >>= 1, lupd_size <<= 1);
	}
	
	template<class ARG>
	void plus(int fr, int to, const ARG& arg){
		alter(fr,to, _1 += arg);
	}
	template<class ARG>
	void minus(int fr, int to, const ARG& arg){
		alter(fr,to,arg, _1 -= _2);
	}
	template<class ARG>
	void multiplies(int fr, int to, const ARG& arg){
		alter(fr,to,arg, _1 *= _2);
	}
	template<class ARG>
	void divides(int fr, int to, const ARG& arg){
		alter(fr,to,arg, _1 /= _2);
	}
	
private:
	void update(int where, int ssize){
		tree[where].first = aggFun(tree[where*2].first, tree[where*2+1].first);
		modFun(tree[where].first, tree[where].second, ssize);
	}
	void updateToRoot(int where, int ssize){
		while(where>1){
			where>>=1;
			ssize<<=1;
			update(where,ssize);
		}
	}
	
public:
	uint size()const{
		return tsize;
	}
	
	const std::pair<T,MOD>* getTree()const{
		return tree;
	}
	
private:
	void check(uint where){
		if(where<tsize)return;
		
		uint oldPos = tsize;
		uint moves = 0;
		while(where >= tsize) tsize <<= 1, ++moves;
		
		tree.resize(tsize*2);
		uint newPos = tsize;
		
		while(oldPos){
			for(uint i=0; i<oldPos; ++i){
				tree[newPos+i] = tree[oldPos+i];
				tree[oldPos+i] = std::make_pair(T(),MOD());
			}
			oldPos >>= 1;
			newPos >>= 1;
		}
		updateToRoot(newPos << 1, tsize >> moves);
	}
	uint tsize;
	std::vector<std::pair<T,MOD> > tree;
	const AGG_FUN& aggFun;
	const MOD_FUN& modFun;
};


template<class T, class MOD, class AGG_FUN, class MOD_FUN>
ITreeMod<T,MOD,AGG_FUN,MOD_FUN> getITree(
			const AGG_FUN& _fun,
			const MOD_FUN& _modFun){
	return ITreeMod<T,MOD,AGG_FUN,MOD_FUN>(_fun,_modFun);
}

template<class T, class MOD, class AGG_FUN>
ITreeMod<T,MOD,AGG_FUN,LambdaPlusAssign<Lambda1,Lambda2> > getITree(
			const AGG_FUN& _fun){
	return ITreeMod<T,MOD,AGG_FUN,__typeof(_1 += _2)> (_fun,(_1 += _2));
}






template<class T, class MOD, class AGG_FUN, class MOD_FUN>
std::ostream& operator<<(std::ostream& os, const ITreeMod<T,MOD,AGG_FUN,MOD_FUN>& t){

	os << "CUMs:\n";
	uint csize = t.getSize();
	for(uint fi=1; fi<=t.getSize(); fi<<=1){
		for(uint j=fi; j<(fi<<1); ++j){
			os << t.getTree()[j].first;
			for(uint i=0; i<csize-1; ++i)os<<' ';
		}
		os<<'\n';
		csize/=2;
	}
	os << "\nMODs:\n";
	csize = t.getSize();
	for(uint fi=1; fi<=t.getSize(); fi<<=1){
		for(uint j=fi; j<(fi<<1); ++j){
			os << t.getTree()[j].second;
			for(uint i=0; i<csize-1; ++i)os<<' ';
		}
		os<<'\n';
		csize/=2;
	}
	os<<'\n'<<'\n';
	return os;
}



}





