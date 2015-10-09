//#define READ_NUM_TEST_CASES
#define FASTIO	// enable fastio

////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013 Adam Blaszkiewicz. All rights reserved.
#ifdef DEBUG_BUILD
#undef FASTIO
#include<cassert>
#endif
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<climits>
#include<cfloat>
#ifdef FASTIO
#include<cstdio>
#else
#include<iostream>
#endif
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
#include<map>
#include<set>
#include<hash_set>
#include<hash_map>
#define mp make_pair
#define e1 first
#define e2 second
#define e3 third
#define e4 fourth
#define fi first
#define se second
#define th third
#define pb push_back
#define pf push_front
#define ins insert
#define era erase
#define it iterator
#define type(x) __typeof(x)
#define foreach(itr,cont) for(__typeof(cont.begin()) itr = cont.begin(); itr != cont.end(); ++itr)
#define rep(i,n) for(int i=0; i<(n); ++i)
#define per(i,n) for(int i=(n)-1; i>=0; --i)
#define fo(i,a,b) for(int i=(a); i<(b); ++i)
#define of(i,a,b) for(int i=(b)-1; i>=(a); --i)
#define sz(c) (int)(c).size()
#ifndef DEBUG_BUILD
#define ASS(x)
#define DBG if(0)
#else
#define ASS(x) assert(x);
#define DBG
#endif
using namespace std;using namespace __gnu_cxx;typedef char BOOL;typedef long long ll;typedef unsigned int uint;typedef unsigned short ushort;typedef unsigned char uchar;typedef unsigned long long ull;typedef pair<int,int> pii;typedef pair<uint,uint> puiui;typedef pair<int,uint> piui;typedef pair<uint,int> puii;typedef pair< ll,ll> pllll;typedef pair<ull,ull> pullull;typedef pair<ull, ll> pullll;typedef pair<ll, ull> pllull;typedef pair<char,char> pcc;typedef pair<char,uchar> pcuc;typedef pair<uchar,char> pucc;typedef pair<uchar,uchar> pucuc;typedef pair<int,ll> pill;typedef pair<ll,int> plli;typedef pair<uint,ll> puill;typedef pair<ll,uint> pllui;typedef pair<ull,int> pulli;typedef pair<int,ull> piull;typedef pair<uint,ull> puiull; typedef pair<ull,uint> pullui;typedef pair<char,ll> pcll;typedef pair<ll,char> pllc;typedef pair<uchar,ll> pucll;typedef pair<ll,uchar> plluc;typedef pair<ull,char> pullc;typedef pair<char,ull> pcull;typedef pair<uchar,ull> pucull; typedef pair<ull,uchar> pulluc;typedef pair<char,int> pci;typedef pair<int,char> pic;typedef pair<uchar,int> puci;typedef pair<int,uchar> piuc;typedef pair<uint,char> puic;typedef pair<char,uint> pcui;typedef pair<uchar,uint> pucui; typedef pair<uint,uchar> puiuc;struct llhash {size_t operator()(ll  x)const {return (int)x ^ (int)(x>>32);}};struct piihash{size_t operator()(pii x)const {return x.first^x.second;}};
#ifndef FASTIO
#define fin(x)
#define fout(x)
inline int read_int() {int r; cin>>r; return r;}inline uint read_uint() {uint r; cin>>r; return r;}inline ull read_ull() {ull r; cin>>r; return r;}inline char read_char() {char r; r=cin.get(); return r;}inline void write_int(int i) {cout<<i;}inline void write_uint(uint i) {cout<<i;}inline void write_ull(ull i) {cout<<i;}inline void write_char(char i) {cout<<i;}
#else
const int STDIN_BUFF_SIZE=32768;const int STDOUT_BUFF_SIZE=32768;
#define FREAD fread_unlocked
#define FWRITE fwrite_unlocked
char _stdin_buff[STDIN_BUFF_SIZE];int _stdin_buff_pos = 0;char _stdout_buff[STDOUT_BUFF_SIZE];int _stdout_buff_pos = 0;inline void fin(int minChars){int remaining = STDIN_BUFF_SIZE-_stdin_buff_pos;if(remaining > minChars) return;for(int i=0; i<remaining; ++i) _stdin_buff[i] = _stdin_buff[i+_stdin_buff_pos];FREAD(_stdin_buff+remaining, 1, _stdin_buff_pos, stdin);_stdin_buff_pos = 0;}inline void fout(int minChars){if(STDOUT_BUFF_SIZE - _stdout_buff_pos > minChars) return;FWRITE(_stdout_buff, 1, _stdout_buff_pos, stdout);_stdout_buff_pos = 0;}inline char read_char(){return _stdin_buff[_stdin_buff_pos++];}inline void write_char(char c){_stdout_buff[_stdout_buff_pos] = c;++_stdout_buff_pos;}inline unsigned int read_uint(){unsigned int r=0;char c = read_char();while(c < '-') c=read_char();for(;;){if(c < '0') return r;c -= '0';r = r*10 + c;c = read_char();}}inline int read_int(){int r=0;char c = read_char();while(c < '-') c=read_char();char minus = 0;if(c=='-'){c = read_char();minus = 1;}for(;;){if(c<'0') return minus?-r:r;c-='0';r = r*10 + c;c=read_char();}}const int _rs = 20;char _r[_rs];inline void write_uint(unsigned int i){if(i==0){write_char('0');return;}int pos = _rs;while(i){_r[--pos] = '0' + i%10;i/=10;}int j;for(j=pos; j<_rs; ++j) write_char(_r[j]);}inline void write_ull(unsigned long long i){if(i==0){write_char('0');return;}int pos = _rs;while(i){_r[--pos] = '0' + i%10;i/=10;}int j;for(j=pos; j<_rs; ++j) write_char(_r[j]);}inline void write_int(int i){if(i==0){write_char('0');return;}if(i<0){write_char('-');i=-i;}int pos = _rs;while(i){_r[--pos] = '0' + i%10;i/=10;}int j;for(j=pos; j<_rs; ++j) write_char(_r[j]);}
#endif
inline void tc();int main()
{
#ifdef FASTIO
    setvbuf ( stdin  , NULL , _IONBF , 0 );
    setvbuf ( stdout , NULL , _IONBF , 0 );
    FREAD(_stdin_buff, 1, STDIN_BUFF_SIZE, stdin);
#else
	ios_base::sync_with_stdio(0);
	cout.precision(40);
	cout.setf(ios::fixed, ios::floatfield);
#endif
#ifdef READ_NUM_TEST_CASES
	int nz;
#ifdef FASTIO
	fin(10);
	nz = read_uint();
#else
	cin >> nz;
#endif
	for(int i=0; i<nz; ++i)
#endif
	tc();
#ifdef FASTIO
    FWRITE(_stdout_buff, 1, _stdout_buff_pos, stdout);
#endif
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////









////////////////////////// TEMPLATES ///////////////////////////////////////////
//
template<bool cond, typename T, typename F> struct IF {typedef T R;};
template<typename T, typename F> struct IF<false,T,F> {typedef F R;};
template <bool B, class T = void> struct ENABLE_IF {typedef T R;};
template <class T> struct ENABLE_IF<false, T> {};
template <bool B, class T = void> struct DISABLE_IF {typedef T RET;};
template <class T> struct DISABLE_IF<true, T> {};
struct VOID {};
////////////////////////////////////////////////////////////////////////////////








////////////////////////////////////////////////////////////////////////////////
// Tuples
template<typename T1, typename T2 = void, typename T3 = void, typename T4 = void>
struct tuple
{
	inline tuple(const T1& e1, const T2& e2, const T3& e3, const T4& e4) :
		first(e1),second(e2),third(e3),fourth(e4){}
	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
};

template<typename T1, typename T2, typename T3>
struct tuple<T1,T2,T3,void>
{
	inline tuple(const T1& e1, const T2& e2, const T3& e3) : first(e1),second(e2),third(e3){}
	T1 first;
	T2 second;
	T3 third;
};

template<typename T1, typename T2, typename T3, typename T4>
tuple<T1,T2,T3,T4> make_tuple(const T1& e1, const T2& e2, const T3& e3, const T4& e4)
{
	return tuple<T1,T2,T3,T4>(e1,e2,e3,e4);
}

template<typename T1, typename T2, typename T3>
tuple<T1,T2,T3> make_tuple(const T1& e1, const T2& e2, const T3& e3)
{
	return tuple<T1,T2,T3>(e1,e2,e3);
}
////////////////////////////////////////////////////////////////////////////////











/////////////////////////////// STATIC ARRAY ///////////////////////////////////
// requires: TEMPLATES
template<typename T, int _capacity>
class StaticArray
{
public:
	StaticArray() : mSize(0) {}
	StaticArray(unsigned int size) : mSize(size) {ASS(size<=_capacity);}
	inline void push_back(const T& t) {new(&mData()[mSize]) T(t); ++mSize;}
	inline void push_back() {new(&mData()[mSize]) T; ++mSize;}
	inline void pop_back() {ASS(mSize>0); --mSize; mData()[mSize].~T();}
	inline T& operator[](const int idx) {return mData()[idx];}
	inline const T& operator[](const int idx) const {ASS(idx<mSize); return mData()[idx];}
	inline T& back() {ASS(mSize>0); return mData()[mSize-1];}
	inline const T& back() const {ASS(mSize>0); return mData()[mSize-1];}
	inline T& front() {ASS(mSize>0); return mData()[0];}
	inline const T& front() const {ASS(mSize>0); return mData()[0];}
	inline int size() const {return mSize;}
	inline unsigned int capacity() const {return _capacity;}
	inline void resize(int newSize)
	{
		ASS(newSize <= _capacity);
		if(newSize > mSize) new (&mData()[mSize]) T[newSize-mSize];
		else for(int i=newSize; i<mSize; ++i) mData()[i].~T();
		mSize = newSize;
	}
	inline void reserve(int howMuch) const {ASS(howMuch <= _capacity);}
	inline void clear() {for(int i=0; i<mSize; ++i) mData()[i].~T(); mSize=0;}
	typedef T* iterator;
	inline iterator begin() {return mData;}
	inline iterator end()   {return mData+mSize;}
private:
	T* mData() {return reinterpret_cast<T*>(_mData);}
	char _mData[_capacity*sizeof(T)];
	int mSize;
};
// vector of at most n elements
template<typename T, int n> struct _vector
{typedef typename IF<n==0,std::vector<T>,StaticArray<T,n> >::R R;};
////////////////////////////////////////////////////////////////////////////////








////////////////////////////////////////////////////////////////////////////////
// SPLAY TREE
// (c) 2013 Adam Blaszkiewicz. All rights reserved.
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
	inline BaseSplayNode(const _Value& _keyval, Derived* _parent, Derived* _l, Derived* _r) :
		keyval(_keyval), parent(_parent), l(_l), r(_r) {}

	// "pure virtual" update functions
	inline void update() {ASS(false);}
	inline void updateLR() {ASS(l && r);ASS(false);}
	inline void updateL() {ASS(l);ASS(false);}
	inline void updateR() {ASS(r);ASS(false);}
	inline void updateNL() {ASS(!l);ASS(false);}
	inline void updateNR() {ASS(!r);ASS(false);}
	inline void updateCopy(Derived* x) {ASS(false);}	// copy stats from x
};
template<typename _Value, typename Derived> struct CopyableSplayNode :
	public BaseSplayNode<_Value, Derived>
{
	typedef BaseSplayNode<_Value,Derived> B;//ASE
	inline void operator=(const CopyableSplayNode& o)
	{
		B::keyval = o.keyval;
		B::parent = o.parent;
		B::l = o.l;
		B::r = o.r;
		if(B::l) B::l->parent = (Derived*)this;
		if(B::r) B::r->parent = (Derived*)this;
		if(B::parent->l == &o) B::parent->l = (Derived*)this;
		else {ASS(B::parent->r==&o); B::parent->r = (Derived*)this;}
	}
	inline CopyableSplayNode(const CopyableSplayNode& o) :
		B(o.keyval,o.parent,o.l,o.r)
	{
		if(B::l) B::l->parent = (Derived*)this;
		if(B::r) B::r->parent = (Derived*)this;
		if(B::parent->l == &o) B::parent->l = (Derived*)this;
		else {ASS(B::parent->r==&o); B::parent->r = (Derived*)this;}
	}
	inline CopyableSplayNode(const _Value& _keyval, Derived* _parent) :
		B(_keyval,_parent) {}
};

