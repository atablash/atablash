//#define READ_NUM_TEST_CASES
//#define FASTIO	// enable fastio
#define UNLOCKED	// fread_unlocked, fwrite_unlocked
#define IOSTREAM

////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_BUILD
#undef FASTIO
#include<cassert>
#endif
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<climits>
#include<cfloat>
#if defined FASTIO or !defined IOSTREAM
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
#define fi first
#define se second
#define pub push_back
#define puf push_front
#define pob pop_back
#define pof pop_front
#define ins insert
#define era erase
#define it iterator
#define type(x) __typeof(x)
#define var(a,from) __typeof(from) a(from)
#define foreach(itr,cont) for(__typeof((cont).begin()) itr = (cont).begin(); itr != (cont).end(); ++itr)
#define foreachr(itr,cont) for(__typeof((cont).rbegin()) itr = (cont).rbegin(); itr != (cont).rend(); ++itr)
#define rep(i,n) for(int i=0; i<(n); ++i)
#define per(i,n) for(int i=(n)-1; i>=0; --i)
#define fo(i,a,b) for(int i=(a); i<(b); ++i)
#define of(i,a,b) for(int i=(b)-1; i>=(a); --i)
#define sz(c) ((int)(c).size())
using namespace std;using namespace __gnu_cxx;typedef char BOOL;typedef long long ll;typedef unsigned int uint;typedef unsigned short ushort;typedef unsigned char uchar;typedef unsigned long long ull;typedef pair<int,int> pii;typedef pair<uint,uint> puiui;typedef pair<int,uint> piui;typedef pair<uint,int> puii;typedef pair< ll,ll> pllll;typedef pair<ull,ull> pullull;typedef pair<ull, ll> pullll;typedef pair<ll, ull> pllull;typedef pair<char,char> pcc;typedef pair<char,uchar> pcuc;typedef pair<uchar,char> pucc;typedef pair<uchar,uchar> pucuc;typedef pair<int,ll> pill;typedef pair<ll,int> plli;typedef pair<uint,ll> puill;typedef pair<ll,uint> pllui;typedef pair<ull,int> pulli;typedef pair<int,ull> piull;typedef pair<uint,ull> puiull; typedef pair<ull,uint> pullui;typedef pair<char,ll> pcll;typedef pair<ll,char> pllc;typedef pair<uchar,ll> pucll;typedef pair<ll,uchar> plluc;typedef pair<ull,char> pullc;typedef pair<char,ull> pcull;typedef pair<uchar,ull> pucull; typedef pair<ull,uchar> pulluc;typedef pair<char,int> pci;typedef pair<int,char> pic;typedef pair<uchar,int> puci;typedef pair<int,uchar> piuc;typedef pair<uint,char> puic;typedef pair<char,uint> pcui;typedef pair<uchar,uint> pucui; typedef pair<uint,uchar> puiuc;struct llhash {size_t operator()(ll  x)const {return (int)x ^ (int)(x>>32);}};struct piihash{size_t operator()(pii x)const {return x.fi^x.se;}};
#ifdef DEBUG_BUILD
#define A(x) assert(x);
#define D
#else
#define A(x)
#define D if(0)
#endif
#define NL WNL();
#ifndef FASTIO
inline void fin(int){}inline void fout(int){}
#ifdef IOSTREAM
inline int rint()
{
    int r;
    cin>>r;
    return r;
}
inline uint ruint()
{
    uint r;
    cin>>r;
    return r;
}
inline ull rull()
{
    ull r;
    cin>>r;
    return r;
}
inline char rchar()
{
    char r = cin.get();
    return r;
}
template<typename T>inline void W(const T& i)
{
    cout<<i;
}
#else
inline int rint()
{
    int r;
    scanf("%d",&r);
    return r;
}
inline uint ruint()
{
    uint r;
    scanf("%u",&r);
    return r;
}
inline ull rull()
{
    ull r;
    scanf("%llu",&r);
    return r;
}
inline ull rll()
{
    ll r;
    scanf("%lld",&r);
    return r;
}
inline char rchar()
{
	return getchar();
}
inline void W(char a)
{
	printf("%c",a);
}
inline void W(int a)
{
	printf("%d",a);
}
inline void W(uint a)
{
	printf("%u",a);
}
inline void W(const ull& a)
{
	printf("%llu",a);
}
inline void W(const ll& a)
{
	printf("%lld",a);
}
#endif
#else
#ifdef UNLOCKED
#define FREAD fread_unlocked
#define FWRITE fwrite_unlocked
#else
#define FREAD fread
#define FWRITE fwrite
#endif
const int STDIN_BUFF_SIZE=32768;
const int STDOUT_BUFF_SIZE=32768;
char _stdin_buff[STDIN_BUFF_SIZE];
int _stdin_buff_pos = 0;
char _stdout_buff[STDOUT_BUFF_SIZE];
int _stdout_buff_pos = 0;
inline void fin(int minChars)
{
    int remaining = STDIN_BUFF_SIZE-_stdin_buff_pos;
    if(remaining > minChars) return;
    for(int i=0; i<remaining; ++i) _stdin_buff[i] = _stdin_buff[i+_stdin_buff_pos];
    FREAD(_stdin_buff+remaining, 1, _stdin_buff_pos, stdin);
    _stdin_buff_pos = 0;
}
inline void fout(int minChars)
{
    if(STDOUT_BUFF_SIZE - _stdout_buff_pos > minChars) return;
    FWRITE(_stdout_buff, 1, _stdout_buff_pos, stdout);
    _stdout_buff_pos = 0;
}
inline char rchar()
{
    return _stdin_buff[_stdin_buff_pos++];
}
inline void W(char c)
{
    _stdout_buff[_stdout_buff_pos] = c;
    ++_stdout_buff_pos;
}
inline unsigned int ruint()
{
    unsigned int r=0;
    char c = rchar();
    while(c < '-') c=rchar();
    for(;;)
    {
        if(c < '0') return r;
        c -= '0';
        r = r*10 + c;
        c = rchar();
    }
}
inline int rint()
{
    int r=0;
    char c = rchar();
    while(c < '-') c=rchar();
    char minus = 0;
    if(c=='-')
    {
        c = rchar();
        minus = 1;
    }
    for(;;)
    {
        if(c<'0') return minus?-r:r;
        c-='0';
        r = r*10 + c;
        c=rchar();
    }
}
const int _rs = 20;
char _r[_rs];
inline void W(unsigned int i)
{
    if(i==0)
    {
        W('0');
        return;
    }
    int pos = _rs;
    while(i)
    {
        _r[--pos] = '0' + i%10;
        i/=10;
    }
    int j;
    for(j=pos; j<_rs; ++j) W(_r[j]);
}
inline void W(unsigned long long i)
{
    if(i==0)
    {
        W('0');
        return;
    }
    int pos = _rs;
    while(i)
    {
        _r[--pos] = '0' + i%10;
        i/=10;
    }
    int j;
    for(j=pos; j<_rs; ++j) W(_r[j]);
}
inline void W(int i)
{
    if(i==0)
    {
        W('0');
        return;
    }
    if(i<0)
    {
        W('-');
        i=-i;
    }
    int pos = _rs;
    while(i)
    {
        _r[--pos] = '0' + i%10;
        i/=10;
    }
    int j;
    for(j=pos; j<_rs; ++j) W(_r[j]);
}
#endif
inline void WNL(){W('\n');}inline void tc();int main()
{
#ifdef FASTIO
    setvbuf ( stdin  , NULL , _IONBF , 0 );
    setvbuf ( stdout , NULL , _IONBF , 0 );
    FREAD(_stdin_buff, 1, STDIN_BUFF_SIZE, stdin);
#else
#ifdef IOSTREAM
	ios_base::sync_with_stdio(0);
	cout.precision(40);
	cout.setf(ios::fixed, ios::floatfield);
#endif
#endif
#ifdef READ_NUM_TEST_CASES
	int nz;
#ifdef FASTIO
	fin(10);
	nz = ruint();
#else
#ifdef IOSTREAM
	cin >> nz;
#else
	scanf("%d",&nz);
#endif
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
			A(x);
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
		A(x->parent == superRoot);
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
		A(x->parent == NULL);
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
		A(x->parent == NULL);
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
		A(x->parent->parent);
		SplayNode *prevParent = rotateRLast(x);
		A(x->parent);
		if(x->parent->l == prevParent) x->parent->l = x;
		else {A(x->parent->r == prevParent); x->parent->r = x;}
	}
	// assumes x's parent is root (its parent == NULL)
	// return prev parent
	template<typename SplayNode>
	static inline SplayNode* rotateRLast(SplayNode* x)
	{
		A(x); A(x->parent);
		A(x->parent->l == x);	// can rotate right
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
		x->updateCopy(parent);
		if(xr)
		{
			parent->addVal(xr);
			x->subVal(xr);
			xr->parent = parent;
			parent->updateL();
		}
		else parent->updateNL();
		x->addVal(parent);
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
		A(x->parent->parent);
		SplayNode *prevParent = rotateLLast(x);
		if(x->parent->l == prevParent) x->parent->l = x;
		else {A(x->parent->r == prevParent); x->parent->r = x;}
	}
	// assumes x's parent is root (its parent == NULL)
	// returns prev parent
	template<typename SplayNode>
	static inline SplayNode* rotateLLast(SplayNode* x)
	{
		A(x); A(x->parent);
		A(x->parent->r == x);	// can rotate left
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
		x->updateCopy(parent);
		if(xl)
		{
			parent->addVal(xl);
			x->subVal(xl);
			xl->parent = x->parent;
			parent->updateR();
		}
		else parent->updateNR();
		x->addVal(parent);
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
		A(x); A(x->parent); A(x->parent->parent);
		if(x->parent->r == x) rotateL(x);
		else {A(x->parent->l == x); rotateR(x);}
	}
	// assumes x's parent is root (its parent == null)
	template<typename SplayNode>
	static inline void rotateLast(SplayNode* x)
	{
		A(x); A(x->parent);A(!x->parent->parent);
		if(x->parent->r == x) rotateLLast(x);
		else {A(x->parent->l == x); rotateRLast(x);}
	}

	// assumes x's grandparent is NOT root (its parent != NULL)
	template<typename SplayNode>	// TODO: can be optimized if splays aren't
									// done using rotate functions but directly
	static inline void splay(SplayNode *x)	// zig-zig or zig-zag
	{
		A(x); A(x->parent); // x not root
		A(x->parent->parent); // parent not root
		A(x->parent->parent->parent); // there is superRoot
		SplayNode *p = x->parent;
		SplayNode *g = p->parent;
		if(evertable && p->getSwapFlag())
		{
			if(p->l == x)
			{
				if(g->l == p) rotateR(x), rotateR(x);
				else {A(g->r == p); rotateL(p), rotateL(x);}
			}
			else
			{
				A(p->r == x);
				if(g->l == p) rotateR(p), rotateR(x);
				else {A(g->r == p); rotateL(x), rotateL(x);}
			}
		}
		else
		{
			if(p->l == x)
			{
				if(g->l == p) rotateR(p), rotateR(x);
				else {A(g->r == p); rotateR(x), rotateL(x);}
			}
			else
			{
				A(p->r == x);
				if(g->l == p) rotateL(x), rotateR(x);
				else {A(g->r == p); rotateL(p), rotateL(x);}
			}
		}
	}

	// assumes x's grandparent is root (its parent == NULL)
	template<typename SplayNode>
	static inline void splayLast(SplayNode *x)	// zig-zig or zig-zag
	{
		A(x); A(x->parent); // x not root
		A(x->parent->parent); // parent not root
		SplayNode *p = x->parent;
		SplayNode *g = p->parent;
		if(evertable && p->getSwapFlag())
		{
			if(p->l == x)
			{
				if(g->l == p) rotateR(x), rotateRLast(x);
				else {A(g->r == p); rotateLLast(p), rotateLLast(x);}
			}
			else
			{
				A(p->r == x);
				if(g->l == p) rotateRLast(p), rotateRLast(x);
				else {A(g->r == p); rotateL(x), rotateLLast(x);}
			}
		}
		else
		{
			if(p->l == x)
			{
				if(g->l == p) rotateRLast(p), rotateRLast(x);
				else {A(g->r == p); rotateR(x), rotateLLast(x);}
			}
			else
			{
				A(p->r == x);
				if(g->l == p) rotateL(x), rotateRLast(x);
				else {A(g->r == p); rotateLLast(p), rotateLLast(x);}
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












//////////////////////////// LINK-CUT //////////////////////////////////////////
//
// Implementation (c) 2013 by Adam Blaszkiewicz. All rights reserved.
//
// Tarjan's ST-trees (aka Link-Cut (aka dynamic trees))
// - rootable means that root can be changed to another vertex in the tree
//   rootable implies that underlying splay trees are evertable
// - linTreeAgg - aggregation of linear values over trees
//   sum is ok, max is not ok
//
// PV is a pointer to struct with:
// - PV l,r,parent,pathParent;
// - bool getSwapFlag()
// - void setSwapFlag(bool)
template<typename PV, bool rootable = false, bool linTreeAgg = false>
class LinkCut
{
	typedef Splayer<rootable> S;
public:
	// links a tree rootet at u to node v
	// u must be tree root! (so no link(u,_) was called earlier)
	// assumes there are no cycles
	static inline void linkRoot(const PV& u, const PV& v)
	{
		if(rootable) splayPath(u);
		else splayPathR(u);

		A(u->parent == NULL);
		A(u->pathParent == NULL);
		A(!(rootable && u->getSwapFlag()) || u->r == NULL);
		A( (rootable && u->getSwapFlag()) || u->l == NULL);
		u->pathParent = v;
		if(linTreeAgg)
		{
			access(v);
			v->addVal(u);
		}
	}
	// cuts pv from its parent
	static inline void cut(const PV& pv)
	{
		splayPath(pv);
		A(pv->pathParent || pv->l ||
			(pv->r && rootable && pv->getSwapFlag()));	// cut legal
		if(rootable && pv->getSwapFlag())
		{
			if(pv->r)
			{
				if(linTreeAgg)
				{
					pv->subVal(pv->r);
					if(pv->pathParent) pv->pathParent->subVal(pv);
				}
				pv->r->parent = NULL;
				pv->r->pathParent = pv->pathParent;
				pv->r->setSwapFlag(!pv->r->getSwapFlag());	// xor
				pv->pathParent = NULL;
				pv->r = NULL;
				pv->updateNR();
			}
			else
			{
				A(pv->pathParent);	// cut legal (pv not root)
				if(linTreeAgg)
				{
					pv->pathParent->subVal(pv);
				}
				pv->pathParent = NULL;
			}
		}
		else
		{
			if(pv->l)
			{
				if(linTreeAgg)
				{
					pv->subVal(pv->l);
					if(pv->pathParent) pv->pathParent->subVal(pv);
				}
				pv->l->parent = NULL;
				pv->l->pathParent = pv->pathParent;
				pv->pathParent = NULL;
				pv->l = NULL;
				pv->updateNL();
			}
			else
			{
				A(pv->pathParent);	// cut legal (pv not root)
				if(linTreeAgg)
				{
					pv->pathParent->subVal(pv);
				}
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
			A(pv->l);
			if(linTreeAgg)
			{
				pv->subVal(pv->l);
			}
			pv->l->parent = NULL;
			pv->l->pathParent = NULL;
			pv->l->setSwapFlag(!pv->l->getSwapFlag());
			pv->l = NULL;
			pv->updateNL();
		}
		else
		{
			A(pv->r);
			if(linTreeAgg)
			{
				pv->subVal(pv->r);
			}
			pv->r->parent = NULL;
			pv->r->pathParent = NULL;
			pv->r = NULL;
			pv->updateNR();
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
	// assumes pv is the smallest element in its tree
	static inline void splayPathR(const PV& pv)
	{
		const PV& prevRoot = S::splayRUntilNull(pv);
		A(pv->parent==NULL);
		pv->pathParent = prevRoot->pathParent;
		//prevRoot->pathParent = NULL;
	}
};
//////////////////////////////////////////// link-cut //////////////////////////






const int n = 10;
const int m = 1000;






struct vertex
{
	vertex() : l(0), r(0), parent(0),pathParent(0),swapFlag(0),siz(1) {}
	vertex *l,*r,*parent,*pathParent;

	unsigned char swapFlag;

	inline bool getSwapFlag() const {return swapFlag;}
	inline void setSwapFlag(bool flag) {swapFlag = flag;}

	// empty update functions
	inline void update() {}
	inline void updateLR() {A(l && r);}
	inline void updateL() {A(l);}
	inline void updateR() {A(r);}
	inline void updateNL() {A(!l);}
	inline void updateNR() {A(!r);}
	inline void updateCopy(vertex* x) {A(x);}	// copy stats from x

	// tree size aggregate
	uint siz;
	inline void addVal(vertex* o) {siz += o->siz;}
	inline void subVal(vertex* o) {siz -= o->siz;}
};


vertex vs[100009];


inline void dbgcheck()
{
	rep(i,n)A(Splayer<true>::consistent(&vs[i]));
}

inline void dbgprint(int i)
{
#ifdef DEBUG_BUILD
	cout << "VERTEX " << i+1 <<endl;
	if(vs[i].l) cout << "  L: " << vs[i].l-vs+1;
	if(vs[i].r) cout << "  R: " << vs[i].r-vs+1;
	if(vs[i].l || vs[i].r) cout << endl;

	if(vs[i].parent == NULL && vs[i].pathParent)
		cout << "  pathParent: " << vs[i].pathParent-vs+1 << endl;
	if(vs[i].swapFlag) cout << "  (swapFlag set)" << endl;
	cout<< endl;
#endif
}

inline void dbgprint()
{
	rep(i,n)dbgprint(i);
}

typedef LinkCut<vertex*,true,true> LC;

inline void tc()
{
	rep(i,m)
	{
		fin(30);
		char c=0;
		while(c<'a' || c>'z')c = rchar();

		while(rchar()!=' ');

		D dbgcheck();

		D if(c=='d')	// access u
		{
			int u = ruint();
			--u;
			LC::access(&vs[u]);
		}

		if(c=='s')	// size u
		{
			int u = ruint();
			--u;
			LC::access(&vs[u]);
			W(vs[u].siz);NL
		}
		else if(c=='c')	// conn u v
		{
			fout(10);
			int u=ruint();
			int v=ruint();
			if(u==v)
			{
				W("YES\n");
				continue;
			}
			--u;--v;
			LC::access(&vs[u]);
			vertex* proot = LC::access(&vs[v]).se;
			if(proot == &vs[u])
			{
				W("YES\n");
			}
			else
			{
				W("NO\n");
			}
		}
		else if(c=='a') // add
		{
			int u=ruint();
			int v=ruint();
			--u;
			--v;
			if(u>v)swap(u,v);
			LC::makeRoot(&vs[u]);
			LC::linkRoot(&vs[u],&vs[v]);
		}
		else if(c=='r') // rem
		{
			int u=ruint();
			int v=ruint();
			--u;
			--v;
			if(u>v)swap(u,v);
			LC::makeRoot(&vs[v]);
			LC::cut(&vs[u]);
		}

		D dbgprint();
	}
}






















