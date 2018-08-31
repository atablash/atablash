#pragma once

#include<atablash/static-array.hpp>
#include<atablash/splay/common.hpp>
#include<atablash/splay/node.hpp>

namespace ab{


// TODO: co z u¿ywaniem allocatora i zwalnianiem pamieci?!

///////////////////////////// SPLAY TREE ///////////////////////////////////////
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
// Common SplayTree
//
// new + delete								2.88s
// nodes on vector:							2.81s
// nodes on vector + reserve:				2.76s
// nodes on vector, no copy constructors	2.76s
// after levels optim						2.74s (2.66??)
// splays everywhere, some optimizations	3.08s
// suddenly (after evert?)					3.40s
//
// does not support evert (swapFlag) (TODO?)
template<typename _Key, typename _Value,
	typename _KeyOfVal,
	typename SplayNode = DefSplayNode<_Value>,
	int MaxNodes = 0,
	typename _Alloc = std::allocator<_Value> >
class SplayTree
{
public:
	class iterator;
private:
	typedef _Key key_type;
	typedef _Value value_type;
	typedef _Alloc allocator_type;
	typedef typename _Alloc::template rebind<SplayNode>::other NodeAlloc;
	typedef Splayer<SplayNode,false> SP;	// not evertable!
public:
	inline SplayTree()
	{
		superRoot()->r = NULL;
		superRoot()->l = NULL;
		superRoot()->parent = NULL;
	}
	inline ~SplayTree()
	{
		//A(!superRoot()->r);
		//if(superRoot()->l) _destroyNode(superRoot()->l);
	}
private:
/*
	void _destroyNode(SplayNode *pn)
	{
		if(pn->l) _destroyNode(pn->l);
		if(pn->r) _destroyNode(pn->r);
		nodeAlloc.destroy(pn);
		nodeAlloc.deallocate(pn,1);
	}*/
private:
	SplayNode* newNode(const _Value& _keyval, SplayNode* parent)
	{
		//_v.push_back(node);
		//return &_v.back();
		return new SplayNode(_keyval, parent);
	}
	void delNode(SplayNode* sn)
	{
		//if(sn!=&_v.back()) *sn = _v.back();
		//_v.pop_back();
		delete sn;								// TODO: zwalniac pamiec moze? (!)
	}
public:
	inline iterator find(const value_type& _keyval, const bool splay = true)
	{
		tuple<SplayNode*,SplayNode**,int> re = _find(_keyval);
		SplayNode* pNode = get<0>(re);
		SplayNode** ppNode = get<1>(re);
		int level = get<2>(re);
		if(*ppNode)
		{
			if(splay)
			{
				SP::splayUntilNode(*ppNode,superRoot(),level);
				return iterator(superRoot()->l);
			}
			else return iterator(*ppNode);
		}
		else
		{
			if(splay && level>0) SP::splayUntilNode(pNode,superRoot(),level-1);
			return iterator(superRoot());
		}
	}
	inline pair<iterator,bool> insert(const value_type& _keyval, const bool splay = true)
	{
		//if(_v.size()==_v.capacity())_v.reserve(_v.size()*3/2);
		tuple<SplayNode*,SplayNode**,int> re = _find(_KeyOfVal()(_keyval));
		SplayNode* pNode = get<0>(re);
		SplayNode** ppNode = get<1>(re);
		int level = get<2>(re);
		if(*ppNode)
		{
			auto result = *ppNode;
			if(splay) SP::splayUntilNode(*ppNode,superRoot(),level);
			return make_pair(iterator(result),false);
		}
		else
		{
			//SplayNode nn(_keyval,pNode);
			*ppNode = newNode(_keyval,pNode);
			auto result = *ppNode;
			A(result);
			if(splay){
				SP::splayUntilNode(*ppNode,superRoot(),level);
				result->template update<0,0>();	// TODO: may be unnecessary when there are any splays
			}
			// TODO: update when not splaying
			A(result);
			return make_pair(iterator(result),true);
		}
	}
	inline iterator insertMulti(const value_type& _keyval, const bool splay = true)
	{
		//if(_v.size()==_v.capacity())_v.reserve(_v.size()*3/2);
		pair<SplayNode*,SplayNode**> re = _findForInsert(_KeyOfVal(_keyval));
		re.se = newNode(_keyval,re.fi);
		if(splay) splayToRoot(*re.se,re.th);
		return iterator(re.se);
	}

	inline void erase(const key_type& _key, const bool splay = true)
	{
		erase(find(_key,splay));
	}

	inline void safe_erase(const key_type& _key, const bool splay = true)
	{
		iterator itr = find(_key, splay);
		if(itr != end()) erase(itr);
	}

	inline void erase(const iterator& itr, const bool splay = true)
	{
		A(itr != end());
		_erase(itr.p, splay);
	}

