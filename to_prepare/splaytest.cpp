//#define READ_NUM_TEST_CASES
//#define FASTIO	// enable fastio

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
char _stdin_buff[STDIN_BUFF_SIZE];int _stdin_buff_pos = 0;char _stdout_buff[STDOUT_BUFF_SIZE];int _stdout_buff_pos = 0;inline void fin(int minChars){int remaining = STDIN_BUFF_SIZE-_stdin_buff_pos;if(remaining > minChars) return;for(int i=0; i<remaining; ++i) _stdin_buff[i] = _stdin_buff[i+_stdin_buff_pos];FREAD(_stdin_buff+remaining, 1, _stdin_buff_pos, stdin);_stdin_buff_pos = 0;}inline void fout(int minChars){if(STDOUT_BUFF_SIZE - _stdout_buff_pos > minChars) return;FWRITE(_stdout_buff, 1, _stdout_buff_pos, stdout);_stdout_buff_pos = 0;}inline char read_char(){return _stdin_buff[_stdin_buff_pos++];}inline void write_char(char c){_stdout_buff[_stdout_buff_pos] = c;++_stdout_buff_pos;}inline unsigned int read_uint(){unsigned int r=0;char c = read_char();while(c < '-') c=read_char();for(;;){if(c < '0') return r;c -= '0';r = r*10 + c;c = read_char();}}inline int read_int(){int r=0;char c = read_char();while(c < '-') c=read_char();char minus = 0;if(c=='-'){c = read_char();minus = 1;}for(;;){if(c<'0') return minus?-r:r;c-='0';r = r*10 + c;c=read_char();}}const int _rs = 20;char __r[_rs];inline void write_uint(unsigned int i){if(i==0){write_char('0');return;}int pos = _rs;while(i){__r[--pos] = '0' + i%10;i/=10;}int j;for(j=pos; j<_rs; ++j) write_char(__r[j]);}inline void write_ull(unsigned long long i){if(i==0){write_char('0');return;}int pos = _rs;while(i){__r[--pos] = '0' + i%10;i/=10;}int j;for(j=pos; j<_rs; ++j) write_char(__r[j]);}inline void write_int(int i){if(i==0){write_char('0');return;}if(i<0){write_char('-');i=-i;}int pos = _rs;while(i){__r[--pos] = '0' + i%10;i/=10;}int j;for(j=pos; j<_rs; ++j) write_char(__r[j]);}
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
	inline tuple(const T1& _e1, const T2& _e2, const T3& _e3, const T4& _e4) :
		first(_e1),second(_e2),third(_e3),fourth(_e4){}
	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
};

template<typename T1, typename T2, typename T3>
struct tuple<T1,T2,T3,void>
{
	inline tuple(const T1& _e1, const T2& _e2, const T3& _e3) :
		first(_e1),second(_e2),third(_e3){}
	T1 first;
	T2 second;
	T3 third;
};

template<typename T1, typename T2, typename T3, typename T4>
tuple<T1,T2,T3,T4> make_tuple(const T1& _e1, const T2& _e2, const T3& _e3, const T4& _e4)
{
	return tuple<T1,T2,T3,T4>(_e1,_e2,_e3,_e4);
}

template<typename T1, typename T2, typename T3>
tuple<T1,T2,T3> make_tuple(const T1& _e1, const T2& _e2, const T3& _e3)
{
	return tuple<T1,T2,T3>(_e1,_e2,_e3);
}
////////////////////////////////////////////////////////////////////////////////












/////////////////////////////// STATIC ARRAY ///////////////////////////////////
// requires: TEMPLATES
template<typename T, int _capacity>
class StaticArray
{
public:
	StaticArray() : mSize(0) {}
	StaticArray(unsigned int _size) : mSize(_size) {ASS(_size<=_capacity);}
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













////////////////////////// SPLAY NODES /////////////////////////////////////////
//
// Copyright (c) 2013 by Adam Blaszkiewicz. All rights reserved.
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

	// PATH AGGREGATION
	// "pure virtual" update functions
	inline void update() {ASS(false);}
	inline void updateLR() {ASS(l && r);ASS(false);}
	inline void updateL() {ASS(l);ASS(false);}
	inline void updateR() {ASS(r);ASS(false);}
	inline void updateNL() {ASS(!l);ASS(false);}
	inline void updateNR() {ASS(!r);ASS(false);}
	inline void updateCopy(Derived* x) {ASS(false);}	// copy stats from x

	// TODO: path aggregation for linear values - could be faster,
	//   especially when updating ancestors of deleted node

	// swap flag - for evert (rerooting)
	inline bool getSwapFlag() const {ASS(false); return 0;}
	inline void setSwapFlag(bool) {ASS(false);}

