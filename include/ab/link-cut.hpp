#pragma once


#include<atablash/link-cutter.hpp>
#include<atablash/aggregator.hpp>


namespace ab{



//////////////////////////// LINK-CUT //////////////////////////////////////////
//
// Implementation: Adam Blaszkiewicz (atablash.pl), 2013-2014
//
/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//
//
//
//
//
// full-blown Link-Cut interface
//
// for max:
// AggFun		= max
// ModFun		= _1 += _2
// ModAddFun	= _1 += _2
// ModSubFun	= _1 -= _2
//
//
// We require:
// AGG_FUN(+):				T x T -> T			associative, commutative, neutral element
// MOD_ADD_FUN(+): 			MOD x MOD -> MOD	associative, commutative, neutral element, inverse element
// MOD_INV_FUN(unary -):	MOD -> MOD			(returns inverse element with respect to MOD_ADD_FUN)
// APPLY_MOD_FUN(@):		T& x MOD -> T&		(no return value, result in first argument)
//
// (t @ mod1) @ mod2  ==  t @ (mod1 + mod2)
//
template<class T, class MOD, bool rootable, bool _pathAgg, bool _treeAgg, class AGG_FUN, class APPLY_MOD_FUN, class MOD_ADD_FUN, class MOD_INV_FUN>
class LinkCut{
private:
	//static const bool pathAgg = typename SAME<path_AGG_FUN,VOID>::r;
	//static const bool pathMod = typename SAME<path_MOD_FUN,VOID>::r and pathAgg;

	//static const bool treeAgg = typename SAME<tree_AGG_FUN,VOID>::r;
	//static const bool treeMod = typename SAME<tree_MOD_FUN,VOID>::r and treeAgg;

	static const bool modifiers = !SAME<MOD,void>::r;

	static const bool pathAgg = _pathAgg;
	static const bool treeAgg = _treeAgg;

	static const bool pathMod = pathAgg and modifiers;
	static const bool treeMod = treeAgg and modifiers;

	struct Node;
	typedef LinkCutter<Node*, rootable, treeAgg> LC;

public:
	LinkCut( const AGG_FUN& _aggFun, const APPLY_MOD_FUN& _applyModFun, const MOD_ADD_FUN& _modAddFun, const MOD_INV_FUN& _modInvFun) :
					aggFun(_aggFun), applyModFun(_applyModFun), modAddFun(_modAddFun), modInvFun(_modInvFun), funs(*this){}

	// link tree rooted at a to b
	void link(uint a, uint b){
		check(max(a,b));

		LC::linkRoot(&nodes[a], &nodes[b], funs);
	}

	// cut a from its parent
	void cut(uint a){
		ASS(a < nodes.siz);

		LC::cut(&nodes[a], funs);
	}

	// get single element
	T get(uint a){
		if(a >= nodes.siz) return T();

		LC::access(&nodes[a], false, funs);
		T result = vThis[a];
		if(pathMod) applyModFun(result, vPathMod[a]);
		if(treeMod) applyModFun(result, vTreeMod[a]);
		return result;
	}

	// aggregate path from a to root
	T path(uint a){
		if(a >= nodes.siz) return T();

		LC::access(&nodes[a], true, funs);
		return vPathAgg[a];
	}

	// aggregate path from a to b
	T path(uint a, uint b){
		if(a==b){
			if(a>=nodes.siz) return T();
			else{
				LC::access(&nodes[a], false, funs);
				T result = vThis[a];
				if(pathMod) applyModFun(result, vPathMod[a]);
				if(treeMod) applyModFun(result, vTreeMod[a]);
				return result;
			}
		}

		ASS(a < nodes.siz);
		ASS(b < nodes.siz);

		LC::access(&nodes[b], true, funs);
		AU(s, LC::access(&nodes[a], true, funs));

		LC::splayPath(s.lastChange, funs);	// access
		T result = T();
		if(rootable and vSwapFlags[s.lastChange - &*nodes.be]){
			if(s.lastChange->l){
				result = vPathAgg[s.lastChange->l - &*nodes.be];
			}
		}
		else{
			if(s.lastChange->r){
				result = vPathAgg[s.lastChange->r - &*nodes.be];
			}
		}

		result = aggFun(result, vThis[s.lastChange - &*nodes.be]);
		if(pathMod) applyModFun(result, vPathMod[s.lastChange - &*nodes.be]);

		if(s.lastDisconnected) result = aggFun(result, vPathAgg[s.lastDisconnected - &*nodes.be]);

		if(treeMod) applyModFun(result, vTreeMod[s.lastChange - &*nodes.be]);

		return result;
	}

