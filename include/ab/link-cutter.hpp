#pragma once
#include<atablash/splay/common.hpp>

namespace ab{


//////////////////////////// LINK-CUTTER ///////////////////////////////////////
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





// Tarjan's ST-trees (aka Link-Cut (aka dynamic trees)) with extensions:
// - rootable means that root can be changed to another vertex in the tree
//   rootable implies that underlying splay trees are evertable
// - treeAgg - aggregation over trees
//   (linear values are easy, others need special handling in addVal and subVal)
//
// PV is a pointer to struct with:
// - PV l,r,parent,pathParent;
//
// - bool& swapFlag()				<- (1) for evert (rootable)
//
// - template<L,R> void update()	<- (2) for path aggregation (splay level)
//		L,R in {-1,0,1}		-1 no child, 0 unknown, 1 child
//			(here you can update node based on its children and modifiers)
//
// - void addVal(PV)			<- (3) for tree aggregation (augmented tree level)
// - void subVal(PV)				(this.ppChildrenVal   +=/-=   other.val)
//									called when pathParent links are created and destroyed
//
// - void addPathMod(PV)			<- for modification on whole paths/subtrees
// - void subPathMod(PV)				(this.mod +=/-= other.mod)
// - void addTreeMod(PV)
// - void subTreeMod(PV)
//
// For tree aggregation use (2)+(3)
// - This is aggregation over splay children plus "pathParent" children
// - on update() set this.val = l.val + r.val + ppChildrenVal
//
template<class PV, bool rootable = false, bool treeAgg = false>
class LinkCutter
{
public:
	typedef Splayer<PV,rootable> S;



public:

	// links a tree rootet at u to node v
	// u must be tree root! (so no link(u,_) was called earlier)
	// assumes there are no cycles
	template<class FUNS>
	static inline void linkRoot(const PV& child, const PV& parent, const FUNS& funs = S::template DefFuns<PV>::get()){

		access(child,false,funs);	// TODO: why?
		// splay because we need to find the root anyway:
		//if(rootable) splayPath(child);	// may be left or right swaps
		//else splayPathR(child);

		// child must be root:
		ASS(child->parent == NULL);
		ASS(child->pathParent == NULL);
		if(rootable and funs.swapFlag(child)) ASS(child->r == NULL);
		else{
			ASS(child->l == NULL);
		}

		// attach child to parent:
		child->pathParent = parent;
		if(treeAgg){
			// access parent because we would have to update ancestors anyway:
			access(parent,false,funs);

			funs.subTreeMod(child,parent);//child->subTreeMod(parent);
			funs.update(child);

			funs.addVal(parent,child);//parent->addVal(child);
			funs.update(parent);	// 2014-11-26
		}
	}

	// cuts pv from its parent
	template<class FUNS>
	static inline void cut(const PV& pv, const FUNS& funs = S::template DefFuns<PV>::get()){
		if(treeAgg) access(pv,false,funs);
		else splayPath(pv,funs);

		ASS(pv->pathParent || pv->l ||
			(pv->r && rootable && funs.swapFlag(pv)));	// cut legal (pv has parent)

		if(rootable and funs.swapFlag(pv)){
			if(treeAgg){
				ASS(pv->r);
			}
			if(pv->r){
				pv->r->pathParent = pv->pathParent;
				if(!treeAgg) pv->pathParent = NULL;
				else{
					ASS(pv->pathParent == NULL);
				}

				funs.addPathMod(pv->r,pv);//pv->r->addPathMod(pv);
				funs.addTreeMod(pv->r,pv);
				funs.update(pv->r);

				pv->r->parent = NULL;
				funs.swapFlag(pv->r) ^= 1; //pv->r->setSwapFlag(!pv->r->getSwapFlag());	// xor
				pv->r = NULL;
				funs.updateNR(pv);//pv->template update<0,-1>();	//NR
			}
			else{
				ASS(pv->pathParent);	// cut legal (pv not root)
				pv->pathParent = NULL;
			}
		}
		else{
			if(treeAgg){
				ASS(pv->l);
			}
			if(pv->l){
				pv->l->pathParent = pv->pathParent;
				if(!treeAgg) pv->pathParent = NULL;
				else{
					ASS(pv->pathParent == NULL); // so no need to subVal from pathParent
				}

				funs.addPathMod(pv->l,pv);
				funs.addTreeMod(pv->l,pv);
				funs.update(pv->l);

				pv->l->parent = NULL;
				pv->l = NULL;
				funs.updateNL(pv);//pv->template update<-1,0>();	// NL
			}
			else{
				// no tree agg if we're here
				ASS(pv->pathParent);	// cut legal (pv not root)
				pv->pathParent = NULL;
			}
		}
	}

	// makes pv a new root (evert)
	template<class FUNS>
	static inline void makeRoot(const PV& pv, const FUNS& funs = S::template DefFuns<PV>::get()){
		ASS(rootable);
		access(pv, true, funs);
		// after access with cutting pv's sons
		// it suffices to reverse the preferred path
		funs.swapFlag(pv) ^= 1; //pv->swapFlag(!pv->swapFlag());
	}

	// builds preferred path from root to pv (and possibly further if cutPrefChild==false)
	// after this, pv is the root of this path's tree
	// pv->l can be used to get aggregate root--pv path values (faster than cutPrefChild==true)
	// returns:
	// - lastChange:  highest preferred path change or pv itself (for lca)
	// - lastDisconnected: disconnected son of lastChange or NULL
	// - prevRoot: previous top level auxliary tree root
	struct access_return{
		access_return(const PV& a, const PV& b, const PV& c) : lastChange(a), lastDisconnected(b), prevRoot(c) {}
		PV lastChange;
		PV lastDisconnected;
		PV prevRoot;
	};

