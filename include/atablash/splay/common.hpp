#pragma once

namespace ab{


//////////////////////// SPLAYER (for splay trees) /////////////////////////////
//
// Implementation: Adam Blaszkiewicz (atablash.pl), 2013-2015
//
/*
    This program is licensed under GNU General Public License version 3.
    See <http://www.gnu.org/licenses/>.
*/
//
//
//
// Splayer provides some splay node operations

	template<class PV> struct DefSplayFuns{
		bool& swapFlag(const PV& v)const{ return v->swapFlag(); }
		
		//void update(const PV& v){/*empty*/}	// v->update<bool,bool>() is called anyway (performance reasons; TODO: check if it makes it worth)
		
		void update  (const PV& v)const{ v->template update< 0, 0>(); }
		void updateL (const PV& v)const{ v->template update< 1, 0>(); }
		void updateR (const PV& v)const{ v->template update< 0, 1>(); }
		void updateNL(const PV& v)const{ v->template update<-1, 0>(); }
		void updateNR(const PV& v)const{ v->template update< 0,-1>(); }
		
		void addVal(const PV& v, const PV& other)const{ return v->addVal(other); }
		void subVal(const PV& v, const PV& other)const{ return v->subVal(other); }
		
		void addPathMod(const PV& v, const PV& other)const{ v->addPathMod(other); }
		void subPathMod(const PV& v, const PV& other)const{ v->subPathMod(other); }
		void addTreeMod(const PV& v, const PV& other)const{ v->addTreeMod(other); }
		void subTreeMod(const PV& v, const PV& other)const{ v->subTreeMod(other); }
		
		static const DefSplayFuns& get(){static DefSplayFuns defFuns; return defFuns;}
	};
	
	
	
template<typename PV, bool evertable = false, class FUNS = DefSplayFuns<PV> >
struct Splayer
{
	static inline void updateUntilNode(PV& x, const PV& superRoot, const FUNS& funs = DefSplayFuns<PV>::get()){
		while(x != superRoot){
			A(x);
			funs.update(x);//x->template update<0,0>();
			x = x->parent;
		}
	}
	
	static inline void updateUntilNull(PV& x, const FUNS& funs = DefSplayFuns<PV>::get()){
		updateUntilNode(x,NULL,funs);
	}
	
	static inline void splayUntilNode(PV& x, const PV& superRoot, int level, const FUNS& funs = DefSplayFuns<PV>::get())
	{
		int level2 = level/2;
		while(level2) splay(x,funs),--level2;
		if(level&1) rotate(x,funs);
		ASS(x->parent == superRoot);
	}
	
	static inline void splayUntilNode(PV& x, PV& superRoot, const FUNS& funs = DefSplayFuns<PV>::get())
	{
		int level = 0;
		PV pc = x;
		while(pc->parent != superRoot) pc = pc->parent, ++level;
		splayUntilNode(x,superRoot,level,funs);
	}
	
	static inline void splayUntilNull(PV& x, int level, const FUNS& funs = DefSplayFuns<PV>::get())
	{
		if(level==0)return;
		if(level==1)rotateLast(x,funs);
		else
		{
			int level2 = level/2;
			while(--level2) splay(x,funs);
			if(level&1) splay(x,funs),rotateLast(x,funs);
			else splayLast(x,funs);
		}
		ASS(x->parent == NULL);
	}
	
	// returns previous root
	static inline PV splayUntilNull(PV& x, const FUNS& funs = DefSplayFuns<PV>::get()){
		int level = 0;
		PV pc = x;
		while(pc->parent) pc = pc->parent, ++level;
		splayUntilNull(x,level,funs);
		return pc;
	}

	// splay only right
	static inline void splayRUntilNull(PV& x, int level, const FUNS& funs = DefSplayFuns<PV>::get()){
		if(level==0)return;
		if(level==1)rotateRLast(x,funs);
		else
		{
			int level2 = level/2;
			while(--level2) rotateR(x->parent,funs), rotateR(x,funs);
			if(level&1) rotateR(x->parent,funs),rotateR(x,funs),rotateRLast(x,funs);
			else rotateRLast(x->parent,funs),rotateRLast(x,funs);
		}
		A(x->parent == NULL);
	}
	// returns previous root
	static inline PV* splayRUntilNull(PV& x, const FUNS& funs = DefSplayFuns<PV>::get())
	{
		int level = 0;
		PV pc = x;
		while(pc->parent) pc = pc->parent, ++level;
		splayRUntilNull(x,level,funs);	// bugfix
		return pc;
	}