// Default SplayNode: copyable, empty update functions
template<typename _Value> struct DefSplayNode :
	public CopyableSplayNode<_Value,DefSplayNode<_Value> >	// tell Base that it's our base
															// (avoid vtable)
{
	typedef CopyableSplayNode<_Value,DefSplayNode<_Value> > B;//ASE

	// Children-less initialization
	inline DefSplayNode(const _Value& _keyval, DefSplayNode* _parent) :
		CopyableSplayNode<_Value,DefSplayNode<_Value> >(_keyval,_parent) {}

	// empty update functions
	inline void update() {}
	inline void updateLR() {ASS(B::l && B::r);}
	inline void updateL() {ASS(B::l);}
	inline void updateR() {ASS(B::r);}
	inline void updateNL() {ASS(!B::l);}
	inline void updateNR() {ASS(!B::r);}
	inline void updateCopy(DefSplayNode* x) {}	// copy stats from x
};
//
// new + delete								2.88s
// nodes on vector:							2.81s
// nodes on vector + reserve:				2.76s
// nodes on vector, no copy constructors	2.76s
// after levels optim						2.74s (2.66??)
// splays everywhere, some optimizations	3.08s
//















////////////////////////////////////////////////////////////////////////////////
// SPLAYER (for splay trees)
// (c) 2013 Adam Blaszkiewicz. All rights reserved.
//
// Splayer contains some splay node operations
template<bool evertable = false>
struct Splayer
{
	template<typename SplayNode>
	static inline void updateUntilNode(SplayNode* x, const SplayNode* const superRoot)
	{
		while(x != superRoot)
		{
			ASS(x);
			x->update();
			x = x->parent;
		}
	}
	template<typename SplayNode>
	static inline void updateUntilNull(SplayNode* x)
	{
		updateUntilNode(x,NULL);
	}
	template<typename SplayNode>
	static inline void splayUntilNode(SplayNode *x,
		const SplayNode* const superRoot, int level)
	{
		//int level = 0;
		//SplayNode *pc = x;
		//while(pc->parent != superRoot()) pc = pc->parent, ++level;
		int level2 = level/2;
		while(level2) splay(x),--level2;
		if(level&1) rotate(x);
		ASS(x->parent == superRoot);
	}
	template<typename SplayNode>
	static inline void splayUntilNode(SplayNode *x, SplayNode *superRoot)
	{
		int level = 0;
		SplayNode *pc = x;
		while(pc->parent != superRoot) pc = pc->parent, ++level;
		splayUntilNode(x,superRoot,level);
	}
	template<typename SplayNode>
	static inline void splayUntilNull(SplayNode *x, int level)
	{
		if(level==0)return;
		if(level==1)rotateLast(x);
		else
		{
			int level2 = level/2;
			while(--level2) splay(x);
			if(level&1) splay(x),rotateLast(x);
			else splayLast(x);
		}
		ASS(x->parent == NULL);
	}
	// returns previous root
	template<typename SplayNode>
	static inline SplayNode* splayUntilNull(SplayNode *x)
	{
		int level = 0;
		SplayNode *pc = x;
		while(pc->parent) pc = pc->parent, ++level;
		splayUntilNull(x,level);
		return pc;
	}