	template<class FUNS>
	static inline access_return access(const PV& pv, const bool cutPrefChild=false, const FUNS& funs = S::template DefFuns<PV>::get()){
		PV _prevRoot = splayPath(pv,funs);
		PV lastDisconnected = NULL;
		if(cutPrefChild){
			if(rootable && funs.swapFlag(pv)){
				if(pv->l){
					lastDisconnected = pv->l;
					pv->l->parent = NULL;
					funs.addPathMod(pv->l, pv); // pv->l->addPathMod(pv);
					funs.update(pv->l);

					pv->l->pathParent = pv;
					funs.swapFlag(pv->l) ^= 1; // pv->l->swapFlag = !pv->l->swapFlag(); // ^= ?
					if(treeAgg) funs.addVal(pv, pv->l); // pv->addVal(pv->l);
					pv->l = NULL;
					funs.updateNL(pv);//pv->template update<-1,0>();	//NL
				}
			}
			else{
				if(pv->r){
					lastDisconnected = pv->r;
					pv->r->parent = NULL;
					funs.addPathMod(pv->r, pv); // pv->r->addPathMod(pv);
					funs.update(pv->r);

					pv->r->pathParent = pv;
					if(treeAgg) funs.addVal(pv, pv->r); // pv->addVal(pv->r);
					pv->r = NULL;
					funs.updateNR(pv);//pv->template update<0,-1>();	// NR
				}
			}
		}

		if(!pv->pathParent){
			return access_return(pv,lastDisconnected,_prevRoot);
		}

		for(;;){
			// zmiana preferred child
			const PV pparent = pv->pathParent;
			if(treeAgg) funs.subVal(pparent, pv); // pparent->subVal(pv);

			//splayPath(pparent);
			const PV prevRoot = S::splayUntilNull(pparent,funs);
			ASS(pparent->parent == NULL);
			//pparent->pathParent = prevRoot->pathParent;
			PV nextpp = prevRoot->pathParent;

			//prevRoot->pathParent = NULL;

			// disconnect old preferred child
			if(rootable && funs.swapFlag(pparent)){
				if(pparent->l){
					lastDisconnected = pparent->l;
					pparent->l->parent = NULL;
					funs.addPathMod(pparent->l, pparent); // pparent->l->addPathMod(pparent);
					funs.update(pparent->l);

					pparent->l->pathParent = pparent;
					funs.swapFlag(pparent->l) ^= 1; // pparent->l->setSwapFlag(!pparent->l->getSwapFlag());
					if(treeAgg) funs.addVal(pparent, pparent->l); // pparent->addVal(pparent->l); r->l ?!
					//pparent->l = NULL;
				}
				else lastDisconnected = NULL;

				pparent->l = pv;
				pv->parent = pparent;
				funs.swapFlag(pv) ^= 1; // pv->setSwapFlag(!pv->getSwapFlag());
				funs.subPathMod(pv, pparent); // pv->subPathMod(pparent);

				//pv->pathParent = NULL;
				funs.updateL(pparent); //pparent->template update<1,0>();	// L
				S::rotateRLast(pv,funs);
			}
			else{
				if(pparent->r){ // if old child exists, disconnect it
					lastDisconnected = pparent->r;
					pparent->r->parent = NULL;
					funs.addPathMod(pparent->r, pparent); // pparent->r->addPathMod(pparent);
					funs.update(pparent->r);

					pparent->r->pathParent = pparent;
					if(treeAgg) funs.addVal(pparent, pparent->r); // pparent->addVal(pparent->r);
					//pparent->r = NULL;
				}
				else lastDisconnected = NULL;

				pparent->r = pv;
				pv->parent = pparent;
				funs.subPathMod(pv, pparent); // pv->subPathMod(pparent);

				//pv->pathParent = NULL;
				funs.updateR(pparent); // pparent->template update<0,1>();	// R
				S::rotateLLast(pv,funs);
			}
			ASS(pv->parent == NULL);

			pv->pathParent = nextpp;

			if(nextpp == NULL){
				return access_return(pparent, lastDisconnected, prevRoot);
			}
		}
	}

	template<class FUNS>
	static inline PV findRoot(const PV& pv, const FUNS& funs = S::template DefFuns<PV>::get()){
		access(pv,false,funs);
		int level = 0;
		PV c = pv;
		if(rootable){
			for(;;){
				PV next = c->swapFlag() ? c->r : c->l;
				if(!next) break;
				c = next;
				++level;
			}
			S::splayUntilNull(c,level);
		}
		else{
			while(c->l) c = c->l, ++level;
			S::splayRUntilNull(c,level);
		}


		ASS(c->parent == NULL);
		ASS(!c->pathParent);
		//pv->pathParent = NULL;

		return c;
	}

public:
	// Splay pv to root of its auxliary tree
	// returns previous root
	template<class FUNS>
	static inline const PV splayPath(const PV& pv, const FUNS& funs = S::template DefFuns<PV>::get()){
		PV prevRoot = S::splayUntilNull(pv, funs);
		ASS(pv->parent==NULL);
		pv->pathParent = prevRoot->pathParent;
		//prevRoot->pathParent = NULL;
		return prevRoot;
	}

private:
	// Splay pv to root of its auxliary tree using only right rotations
	// assumes pv is the smallest element in its tree (sure???)
	template<class FUNS>
	static inline void splayPathR(const PV& pv, const FUNS& funs = S::template DefFuns<PV>::get()){
		const PV& prevRoot = S::splayRUntilNull(pv, funs);
		ASS(pv->parent==NULL);
		pv->pathParent = prevRoot->pathParent;
		//prevRoot->pathParent = NULL;
	}
};
//////////////////////////////////////////// link-cutter //////////////////////


}