	inline iterator begin()
	{
		SplayNode *pc = superRoot();
		while(pc->l) pc = pc->l;
		return iterator(pc);
	}
	inline iterator end()
	{
		return iterator(superRoot());
	}
	inline bool isRoot(SplayNode* x) const
	{
		return x->parent == NULL;
	}
	inline void clear()
	{
		//_v.clear();
		superRoot()->l = NULL;
	}
	inline void reserve(unsigned int size)
	{
		//_v.reserve(size);
	}

private:
	// assumes x has no children
	inline void _justErase(SplayNode* x)
	{
		A(x != superRoot());
		A(!x->l); A(!x->r);
		if(x->parent->r == x)
		{
			x->parent->r = NULL;
			if(x->parent != superRoot())
			{
				x->parent->template update<0,-1>();//NR
				SP::updateUntilNode(x->parent->parent,superRoot());
				//SP::subUntilNode(x->parent,x,superRoot());
			}
		}
		else
		{
			A(x->parent->l == x);
			x->parent->l = NULL;
			if(x->parent != superRoot())
			{
				x->parent->template update<-1,0>();//NL
				SP::updateUntilNode(x->parent->parent,superRoot());
				//SP::subUntilNode(x->parent,x,superRoot());
			}
		}
		delNode(x);
	}
	// assumes x has only left child
	inline void _eraseL(SplayNode* x)
	{
		A(x != superRoot());
		A(x->l); A(!x->r);
		if(x->parent->l == x)
		{
			x->parent->l = x->l;
			if(x->parent != superRoot())
			{
				x->parent->template update<1,0>();//L
				SP::updateUntilNode(x->parent->parent,superRoot());
				//SP::subUntilNode(x->parent,x,superRoot());
			}
		}
		else
		{
			A(x->parent->r == x);
			x->parent->r = x->l;
			if(x->parent != superRoot())
			{
				x->parent->template update<0,1>();//R
				SP::updateUntilNode(x->parent->parent, superRoot());
				//SP::subUntilNode(x->parent,x,superRoot());
			}
		}
		x->l->parent = x->parent;
		delNode(x);
	}
	// assumes x has only right child
	inline void _eraseR(SplayNode* x)
	{
		A(x != superRoot());
		A(!x->l); A(x->r);
		if(x->parent->l == x)
		{
			x->parent->l = x->r;
			if(x->parent != superRoot())
			{
				x->parent->template update<1,0>();//L
				SP::updateUntilNode(x->parent->parent,superRoot());
				//SP::subUntilNode(x->parent,x,superRoot());
			}
		}
		else
		{
			A(x->parent->r == x);
			x->parent->r = x->r;
			if(x->parent != superRoot())
			{
				x->parent->template update<0,1>();//R
				SP::updateUntilNode(x->parent->parent,superRoot());
				//SP::subUntilNode(x->parent,x,superRoot());
			}
		}
		x->r->parent = x->parent;
		delNode(x);
	}
	// assumes x has both children
	// replaces x with its predecessor
	inline void _eraseIn(SplayNode* const x, const bool splay = true)
	{
		A(x != superRoot());
		A(x->l); A(x->r);
		SplayNode *c = x->l;
		if(c->r)
		{
			c = c->r;
			if(splay)
			{
				int level = 0;
				while(c->r) c = c->r, ++level;
				SplayNode* p = c->parent;
				p->r = c->l;
				if(c->l)
				{
					//c->subVal(c->l);
					c->l->parent = p;
					p->template update<0,1>();//R
				}
				else p->template update<0,-1>();//NR

				int level2 = level/2;
				while(level2)
				{
					SP::rotateL(p->parent);
					SP::rotateL(p);
					--level2;
				}
				if(level&1)
				{
					SP::rotateL(p);
				}
				//p->subVal(c);
				//p->template updateL(c);
				A(p->parent == x);	// splay succeeded
			}
			else // if(!splay)		// TODO: fix updates
			{
				while(c->r) c = c->r;
				c->parent->r = c->l;
				if(c->l)
				{
					c->subVal(c->l);
					c->l->parent = c->parent;
					c->parent->template update<0,1>();//R
				}
				else c->parent->template update<0,-1>();//NR

				c->parent->subVal(c);

				SplayNode *cn = c->parent->parent;
				while(cn != x)
				{
					cn->template update<0,1>();//R
					cn->subVal(c);
					cn = cn->parent;
				}
			}
			c->l = x->l;
			c->l->parent = c;
			c->parent = x->parent;
			if(x->parent->l == x) x->parent->l = c;
			else {A(x->parent->r == x); x->parent->r = c;}
			c->r = x->r;
			c->r->parent = c;
			c->template update<1,1>();//LR
			//c->addVal(c->l);
			//c->addVal(c->r);
		}
		else
		{
			c->parent = x->parent;
			if(x->parent->l == x) x->parent->l = c;
			else {A(x->parent->r == x); x->parent->r = c;}
			c->r = x->r;
			c->r->parent = c;
			c->template update<0,1>();//R
			c->addVal(c->r);
		}
		for(;;)	// TODO: replace with updateUntilNode
		{
			c = c->parent;
			if(c==superRoot())break;
			c->template update<0,0>();
			//c->subVal(x);
		}
		delNode(x);
	}
	// select appriopriate erase function to call
	inline void _erase(SplayNode* x, const bool splay = true, int level = -1)
	{
		SplayNode* parent = x->parent;
		if(x->l)
		{
			if(x->r) _eraseIn(x,splay);
			else _eraseL(x);
		}
		else
		{
			if(x->r) _eraseR(x);
			else _justErase(x);
		}

		// splay parent
		if(splay && parent!=superRoot())
		{
			if(level != -1) SP::splayUntilNode(parent,superRoot(),level-1);
			else SP::splayUntilNode(parent,superRoot());
		}
	}
public:
	class iterator
	{	friend class SplayTree;
	public: //private:		// ugly hack?!
		inline iterator(SplayNode *_p) : p(_p) {}
	public:
		inline bool operator==(const iterator& o) const {return p==o.p;}
		inline bool operator!=(const iterator& o) const {return !(*this==o);}
		inline value_type& operator*() {return p->keyval;}
		inline const value_type& operator*() const {return p->keyval;}
		inline value_type* operator->() {return &p->keyval;}
		inline const value_type* operator->() const {return &p->keyval;}