	// splay only right
	template<typename SplayNode>
	static inline void splayRUntilNull(SplayNode *x, int level)
	{
		if(level==0)return;
		if(level==1)rotateRLast(x);
		else
		{
			int level2 = level/2;
			while(--level2) rotateR(x->parent),rotateR(x);
			if(level&1) rotateR(x->parent),rotateR(x),rotateRLast(x);
			else rotateRLast(x->parent),rotateRLast(x);
		}
		ASS(x->parent == NULL);
	}
	// returns previous root
	template<typename SplayNode>
	static inline SplayNode* splayRUntilNull(SplayNode *x)
	{
		int level = 0;
		SplayNode *pc = x;
		while(pc->parent) pc = pc->parent, ++level;
		splayRUntilNull(x,level);	// bugfix
		return pc;
	}


	// ROTATIONS

	// assumes x's parent is NOT root (its parent != NULL)
	template<typename SplayNode>
	static inline void rotateR(SplayNode* x)
	{
		ASS(x->parent->parent);
		SplayNode *prevParent = rotateRLast(x);
		ASS(x->parent);
		if(x->parent->l == prevParent) x->parent->l = x;
		else {ASS(x->parent->r == prevParent); x->parent->r = x;}
	}
	// assumes x's parent is root (its parent == NULL)
	// return prev parent
	template<typename SplayNode>
	static inline SplayNode* rotateRLast(SplayNode* x)
	{
		ASS(x); ASS(x->parent);
		ASS(x->parent->l == x);	// can rotate right
		SplayNode *parent = x->parent;
		SplayNode* xr;
		if(evertable && x->getSwapFlag())
		{
			xr = x->l;
			x->l = x->r;
			if(x->l) x->l->setSwapFlag(!x->l->getSwapFlag());
			if(xr) xr->setSwapFlag(!xr->getSwapFlag());
		}
		else
		{
			xr = x->r;
		}
		parent->l = xr;
		x->updateCopy(parent);
		if(xr)
		{
			xr->parent = parent;
			parent->updateL();
		}
		else parent->updateNL();
		x->parent = parent->parent;
		x->setSwapFlag(parent->getSwapFlag());
		parent->setSwapFlag(false);
		x->r = parent;
		parent->parent = x;
		return parent;
	}
	// assumes x's parent is NOT root (its parent != NULL)
	template<typename SplayNode>
	static inline void rotateL(SplayNode* x)
	{
		ASS(x->parent->parent);
		SplayNode *prevParent = rotateLLast(x);
		if(x->parent->l == prevParent) x->parent->l = x;
		else {ASS(x->parent->r == prevParent); x->parent->r = x;}
	}
	// assumes x's parent is root (its parent == NULL)
	// returns prev parent
	template<typename SplayNode>
	static inline SplayNode* rotateLLast(SplayNode* x)
	{
		ASS(x); ASS(x->parent);
		ASS(x->parent->r == x);	// can rotate left
		SplayNode *parent = x->parent;
		SplayNode *xl;
		if(evertable && x->getSwapFlag())
		{
			xl = x->r;
			x->r = x->l;
			if(x->r) x->r->setSwapFlag(!x->r->getSwapFlag());
			if(xl) xl->setSwapFlag(!xl->getSwapFlag());
		}
		else
		{
			xl = x->l;
		}
		parent->r = xl;
		x->updateCopy(parent);
		if(xl)
		{
			xl->parent = x->parent;
			parent->updateR();
		}
		else parent->updateNR();
		x->parent = parent->parent;
		x->setSwapFlag(parent->getSwapFlag());
		parent->setSwapFlag(false);
		x->l = parent;
		parent->parent = x;
		return parent;
	}
	// assumes x's parent is NOT root (its parent != null)
	template<typename SplayNode>
	static inline void rotate(SplayNode* x)
	{
		ASS(x); ASS(x->parent); ASS(x->parent->parent);
		if(x->parent->r == x) rotateL(x);
		else {ASS(x->parent->l == x); rotateR(x);}
	}
	// assumes x's parent is root (its parent == null)
	template<typename SplayNode>
	static inline void rotateLast(SplayNode* x)
	{
		ASS(x); ASS(x->parent);ASS(!x->parent->parent);
		if(x->parent->r == x) rotateLLast(x);
		else {ASS(x->parent->l == x); rotateRLast(x);}
	}