	// ROTATIONS

	// assumes x's parent is NOT root (its parent != NULL)
	static inline void rotateR(PV& x, const FUNS& funs = DefSplayFuns<PV>::get())
	{
		ASS(x->parent && x->parent->parent);
		PV prevParent = rotateRLast(x,funs);
		ASS(x->parent);
		if(x->parent->l == prevParent) x->parent->l = x;
		else {ASS(x->parent->r == prevParent); x->parent->r = x;}
	}
	
	
	// assumes x's parent is root (its parent == NULL)
	// return prev parent
	/*
	//      parent                     x
	//       / %                      * \
	//      /   %                    *   \
	//     /     %                  *     \
	//    x               ===>           parent
	//   * \                              / %
	//  *   \                            /   %
	//       xr                         xr
	//
	*/
	static inline PV rotateRLast(PV& x, const FUNS& funs = DefSplayFuns<PV>::get())
	{
		ASS(x); ASS(x->parent);
		ASS(x->parent->l == x);	// x must be the left son
		PV parent = x->parent;
		PV xr;
		
		if(evertable && funs.swapFlag(x)){
			xr = x->l;
			
			// we want x not swapped, so swap children and theif swapFlags
			x->l = x->r;
			if(x->l) funs.swapFlag(x->l) ^= 1;
			if(xr) funs.swapFlag(xr) ^= 1;
		}
		else{
			xr = x->r;
		}
		
		parent->l = xr;
		
		funs.addPathMod(x,parent);
		funs.addTreeMod(x,parent);
		funs.subPathMod(parent,x);
		funs.subTreeMod(parent,x);
		
		if(xr){
			funs.subPathMod(xr,parent);
			funs.subTreeMod(xr,parent);
			funs.update(xr);
			xr->parent = parent;
			funs.updateL(parent);	// L
		}
		else funs.updateNL(parent);//parent->template update<-1,0>();	// NL
		
		x->parent = parent->parent;
		funs.swapFlag(x) = funs.swapFlag(parent);//x->setSwapFlag(parent->getSwapFlag());
		funs.swapFlag(parent) = false;//parent->setSwapFlag(false);
		x->r = parent;
		parent->parent = x;
		funs.updateR(x);//x->template update<0,1>();
		return parent;
	}
	
	// assumes x's parent is NOT root (its parent != NULL)
	static inline void rotateL(PV& x, const FUNS& funs = DefSplayFuns<PV>::get()){
		ASS(x->parent->parent);
		PV prevParent = rotateLLast(x,funs);
		if(x->parent->l == prevParent) x->parent->l = x;
		else {ASS(x->parent->r == prevParent); x->parent->r = x;}
	}
	
	// assumes x's parent is root (its parent == NULL)
	// returns prev parent
	static inline PV rotateLLast(PV& x, const FUNS& funs = DefSplayFuns<PV>::get()){
		ASS(x); ASS(x->parent);
		ASS(x->parent->r == x);	// can rotate left
		PV parent = x->parent;
		PV xl;
		if(evertable && funs.swapFlag(x)){
			xl = x->r;
			x->r = x->l;
			if(x->r) funs.swapFlag(x->r) ^= 1; //x->r->setSwapFlag(!x->r->getSwapFlag());
			if(xl) funs.swapFlag(xl) ^= 1; //xl->setSwapFlag(!xl->getSwapFlag());
		}
		else{
			xl = x->l;
		}
		parent->r = xl;
		
		funs.addPathMod(x,parent);//x->addPathMod(parent);
		funs.addTreeMod(x,parent);//x->addTreeMod(parent);
		funs.subPathMod(parent,x);//parent->subPathMod(x);
		funs.subTreeMod(parent,x);//parent->subTreeMod(x);
		
		if(xl){
			funs.subPathMod(xl,parent);//xl->addPathMod(x);
			funs.subTreeMod(xl,parent);//xl->addTreeMod(x);
			xl->parent = x->parent;
			funs.update(xl);
			funs.updateR(parent);//parent->template update<0,1>();	// R
		}
		else funs.updateNR(parent);//parent->template update<0,-1>(); // NR
		
		x->parent = parent->parent;
		funs.swapFlag(x) = funs.swapFlag(parent);//x->swapFlag = parent->swapFlag();
		funs.swapFlag(parent) = false;//parent->setSwapFlag(false);
		x->l = parent;
		parent->parent = x;
		funs.updateL(x);//x->template update<1,0>();
		return parent;
	}
	