	// aggregate subtree rooted at a
	T tree(uint a){
		if(a>=nodes.siz) return T();

		LC::access(&nodes[a], true, funs);

		T aa = vThis[a];
		if(pathMod) applyModFun(aa, vPathMod[a]);
		T result = aggFun(vAggregator[a].get(), aa);
		if(treeMod) applyModFun(result, vTreeMod[a]);

		return result;
	}

	int lca(uint a, uint b){
		if(a==b)return a;

		ASS(a < nodes.size());
		ASS(b < nodes.size());
		LC::access(&nodes[a], true, funs); // TODO: use node->l to get aggregate, without using cutPrefChild
		return LC::access(&nodes[b], false, funs).lastChange - &*nodes.be;
	}

	void set(uint where, const T& what){
		check(where);

		LC::access(&nodes[where], false, funs);
		vThis[where] = what;

		if(pathAgg) applyModFun(vThis[where], modInvFun(vPathMod[where]));
		if(treeAgg) applyModFun(vThis[where], modInvFun(vTreeMod[where]));

		funs.update(&nodes[where]);
	}

	void mod(uint where, const T& what){
		check(where);

		LC::access(&nodes[where], false, funs);
		applyModFun(vThis[where], what);
		funs.update(&nodes[where]);
	}

	// mod path to root
	void modPath(uint a, const T& m){
		check(a);

		LC::access(&nodes[a], true, funs);

		modAddFun(vPathMod[a], m);
		funs.update(&nodes[a]);
	}

	void modPath(uint a, uint b, const MOD& m){
		if(a==b){
			if(a>=nodes.siz)check(a);
			else LC::access(&nodes[a], false, funs);
			applyModFun(vThis[a], m);
			funs.update(&nodes[a]);
			return;
		}

		ASS(a < nodes.siz);
		ASS(b < nodes.siz);

		LC::access(&nodes[b], true, funs);
		modAddFun(vPathMod[b], m);
		funs.update(&nodes[b]);

		AU(s, LC::access(&nodes[a], true, funs));
		modAddFun(vPathMod[a], m);
		funs.update(&nodes[a]);

		LC::access(s.lastChange, true, funs);
		MOD inv = modInvFun(m);
		modAddFun(vPathMod[s.lastChange-&*nodes.be], inv);
		modAddFun(vPathMod[s.lastChange-&*nodes.be], inv);
		applyModFun(vThis[s.lastChange-&*nodes.be], m);
		funs.update(s.lastChange);
	}

	void modTree(uint a, const MOD& m){
		check(a);

		LC::access(&nodes[a], true, funs);
		applyModFun(vTreeMod[a], m);
		if(rootable and vSwapFlags[a]){
			if(nodes[a].r){
				applyModFun(vTreeMod[nodes[a].r - &*nodes.be], modInvFun(m));
				funs.update(nodes[a].r);
				funs.updateR(&nodes[a]);
			}
			else funs.updateNR(&nodes[a]);
		}
		else{
			if(nodes[a].l){
				applyModFun(vTreeMod[nodes[a].l - &*nodes.be], modInvFun(m));
				funs.update(nodes[a].l);
				funs.updateL(&nodes[a]);
			}
			else funs.updateNL(&nodes[a]);
		}
	}

	void makeRoot(uint a){
		if(a >= nodes.siz)return;

		LC::makeRoot(&nodes[a], funs);
	}