	// assumes x's grandparent is NOT root (its parent != NULL)
	template<typename SplayNode>
	static inline void splay(SplayNode *x)	// zig-zig or zig-zag
	{
		ASS(x); ASS(x->parent); // x not root
		ASS(x->parent->parent); // parent not root
		ASS(x->parent->parent->parent); // there is superRoot
		SplayNode *p = x->parent;
		SplayNode *g = p->parent;
		if(evertable && p->getSwapFlag())
		{
			if(p->l == x)
			{
				if(g->l == p) rotateR(x), rotateR(x);
				else {ASS(g->r == p); rotateL(p), rotateL(x);}
			}
			else
			{
				ASS(p->r == x);
				if(g->l == p) rotateR(p), rotateR(x);
				else {ASS(g->r == p); rotateL(x), rotateL(x);}
			}
		}
		else
		{
			if(p->l == x)
			{
				if(g->l == p) rotateR(p), rotateR(x);
				else {ASS(g->r == p); rotateR(x), rotateL(x);}
			}
			else
			{
				ASS(p->r == x);
				if(g->l == p) rotateL(x), rotateR(x);
				else {ASS(g->r == p); rotateL(p), rotateL(x);}
			}
		}
	}

	// assumes x's grandparent is root (its parent == NULL)
	template<typename SplayNode>
	static inline void splayLast(SplayNode *x)	// zig-zig or zig-zag
	{
		ASS(x); ASS(x->parent); // x not root
		ASS(x->parent->parent); // parent not root
		SplayNode *p = x->parent;
		SplayNode *g = p->parent;
		if(evertable && p->getSwapFlag())
		{
			if(p->l == x)
			{
				if(g->l == p) rotateR(x), rotateRLast(x);
				else {ASS(g->r == p); rotateLLast(p), rotateLLast(x);}
			}
			else
			{
				ASS(p->r == x);
				if(g->l == p) rotateRLast(p), rotateRLast(x);
				else {ASS(g->r == p); rotateL(x), rotateLLast(x);}
			}
		}
		else
		{
			if(p->l == x)
			{
				if(g->l == p) rotateRLast(p), rotateRLast(x);
				else {ASS(g->r == p); rotateR(x), rotateLLast(x);}
			}
			else
			{
				ASS(p->r == x);
				if(g->l == p) rotateL(x), rotateRLast(x);
				else {ASS(g->r == p); rotateLLast(p), rotateLLast(x);}
			}
		}
	}
	// debug
	template<typename SplayNode>
	static inline bool consistent(const SplayNode *pv)
	{
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
///////////////////////////////////////////////////////////////////////////////















//////////////////////////// LINK-CUT //////////////////////////////////////////
//
// Implementation (c) 2013 by Adam Blaszkiewicz. All rights reserved.
//
// Tarjan's ST-trees (aka Link-Cut (aka dynamic trees))
// rootable means that root can be changed to another vertex in the tree
// rootable implies that underlying splay trees are evertable
//
// PV is a pointer to struct with:
// - PV l,r,parent,pathParent;
// bool getSwapFlag()
// void setSwapFlag(bool)
template<typename PV, bool rootable = false>
class LinkCut
{
	typedef Splayer<rootable> S;
public:
	// links a tree rootet at u to node v
	// u must be tree root! (so no link(u,_) was called earlier)
	// assumes there are no cycles
	static inline void link(const PV& u, const PV& v)
	{
		if(rootable) splayPath(u);
		else splayPathR(u);

		ASS(u->parent == NULL);
		ASS(u->pathParent == NULL);
		ASS(!(rootable && u->getSwapFlag()) || u->r == NULL);
		ASS( (rootable && u->getSwapFlag()) || u->l == NULL);
		u->pathParent = v;
	}
	// cuts pv from its parent
	static inline void cut(const PV& pv)
	{
		splayPath(pv);
		ASS(pv->pathParent || pv->l ||
			(pv->r && rootable && pv->getSwapFlag()));	// cut legal
		if(rootable && pv->getSwapFlag())
		{
			if(pv->r)
			{
				pv->r->parent = NULL;
				pv->r->pathParent = pv->pathParent;
				pv->r->setSwapFlag(!pv->r->getSwapFlag());	// xor
				pv->pathParent = NULL;
				pv->r = NULL;
				pv->updateNR();
			}
			else
			{
				ASS(pv->pathParent);
				pv->pathParent = NULL;
			}
		}
		else
		{
			if(pv->l)
			{
				pv->l->parent = NULL;
				pv->l->pathParent = pv->pathParent;
				pv->pathParent = NULL;
				pv->l = NULL;
				pv->updateNL();
			}
			else
			{
				ASS(pv->pathParent);
				pv->pathParent = NULL;
			}
		}
	}
	// cuts pv from its preferred child
	static inline void cutChild(const PV& pv)
	{
		splayPath(pv);
		if(rootable && pv->getSwapFlag())
		{
			ASS(pv->l);
			pv->l->parent = NULL;
			pv->l->pathParent = NULL;
			pv->l->setSwapFlag(!pv->l->getSwapFlag());
			pv->l = NULL;
			pv->updateNL();
		}
		else
		{
			ASS(pv->r);
			pv->r->parent = NULL;
			pv->r->pathParent = NULL;
			pv->r = NULL;
			pv->updateNR();
		}
	}
	// makes pv a new root (evert)
	static inline void makeRoot(const PV& pv)
	{
		ASS(rootable);
		access(pv,true);
		// after access with cutting pv's sons
		// it suffices to reverse the preferred path
		pv->setSwapFlag(!pv->getSwapFlag());
	}
	// builds preferred path from root to pv (and possibly further if cutPrefChild==false)
	// after this, pv is the root of this path's tree
	// pv->l can be used to get aggregate root--pv path values
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
					pv->updateNL();
				}
			}
			else
			{
				if(pv->r)
				{
					pv->r->parent = NULL;
					pv->r->pathParent = pv;
					pv->r = NULL;
					pv->updateNR();
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

			//splayPath(pparent);
			const PV prevRoot = S::splayUntilNull(pparent);
			ASS(pparent->parent == NULL);
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
					//pparent->l = NULL;
				}
				pparent->l = pv;
				pv->parent = pparent;
				pv->setSwapFlag(!pv->getSwapFlag());
				//pv->pathParent = NULL;
				pparent->updateL();
				S::rotateRLast(pv);
			}
			else
			{
				if(pparent->r) // if old child exists, disconnect it
				{
					pparent->r->parent = NULL;
					//pparent->r = NULL;
					pparent->r->pathParent = pparent;
				}
				pparent->r = pv;
				pv->parent = pparent;
				//pv->pathParent = NULL;
				pparent->updateR();
				S::rotateLLast(pv);
			}
			ASS(pv->parent == NULL);

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