	// assumes x's parent is NOT root (its parent != null)
	static inline void rotate(PV& x, const FUNS& funs = DefSplayFuns<PV>::get())
	{
		ASS(x); ASS(x->parent); ASS(x->parent->parent);
		if(x->parent->r == x) rotateL(x,funs);
		else {ASS(x->parent->l == x); rotateR(x,funs);}
	}
	
	// assumes x's parent is root (its parent == null)
	static inline void rotateLast(PV& x, const FUNS& funs = DefSplayFuns<PV>::get())
	{
		ASS(x); ASS(x->parent);ASS(!x->parent->parent);
		if(x->parent->r == x) rotateLLast(x,funs);
		else {ASS(x->parent->l == x); rotateRLast(x,funs);}
	}

	// assumes x's grandparent is NOT root (its parent != NULL)
										// TODO: can be optimized if splays aren't
										// done using rotate functions but directly
	static inline void splay(PV& x, const FUNS& funs = DefSplayFuns<PV>::get()){	// zig-zig or zig-zag
		ASS(x); ASS(x->parent); // x not root
		ASS(x->parent->parent); // parent not root
		ASS(x->parent->parent->parent); // there is superRoot
		PV p = x->parent;
		PV g = p->parent;
		if(evertable and funs.swapFlag(p)){
			if(p->l == x){
				if(g->l == p) rotateR(x,funs), rotateR(x,funs);
				else {ASS(g->r == p); rotateL(p,funs), rotateL(x,funs);}
			}
			else{
				ASS(p->r == x);
				if(g->l == p) rotateR(p,funs), rotateR(x,funs);
				else {ASS(g->r == p); rotateL(x,funs), rotateL(x,funs);}
			}
		}
		else{
			if(p->l == x){
				if(g->l == p) rotateR(p,funs), rotateR(x,funs);
				else {ASS(g->r == p); rotateR(x,funs), rotateL(x,funs);}
			}
			else{
				ASS(p->r == x);
				if(g->l == p) rotateL(x,funs), rotateR(x,funs);
				else {ASS(g->r == p); rotateL(p,funs), rotateL(x,funs);}
			}
		}
	}

	// assumes x's grandparent is root (its parent == NULL)
	static inline void splayLast(PV& x, const FUNS& funs = DefSplayFuns<PV>::get()){	// zig-zig or zig-zag
		ASS(x); ASS(x->parent); // x not root
		ASS(x->parent->parent); // parent not root
		PV p = x->parent;
		PV g = p->parent;
		if(evertable and funs.swapFlag(p)){
			if(p->l == x){
				if(g->l == p) rotateR(x,funs), rotateRLast(x,funs);
				else {ASS(g->r == p); rotateLLast(p,funs), rotateLLast(x,funs);}
			}
			else{
				ASS(p->r == x);
				if(g->l == p) rotateRLast(p,funs), rotateRLast(x,funs);
				else {ASS(g->r == p); rotateL(x,funs), rotateLLast(x,funs);}
			}
		}
		else{
			if(p->l == x){
				if(g->l == p) rotateRLast(p,funs), rotateRLast(x,funs);
				else {ASS(g->r == p); rotateR(x,funs), rotateLLast(x,funs);}
			}
			else{
				ASS(p->r == x);
				if(g->l == p) rotateL(x,funs), rotateRLast(x,funs);
				else {ASS(g->r == p); rotateLLast(p,funs), rotateLLast(x,funs);}
			}
		}
	}
	
	// debug
	static inline bool consistent(const PV& pv){
		// parent link
		if(pv->parent && pv->parent->l!=pv && pv->parent->r!=pv) return false;
		// left link
		if(pv->l && pv->l->parent!=pv) return false;
		// right link
		if(pv->r && pv->r->parent!=pv) return false;
		// children are the same
		if(pv->l && pv->l == pv->r) return false;
		if(pv->l && pv->l == pv->parent) return false;
		if(pv->r && pv->r == pv->parent) return false;
		return true;
	}
};
////////////////////////////////////////////////////////////////////////////////



}