	void debug(uint a)const{
		if(a>=nodes.siz)return;

		E<"Node "<a<":"<EN;
		E < "  parent = " < (nodes[a].parent ? nodes[a].parent-&*nodes.be : 0) < EN;
		E < "  l = " < (nodes[a].l ? nodes[a].l-&*nodes.be : 0) < EN;
		E < "  r = " < (nodes[a].r ? nodes[a].r-&*nodes.be : 0) < EN;
		E < "  pathParent = " < (nodes[a].pathParent ? nodes[a].pathParent-&*nodes.be : 0) < EN;
		if(rootable) E < "  swapFlag = " < (int)vSwapFlags[a] < EN;
		if(pathAgg or treeAgg)
					 E < "  vThis    = " < vThis[a] < EN;
		if(pathAgg)  E < "  pathAgg  = " < vPathAgg[a] < EN;
		if(treeAgg)  E < "  treeAgg  = " < vTreeAgg[a] < EN;
		if(pathMod)  E < "  pathMod  = " < vPathMod[a] < EN;
		if(treeMod)  E < "  treeMod  = " < vTreeMod[a] < EN;
	}

private:
	void check(uint a){
		if(a < nodes.siz)return;

		ui8* prev = (ui8*)&nodes[0];
		nodes.resize((a+2)*3/2);
		ui8* now  = (ui8*)&nodes[0];

		// if there was vector relocation
		if(prev != now){
			int diff = now-prev;
			for(uint i=0; i<nodes.siz; ++i){
				if(nodes[i].parent) 	(ui8*&)nodes[i].parent += diff;
				if(nodes[i].l) 			(ui8*&)nodes[i].l += diff;
				if(nodes[i].r)			(ui8*&)nodes[i].r += diff;
				if(nodes[i].pathParent) (ui8*&)nodes[i].pathParent += diff;
			}

			for(uint i=0; i<nodes.size(); ++i) ASS(LC::S::consistent(&nodes[i]));
		}

		if(rootable) vSwapFlags.resize(nodes.siz);
		if(pathAgg) vPathAgg.resize(nodes.siz);
		if(pathMod) vPathMod.resize(nodes.siz);
		if(treeAgg) vTreeAgg.resize(nodes.siz);
		if(treeMod) vTreeMod.resize(nodes.siz);
		if(treeMod) vAggregator.resize(nodes.siz, aggFun);

		if(pathAgg or treeAgg) vThis.resize(nodes.siz);
	}

private:
	std::vector<Node> nodes;

	std::vector<T> vPathAgg;
	std::vector<MOD> vPathMod;

	std::vector<T> vTreeAgg;
	std::vector<MOD> vTreeMod;

	std::vector<T> vThis;

	std::vector<char> vSwapFlags;

	std::vector<Aggregator<T,AGG_FUN> > vAggregator; // for path-children (treeAgg)

	const AGG_FUN& aggFun;
	const APPLY_MOD_FUN& applyModFun;
	const MOD_ADD_FUN& modAddFun;
	const MOD_INV_FUN& modInvFun;


	struct LinkCutterFuns{
		LinkCutterFuns(LinkCut& _lc) : lc(_lc) {}
		LinkCut& lc;

		char&       swapFlag(      Node* v)const{ return lc.vSwapFlags[v - &*lc.nodes.be]; }
		const char& swapFlag(const Node* v)const{ return lc.vSwapFlags[v - &*lc.nodes.be]; }

		template<int L, int R>
		void _update  (const Node* v)const{
			int i = v - &*lc.nodes.be;
			if((L==1 or v->l) and (R==1 or v->r)){
				int l = v->l - &*lc.nodes.be;
				int r = v->r - &*lc.nodes.be;
				if(lc.pathAgg) lc.vPathAgg[i] = lc.aggFun( lc.aggFun(lc.vPathAgg[l], lc.vPathAgg[r]), lc.vThis[i]);
				if(lc.pathMod) lc.applyModFun(lc.vPathAgg[i], lc.vPathMod[i]);
				if(lc.treeMod) lc.applyModFun(lc.vPathAgg[i], lc.vTreeMod[i]);

				if(lc.treeAgg) lc.vTreeAgg[i] = lc.aggFun( lc.aggFun(lc.vTreeAgg[l], lc.vTreeAgg[r]), lc.vAggregator[i].get());
				if(lc.treeMod) lc.applyModFun(lc.vTreeAgg[i], lc.vTreeMod[i]);
			}
			else if((L==1 or v->l) or (R==1 or v->r)){
				int c;
				if(L==1 or v->l)	c = v->l - &*lc.nodes.be;
				else				c = v->r - &*lc.nodes.be;

				if(lc.pathAgg) lc.vPathAgg[i] = lc.aggFun(lc.vPathAgg[c], lc.vThis[i]);
				if(lc.pathMod) lc.applyModFun(lc.vPathAgg[i], lc.vPathMod[i]);
				if(lc.treeMod) lc.applyModFun(lc.vPathAgg[i], lc.vTreeMod[i]);

				if(lc.treeAgg) lc.vTreeAgg[i] = lc.aggFun(lc.vTreeAgg[c], lc.vAggregator[i].get());
				if(lc.treeMod) lc.applyModFun(lc.vTreeAgg[i], lc.vTreeMod[i]);
			}
			else{
				if(lc.pathAgg) lc.vPathAgg[i] = lc.vThis[i];
				if(lc.pathMod) lc.applyModFun(lc.vPathAgg[i], lc.vPathMod[i]);
				if(lc.treeMod) lc.applyModFun(lc.vPathAgg[i], lc.vTreeMod[i]);

				if(lc.treeAgg) lc.vTreeAgg[i] = lc.vAggregator[i].get();
				if(lc.treeMod) lc.applyModFun(lc.vTreeAgg[i], lc.vTreeMod[i]);
			}
		}