		ASS(c->parent == NULL);
		ASS(!c->pathParent);
		//pv->pathParent = NULL;

		return c;
	}
private:
	// Splay pv to root of its auxliary tree
	// returns previous root
	static inline const PV splayPath(const PV& pv)
	{
		PV prevRoot = S::splayUntilNull(pv);
		ASS(pv->parent==NULL);
		pv->pathParent = prevRoot->pathParent;
		//prevRoot->pathParent = NULL;
		return prevRoot;
	}
	// Splay pv to root of its auxliary tree using only right rotations
	// assumes pv is the smallest element in its tree
	static inline void splayPathR(const PV& pv)
	{
		const PV& prevRoot = S::splayRUntilNull(pv);
		ASS(pv->parent==NULL);
		pv->pathParent = prevRoot->pathParent;
		//prevRoot->pathParent = NULL;
	}
};
//////////////////////////////////////////// link-cut //////////////////////////

















int n,m;

struct vertex
{
	vertex() : l(0), r(0), parent(0),pathParent(0) {}
	vertex *l,*r,*parent,*pathParent;

	// empty update functions (not used)
	inline void update() {}
	inline void updateLR() {ASS(l && r);}
	inline void updateL() {ASS(l);}
	inline void updateR() {ASS(r);}
	inline void updateNL() {ASS(!l);}
	inline void updateNR() {ASS(!r);}
	inline void updateCopy(vertex* x) {ASS(x);}	// copy stats from x

