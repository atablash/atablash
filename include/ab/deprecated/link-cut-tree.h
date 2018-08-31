#pragma once
#include<atablash/splay.h>

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
// Tarjan's ST-trees (aka Link-Cut (aka dynamic trees)) with extensions:
// - rootable means that root can be changed to another vertex in the tree
//   rootable implies that underlying splay trees are evertable
// - linTreeAgg - aggregation of linear values over trees
//   (sum is ok, max is not ok; unless you keep all path-children vals on priority queue)
//
// PV is a pointer to struct with:
// - PV l,r,parent,pathParent;
//
// - bool getSwapFlag()				<- (1) for evert (rootable)
// - void setSwapFlag(bool)
//
// - template<L,R> void update()	<- (2) for path aggregation (splay level)
//		L,R in {-1,0,1}		-1 no child, 0 unknown, 1 child
//
// - void addVal(PV)			<- (3) for tree aggregation (augmented tree level, linear values only)
// - void subVal(PV)				(this.ppChildrenVal   +=/-=   other.val)
//
//
// For tree aggregation use (2)+(3)
// - This is aggregation over splay children plus "pathParent" children
// - on update() set this.val = l.val + r.val + ppChildrenVal
//
template<typename PV, bool rootable = false, bool linTreeAgg = false>
class LinkCutter
{
	typedef Splayer<rootable> S;
public:
	// links a tree rootet at u to node v
	// u must be tree root! (so no link(u,_) was called earlier)
	// assumes there are no cycles
	static inline void linkRoot(const PV& child, const PV& parent)
	{
		access(child);	// trzeba?
		
		// splay because we need to find the root anyway
		//if(rootable) splayPath(child);	// may be left or right swaps
		//else splayPathR(child);

		A(child->parent == NULL);
		A(child->pathParent == NULL);
		A(!(rootable && child->getSwapFlag()) || child->r == NULL);
		A( (rootable && child->getSwapFlag()) || child->l == NULL);
		child->pathParent = parent;
		if(linTreeAgg)
		{
			access(parent);
			parent->addVal(child);
		}
	}
	// cuts pv from its parent
	static inline void cut(const PV& pv)
	{
		if(linTreeAgg) access(pv);
		else splayPath(pv);

		A(pv->pathParent || pv->l ||
			(pv->r && rootable && pv->getSwapFlag()));	// cut legal
		if(rootable && pv->getSwapFlag())
		{
			if(linTreeAgg || pv->r)
			{
				A(pv->r);
				if(!linTreeAgg){
					pv->r->pathParent = pv->pathParent;
					pv->pathParent = NULL;
				} else A(pv->pathParent == NULL);
				pv->r->parent = NULL;
				pv->r->pathParent = NULL; // trzeba
				pv->r->setSwapFlag(!pv->r->getSwapFlag());	// xor
				pv->r = NULL;
				pv->template update<0,-1>();	//NR
			}
			else
			{
				A(pv->pathParent);	// cut legal (pv not root)
				pv->pathParent = NULL;
			}
		}
		else
		{
			if(linTreeAgg || pv->l)
			{
				A(pv->l);
				if(!linTreeAgg){
					pv->l->pathParent = pv->pathParent;
					pv->pathParent = NULL;
				} else A(pv->pathParent == NULL);
				pv->l->parent = NULL;
				pv->l->pathParent = NULL; // trzeba
				pv->l = NULL;
				pv->template update<-1,0>();	// NL
			}
			else
			{
				A(pv->pathParent);	// cut legal (pv not root)
				pv->pathParent = NULL;
			}
		}
	}
	// makes pv a new root (evert)
	static inline void makeRoot(const PV& pv)
	{
		A(rootable);
		access(pv,true);
		// after access with cutting pv's sons
		// it suffices to reverse the preferred path
		pv->setSwapFlag(!pv->getSwapFlag());
	}
	// builds preferred path from root to pv (and possibly further if cutPrefChild==false)
	// after this, pv is the root of this path's tree
	// pv->l can be used to get aggregate root--pv path values (faster than cutPrefChild==true)
	// returns:
	// - first:  highest preferred path change (for lca)
	// - second: previous top level auxliary tree root
	static inline pair<PV,PV> access(const PV& pv, const bool cutPrefChild=false)
	{
		PV _prevRoot = splayPath(pv);
		if(cutPrefChild)
		{
			if(rootable && pv->getSwapFlag())
			{
				if(pv->l)
				{
					pv->l->parent = NULL;
					pv->l->pathParent = pv;
					pv->l->setSwapFlag(!pv->l->getSwapFlag());
					pv->l = NULL;
					pv->template update<-1,0>();	//NL
				}
			}
			else
			{
				if(pv->r)
				{
					pv->r->parent = NULL;
					pv->r->pathParent = pv;
					if(linTreeAgg) pv->addVal(pv->r);
					pv->r = NULL;
					pv->template update<0,-1>();	// NR
				}
			}
		}
		if(!pv->pathParent)
		{
			return make_pair(pv,_prevRoot);
		}
		for(;;)
		{
			// zmiana preferred child
			const PV pparent = pv->pathParent;
			if(linTreeAgg) pparent->subVal(pv);

			//splayPath(pparent);
			const PV prevRoot = S::splayUntilNull(pparent);
			A(pparent->parent == NULL);
			//pparent->pathParent = prevRoot->pathParent;
			PV nextpp = prevRoot->pathParent;

			//prevRoot->pathParent = NULL;

			// disconnect old preferred child
			if(rootable && pparent->getSwapFlag())
			{
				if(pparent->l)
				{
					pparent->l->parent = NULL;
					pparent->l->pathParent = pparent;
					pparent->l->setSwapFlag(!pparent->l->getSwapFlag());
					if(linTreeAgg) pparent->addVal(pparent->r);
					//pparent->l = NULL;
				}
				pparent->l = pv;
				pv->parent = pparent;
				pv->setSwapFlag(!pv->getSwapFlag());
				//pv->pathParent = NULL;
				pparent->template update<1,0>();	// L
				S::rotateRLast(pv);
			}
			else
			{
				if(pparent->r) // if old child exists, disconnect it
				{
					pparent->r->parent = NULL;
					pparent->r->pathParent = pparent;
					if(linTreeAgg) pparent->addVal(pparent->r);
					//pparent->r = NULL;
				}
				pparent->r = pv;
				pv->parent = pparent;
				//pv->pathParent = NULL;
				pparent->template update<0,1>();	// R
				S::rotateLLast(pv);
			}
			A(pv->parent == NULL);

			pv->pathParent = nextpp;

			if(nextpp == NULL)
			{
				return make_pair(pparent,prevRoot);
			}
		}
	}
	static inline PV findRoot(const PV& pv)
	{
		access(pv);
		int level = 0;
		PV c = pv;
		if(rootable)
		{
			for(;;)
			{
				PV next = c->getSwapFlag() ? c->r : c->l;
				if(!next) break;
				c = next;
				++level;
			}
			S::splayUntilNull(c,level);
		}
		else
		{
			while(c->l) c = c->l, ++level;
			S::splayRUntilNull(c,level);
		}


		A(c->parent == NULL);
		A(!c->pathParent);
		//pv->pathParent = NULL;

		return c;
	}
private:
	// Splay pv to root of its auxliary tree
	// returns previous root
	static inline const PV splayPath(const PV& pv)
	{
		PV prevRoot = S::splayUntilNull(pv);
		A(pv->parent==NULL);
		pv->pathParent = prevRoot->pathParent;
		//prevRoot->pathParent = NULL;
		return prevRoot;
	}
	// Splay pv to root of its auxliary tree using only right rotations
	// assumes pv is the smallest element in its tree (sure???)
	static inline void splayPathR(const PV& pv)
	{
		const PV& prevRoot = S::splayRUntilNull(pv);
		A(pv->parent==NULL);
		pv->pathParent = prevRoot->pathParent;
		//prevRoot->pathParent = NULL;
	}
};
//////////////////////////////////////////// link-cutter //////////////////////


}