	// TREE AGGREGATION (only linear values)
	// suitable for sum, not suitable for max
	inline void addVal(Derived* x) {ASS(false);}
	inline void subVal(Derived* x) {ASS(false);}
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
////////////////////////////////////////////////////////////////////////////////


















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
		parent->subVal(x);
		parent->addVal(xr);
		x->subVal(xr);
		x->addVal(parent);
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
		parent->subVal(x);
		parent->addVal(xl);
		x->subVal(xl);
		x->addVal(parent);
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
	template<typename SplayNode>	// TODO: can be optimized if splays aren't
									// done using rotate functions but directly
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
////////////////////////////////////////////////////////////////////////////////










///////////////////////////// SPLAY TREE ///////////////////////////////////////
//
// Implementation (c) 2013 by Adam Blaszkiewicz. All rights reserved.
//
// Common SplayTree
//
// new + delete								2.88s
// nodes on vector:							2.81s
// nodes on vector + reserve:				2.76s
// nodes on vector, no copy constructors	2.76s
// after levels optim						2.74s (2.66??)
// splays everywhere, some optimizations	3.08s
// suddenly 3.40s
//
// does not support evert (swapFlag) (TODO?)
template<typename _Key, typename _Value,
	typename _KeyOfVal, int MaxNodes = 0,
	typename SplayNode = DefSplayNode<_Value>,
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
	typedef Splayer<> S;
public:
	inline SplayTree()
	{
		superRoot()->r = NULL;
		superRoot()->l = NULL;
		superRoot()->parent = NULL;
	}
	inline ~SplayTree()
	{
		//ASS(!superRoot()->r);
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
	SplayNode* newNode(const SplayNode& node, SplayNode* hint = NULL)
	{
		_v.push_back(node);
		return &_v.back();
		//return new SplayNode(_keyval);
	}
	void delNode(SplayNode* sn)
	{
		if(sn!=&_v.back()) *sn = _v.back();
		_v.pop_back();
		//delete sn;
	}
public:
	inline iterator find(const value_type& _keyval, const bool splay = true)
	{
		tuple<SplayNode*,SplayNode**,int> r = _find(_keyval);
		if(*r.se)
		{
			if(splay)
			{
				S::splayUntilNode(*r.se,superRoot(),r.th);
				return iterator(superRoot()->l);
			}
			else return iterator(*r.se);
		}
		else
		{
			if(splay && r.th>0) S::splayUntilNode(r.fi,superRoot(),r.th-1);
			return iterator(superRoot());
		}
	}
	inline pair<iterator,bool> insert(const value_type& _keyval, const bool splay = true)
	{
		if(_v.size()==_v.capacity())_v.reserve(_v.size()*3/2);
		tuple<SplayNode*,SplayNode**,int> re = _find(_KeyOfVal()(_keyval));
		if(*re.se)
		{
			if(splay) S::splayUntilNode(*re.se,superRoot(),re.th);
			return make_pair(iterator(*re.se),false);
		}
		else
		{
			SplayNode nn(_keyval,re.fi);
			*re.se = &nn;
			newNode(nn,re.fi);
			if(splay) S::splayUntilNode(*re.se,superRoot(),re.th);
			return make_pair(iterator(*re.se),true);
		}
	}
	inline iterator insertMulti(const value_type& _keyval, const bool splay = true)
	{
		if(_v.size()==_v.capacity())_v.reserve(_v.size()*3/2);
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
		ASS(itr != end());
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
		_v.clear();
		superRoot()->l = NULL;
	}
	inline void reserve(unsigned int size)
	{
		_v.reserve(size);
	}

private:
	// assumes x has no children
	inline void _justErase(SplayNode* x)
	{
		ASS(x != superRoot());
		ASS(!x->l); ASS(!x->r);
		if(x->parent->r == x)
		{
			x->parent->r = NULL;
			if(x->parent != superRoot())
			{
				x->parent->updateNR();
				SplayNode* c = x->parent;
				while(c!=superRoot())
				{
					c->update();
					c->subVal(x);
					c = c->parent;
				}
			}
		}
		else
		{
			ASS(x->parent->l == x);
			x->parent->l = NULL;
			if(x->parent != superRoot())
			{
				x->parent->updateNL();
				SplayNode* c = x->parent;
				while(c!=superRoot())
				{
					c->update();
					c->subVal(x);
					c = c->parent;
				}
			}
		}
		delNode(x);
	}
	// assumes x has only left child
	inline void _eraseL(SplayNode* x)
	{
		ASS(x != superRoot());
		ASS(x->l); ASS(!x->r);
		if(x->parent->l == x)
		{
			x->parent->l = x->l;
			if(x->parent != superRoot())
			{
				x->parent->updateL();
				SplayNode* c = x->parent;
				while(c!=superRoot())
				{
					c->update();
					c->subVal(x);
					c = c->parent;
				}
			}
		}
		else
		{
			ASS(x->parent->r == x);
			x->parent->r = x->l;
			if(x->parent != superRoot())
			{
				x->parent->updateR();
				SplayNode* c = x->parent;
				while(c!=superRoot())
				{
					c->update();
					c->subVal(x);
					c = c->parent;
				}
			}
		}
		x->l->parent = x->parent;
		delNode(x);
	}
	// assumes x has only right child
	inline void _eraseR(SplayNode* x)
	{
		ASS(x != superRoot());
		ASS(!x->l); ASS(x->r);
		if(x->parent->l == x)
		{
			x->parent->l = x->r;
			if(x->parent != superRoot())
			{
				x->parent->updateL();
				SplayNode* c = x->parent;
				while(c!=superRoot())
				{
					c->update();
					c->subVal(x);
					c = c->parent;
				}
			}
		}
		else
		{
			ASS(x->parent->r == x);
			x->parent->r = x->r;
			if(x->parent != superRoot())
			{
				x->parent->updateR();
				SplayNode* c = x->parent;
				while(c!=superRoot())
				{
					c->update();
					c->subVal(x);
					c = c->parent;
				}
			}
		}
		x->r->parent = x->parent;
		delNode(x);
	}
	// assumes x has both children
	// replaces x with its predecessor
	inline void _eraseIn(SplayNode* const x, const bool splay = true)
	{
		ASS(x != superRoot());
		ASS(x->l); ASS(x->r);
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
					c->subVal(c->l);
					c->l->parent = p;
				}

				int level2 = level/2;
				while(level2)
				{
					S::rotateL(p->parent);
					S::rotateL(p);
					--level2;
				}
				if(level&1)
				{
					S::rotateL(p);
				}
				p->subVal(c);
				p->updateL();
				ASS(p->parent == x);	// splay succeeded
			}
			else // if(!splay)
			{
				while(c->r) c = c->r;
				c->parent->r = c->l;
				if(c->l)
				{
					c->subVal(c->l);
					c->l->parent = c->parent;
					c->parent->updateR();
				}
				else c->parent->updateNR();

				c->parent->subVal(c);

				SplayNode *cn = c->parent->parent;
				while(cn != x)
				{
					cn->updateR();
					cn->subVal(c);
					cn = cn->parent;
				}
			}
			c->l = x->l;
			c->l->parent = c;
			c->parent = x->parent;
			if(x->parent->l == x) x->parent->l = c;
			else {ASS(x->parent->r == x); x->parent->r = c;}
			c->r = x->r;
			c->r->parent = c;
			c->updateLR();
			c->addVal(c->l);
			c->addVal(c->r);
		}
		else
		{
			c->parent = x->parent;
			if(x->parent->l == x) x->parent->l = c;
			else {ASS(x->parent->r == x); x->parent->r = c;}
			c->r = x->r;
			c->r->parent = c;
			c->updateR();
			c->addVal(c->r);
		}
		for(;;)
		{
			c = c->parent;
			if(c==superRoot())break;
			c->update();
			c->subVal(x);
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
		if(splay && parent!=superRoot())
		{
			if(level != -1) S::splayUntilNode(parent,superRoot(),level-1);
			else S::splayUntilNode(parent,superRoot());
		}
	}
