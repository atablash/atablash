#pragma once

namespace ab{


////////////////////////// SPLAY NODES /////////////////////////////////////////
//
// Implementation: Adam Blaszkiewicz (atablash.pl), 2013-2015
//
/*
    This program is licensed under GNU General Public License version 3.
    See <http://www.gnu.org/licenses/>.
*/






// simple splay node. derive from this
// no pathParent!
template<class Derived> struct SplayNode{
	SplayNode() : parent(0), l(0), r(0) {}
	Derived *parent, *l, *r;
	
	bool& swapFlag(){ return *(bool*)NULL; }

	/*
	template<int L, int R>
	void update(){
		if(L==1 or (L==0 and l)){
		}
		if(R==1 or (R==0 and r)){
		}
	}
	*/
	
	// Tree aggregation
	void addVal(const Derived* other){}
	void subVal(const Derived* other){}
	
	// Subtree modifiers
	void addPathMod(const Derived* other){}
	void subPathMod(const Derived* other){}
	void addTreeMod(const Derived* other){}
	void subTreeMod(const Derived* other){}
};





//
//
// SplayNode is used for SplayTree (SplaySet, SplayMap (TODO), ...) in splay/tree.h
//
// TODO: use SplayNode instead?
//
template<typename _Value, typename Derived> struct BaseSplayNode
{
	// Only data, should be no vptr
	_Value keyval;
	Derived *parent;
	Derived *l;
	Derived *r;

	// Children-less initialization
	inline BaseSplayNode(const _Value& _keyval, Derived* _parent) :
		keyval(_keyval), parent(_parent), l(NULL), r(NULL) {}

	// Full initialization
	inline BaseSplayNode(const _Value& _keyval, Derived* _parent, Derived* _l, Derived* __r) :
		keyval(_keyval), parent(_parent), l(_l), r(__r) {}

	// PATH AGGREGATION
	// "pure virtual" update functions
	template<bool,bool>
	inline void update() {ASS(false);}
	//inline void updateCopy(Derived* x) {A(false);}	// copy stats from x

	// TODO: path aggregation for linear values - could be faster,
	//   especially when updating ancestors of deleted node

	// swap flag - for evert (rerooting)
	inline bool& swapFlag() const {ASS(false); return 0;}

	// TREE AGGREGATION
	inline void addVal(Derived* x) {ASS(false);}
	inline void subVal(Derived* x) {ASS(false);}
	
	// SUBTREE MODIFIERS for path/tree aggregation
	inline void addPathMod(Derived*){ASS(false);}
	inline void subPathMod(Derived*){ASS(false);}
	inline void addTreeMod(Derived*){ASS(false);}
	inline void subTreeMod(Derived*){ASS(false);}
};






template<typename _Value, typename Derived> struct CopyableSplayNode :
	public BaseSplayNode<_Value, Derived>
{
	typedef BaseSplayNode<_Value,Derived> B;//ASE
	inline void operator=(const CopyableSplayNode& o){
		B::operator=(o);
		if(B::l) B::l->parent = (Derived*)this;
		if(B::r) B::r->parent = (Derived*)this;
		if(B::parent->l == &o) B::parent->l = (Derived*)this;
		else {A(B::parent->r==&o); B::parent->r = (Derived*)this;}
	}
	inline CopyableSplayNode(const CopyableSplayNode& o) : B(o){
		if(B::l) B::l->parent = (Derived*)this;
		if(B::r) B::r->parent = (Derived*)this;
		if(B::parent->l == &o) B::parent->l = (Derived*)this;
		else {A(B::parent->r==&o); B::parent->r = (Derived*)this;}
	}
	inline CopyableSplayNode(const _Value& _keyval, Derived* _parent) :
		B(_keyval,_parent) {}
};






// Default SplayNode: copyable, empty update functions
//
// Do not derive from this!!
//
template<typename _Value> struct DefSplayNode :
	public CopyableSplayNode<_Value,DefSplayNode<_Value> >	// tell Base that it's our base
															// (avoid vtable)
{
	typedef CopyableSplayNode<_Value,DefSplayNode<_Value> > B;//ASE

	// Children-less initialization
	inline DefSplayNode(const _Value& _keyval, DefSplayNode* _parent) :
		CopyableSplayNode<_Value,DefSplayNode<_Value> >(_keyval,_parent) {}

	// empty update functions
	template<int L, int R>
	inline void update() {}
};
////////////////////////////////////////////////////////////////////////////////



}