	// not used (trees not revertable)
	inline bool getSwapFlag() const {return false;}
	inline void setSwapFlag(bool) const {}
};

vertex vs[100009];

typedef LinkCut<vertex*> LC;

inline void tc()
{
	n=read_uint();
	m=read_uint();

	rep(i,m)
	{
		fin(30);
		char c=0;
		while(c!='l' && c!='c')c = read_char();
		c=read_char();

		while(read_char()!=' ');

		if(c=='c')	// lca
		{
			fout(10);
			int u=read_uint();
			int v=read_uint();
			if(u==v)
			{
				write_uint(u);
				write_char('\n');
				continue;
			}
			--u;--v;
			LC::access(&vs[u],true); // access with 'cut pref child'
			vertex* lca = LC::access(&vs[v]).fi;
			write_uint(lca-vs + 1),write_char('\n');
		}
		else if(c=='i') // link
		{
			int u=read_uint();
			int v=read_uint();
			--u;
			--v;
			LC::link(&vs[u],&vs[v]);
		}
		else if(c=='u') // cut
		{
			int x = read_uint();
			--x;
			LC::cut(&vs[x]);
		}
	}
}






// SplaySet test
/*

template<typename T>
class SplaySet : public SplayTree<T,T,_Identity<T>, 0 >{};


inline void tc()
{
	cout.precision(4);

	int seed = time(0);
	const int numIns = 1000000;
	const int numOps = 1000000;
	SplaySet<int> sp;
	set<int> se;

	int s;

	sp.reserve(1000000);
	//se.reserve(1000000);

	cout<<"RANDOM:"<<endl;
	s = clock();
	srand(seed);
	for(int i=0; i<numIns; ++i)
	{
		int val = rand()%(numIns*2);
		sp.insert(val);
	}
	for(int i=0; i<numOps; ++i)
	{
		int val = rand()%(numIns*2);
		sp.safe_erase(val);
		val = rand()%(numIns*2);
		sp.insert(val);
	}
	cout << "Splay time: " << (double)(clock()-s)/CLOCKS_PER_SEC << endl;

	s = clock();
	srand(seed);
	for(int i=0; i<numIns; ++i)
	{
		int val = rand()%(numIns*2);
		se.insert(val);
	}
	for(int i=0; i<numOps; ++i)
	{
		int val = rand()%(numIns*2);
		se.erase(val);
		val = rand()%(numIns*2);
		se.insert(val);
	}
	cout << "Set time: " << (double)(clock()-s)/CLOCKS_PER_SEC << endl;




	bool ok = true;
	int num = 0;
	set<int>::iterator itr2 = se.begin();
	for(SplaySet<int>::iterator itr = sp.begin();
		itr != sp.end(); ++itr, ++itr2)
	{
		if(*itr != *itr2)
		{
			ok=false;
			break;
		}
		else ++num;
	}
	if(ok) cout << num << " values OK."<<endl;
	else cout<<"WA!!!"<<endl;


	sp.clear();
	se.clear();
	cout<<"LOCALITY:"<<endl;
	s = clock();
	srand(seed);
	for(int i=0; i<numIns; ++i)
	{
		int val = rand()%(numIns*2);
		sp.insert(val);
	}
	for(int i=0; i<numOps; ++i)
	{
		int val = rand()%(numIns*2/10000);
		sp.safe_erase(val);
		val = rand()%(numIns*2/10000);
		sp.insert(val);
	}
	cout << "Splay time: " << (double)(clock()-s)/CLOCKS_PER_SEC << endl;

	s = clock();
	srand(seed);
	for(int i=0; i<numIns; ++i)
	{
		int val = rand()%(numIns*2);
		se.insert(val);
	}
	for(int i=0; i<numOps; ++i)
	{
		int val = rand()%(numIns*2/10000);
		se.erase(val);
		val = rand()%(numIns*2/10000);
		se.insert(val);
	}
	cout << "Set time: " << (double)(clock()-s)/CLOCKS_PER_SEC << endl;
}


*/
