		void update  (const Node* v)const{ _update< 0, 0>(v); }
		void updateL (const Node* v)const{ _update< 1, 0>(v); }
		void updateR (const Node* v)const{ _update< 0, 1>(v); }
		void updateNL(const Node* v)const{ _update<-1, 0>(v); }
		void updateNR(const Node* v)const{ _update< 0,-1>(v); }

		void addVal(const Node* v, const Node* other)const{
			int i = v - &*lc.nodes.begin();
			int j = other - &*lc.nodes.begin();
			//E < "addVal " < i < " " < j < "   " < lc.aggFun(lc.vTreeAgg[j], lc.vPathAgg[j]) < N;
			lc.vAggregator[i].insert( lc.aggFun(lc.vTreeAgg[j], lc.vPathAgg[j]) );
		}
		void subVal(const Node* v, const Node* other)const{
			int i = v - &*lc.nodes.begin();
			int j = other - &*lc.nodes.begin();
			//E < "subVal " < i < " " < j < "   " < lc.aggFun(lc.vTreeAgg[j], lc.vPathAgg[j]) < N;
			lc.vAggregator[i].erase( lc.aggFun(lc.vTreeAgg[j], lc.vPathAgg[j]) );
		}

		void addPathMod(const Node* v, const Node* other)const{
			int i = v - &*lc.nodes.begin();
			int j = other - &*lc.nodes.begin();
			lc.modAddFun(lc.vPathMod[i], lc.vPathMod[j]);
		}
		void subPathMod(const Node* v, const Node* other)const{
			int i = v - &*lc.nodes.begin();
			int j = other - &*lc.nodes.begin();
			lc.modAddFun(lc.vPathMod[i], lc.modInvFun(lc.vPathMod[j]));
		}
		void addTreeMod(const Node* v, const Node* other)const{
			int i = v - &*lc.nodes.begin();
			int j = other - &*lc.nodes.begin();
			lc.modAddFun(lc.vTreeMod[i], lc.vTreeMod[j]);
		}
		void subTreeMod(const Node* v, const Node* other)const{
			int i = v - &*lc.nodes.begin();
			int j = other - &*lc.nodes.begin();
			lc.modAddFun(lc.vTreeMod[i], lc.modInvFun(lc.vTreeMod[j]));
		}
	} funs;

	struct Node{
		Node() : parent(0), l(0), r(0), pathParent(0){}
		Node *parent, *l, *r, *pathParent;
	};
};

template<class T, class MOD, bool rootable=false, bool pathAgg=false, bool treeAgg=false, class AGG_FUN, class APPLY_MOD_FUN, class MOD_ADD_FUN, class MOD_INV_FUN>
LinkCut<T, MOD, rootable, pathAgg, treeAgg, AGG_FUN, APPLY_MOD_FUN, MOD_ADD_FUN, MOD_INV_FUN>
getLinkCut( const AGG_FUN& _aggFun = _1 + _2, const APPLY_MOD_FUN& _modFun = _1 += _2, const MOD_ADD_FUN& _modAddFun = _1 += _2, const MOD_INV_FUN& _modInvFun = -_1){
	return LinkCut<T, MOD, rootable, pathAgg, treeAgg, AGG_FUN, APPLY_MOD_FUN, MOD_ADD_FUN, MOD_INV_FUN>
		(_aggFun, _modFun, _modAddFun, _modInvFun);
}
//////////////////////////////////////////// link-cut /////////////////////////



}