public:
	class iterator
	{	friend class SplayTree;
	private:
		inline iterator(SplayNode *_p) : p(_p) {}
	public:
		inline bool operator==(const iterator& o) const {return p==o.p;}
		inline bool operator!=(const iterator& o) const {return !(*this==o);}
		inline value_type& operator*() {return p->keyval;}
		inline const value_type& operator*() const {return p->keyval;}
		inline value_type* operator->() {return &p->keyval;}
		inline const value_type* operator->() const {return &p->keyval;}
		inline iterator& operator++()
		{
			ASS(p);
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
			ASS(p);
			if(p->l)
			{
				p = p->l;
				while(p->r) p = p->r;
			}
			else
			{
				while(p->parent->l == p) {p = p->parent; ASS(p!=superRoot());}
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
private:
	typename _vector<SplayNode,MaxNodes>::R _v;

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
		while(_KeyOfVal()((cand)->keyval) != _key)
		{
			++level;
			if(_KeyOfVal()((cand)->keyval) <= _key)
			{
				if((cand)->r) cand = (cand)->r;
				else return make_tuple((cand),&(cand)->r,level);
			}
			else
			{
				if((cand)->l) cand = (cand)->l;
				else return make_tuple((cand),&(cand)->l,level);
			}
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





















// SplaySet test

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



	{
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
	}

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



	{
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
	}
}