		inline SplayNode* getnode(){	// todo: is this an ugly hack?
			return p;
		}

		inline iterator& operator++()
		{
			A(p);
			if(p->r)
			{
				p = p->r;
				while(p->l) p = p->l;
			}
			else
			{
				while(p->parent->r == p) p = p->parent;
				p = p->parent;
			}
			return *this;
		}
		inline iterator& operator--()
		{
			A(p);
			if(p->l)
			{
				p = p->l;
				while(p->r) p = p->r;
			}
			else
			{
				while(p->parent->l == p){
					p = p->parent;
					A(p->parent);//!=superRoot());
				}
				p = p->parent;
			}
			return *this;
		}
		inline iterator& operator++(int)
		{
			iterator itr(*this);
			++*this;
			return itr;
		}
		inline iterator& operator--(int)
		{
			iterator itr(*this);
			--*this;
			return itr;
		}
	private:
		SplayNode *p;
	};
public:
	SplayNode& root(){
		A(superRoot()->l);
		return *superRoot()->l;
	}
private:
	//typename _vector<SplayNode,MaxNodes>::R _v;		// TODO: zrobic znowu!

	// super root is not initialized since we only use its l,r,parent pointers
	// it's the greatest node of the tree, has only left son
	char _superRoot[sizeof(SplayNode)];
	inline SplayNode* superRoot()
	{
		SplayNode *sr = reinterpret_cast<SplayNode*>(_superRoot);
		return sr;
	}

	// finds an element and returns <node,parent's pointer to this node, depth(level)>
	inline tuple<SplayNode*,SplayNode**,int> _find(const key_type& _key)
	{
		int level = 0;
		if(superRoot()->l == 0) return make_tuple(superRoot(),&superRoot()->l,0);
		SplayNode *cand = superRoot()->l;
		for(;;) //while(_KeyOfVal()((cand)->keyval) != _key)
		{
			if(_KeyOfVal()((cand)->keyval) < _key)
			{
				++level;
				if((cand)->r) cand = (cand)->r;
				else return make_tuple((cand),&(cand)->r,level);
			}
			else if(_key < _KeyOfVal()((cand)->keyval))
			{
				++level;
				if((cand)->l) cand = (cand)->l;
				else return make_tuple((cand),&(cand)->l,level);
			}
			else break;
		}
		return make_tuple((cand)->parent,
			cand->parent->r==cand?&cand->parent->r:&cand->parent->l,level);
	}
	// finds a place to insert a new element, possibly equal to some others already present
	// returns parent's pointer to this node
	inline SplayNode** _findForInsert(const key_type& _key)	// multiset
	{
		SplayNode **cand = &superRoot()->l;
		while(*cand != NULL)
		{
			if(_KeyOfVal()((*cand)->keyval) <= _key) cand = &(*cand)->r;
			else cand = &(*cand)->l;
		}
		return cand;
	}
};
////////////////////////////////////////////////////// (splay tree) ////////////


template<typename T, class Node = DefSplayNode<T> >
class SplaySet : public SplayTree<T,T,_Identity<T>, Node>{};


}
