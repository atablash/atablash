#pragma once

#include<atablash/types.hpp>
#include<atablash/static-array.hpp>

namespace ab{


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// GRAFY ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////
// BINDER
template<typename Ret, typename T>
class Binder
{
public:
	Binder(Ret T::*_var) : var(_var) {}
	Ret& operator()(T& obj) const
	{
		return obj.*var;
	}
	const Ret& operator()(const T& obj) const
	{
		return obj.*var;
	}
private:
	Ret T::*var;
};
template<typename Ret, typename T>
Binder<Ret,T> bind(Ret T::*var)
{
	return Binder<Ret,T>(var);
}
////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////
// KLASY GRAFOW //
// * wskaźniki (PE, PV) i iteratory nie mogą dużo zajmować,
//   bo mogą być pamiętane przez np. Dinica
// * Dlatego są to prawie (całkiem?) gołe wskaźniki, które nie zawsze pozwalają
//   na wykonanie żądanej operacji na tym, na co wskazują, np. usuwania krawędzi
// * Po to są klasy VC i EC (context), które pozwalają wykonywać
//   wszystkie dostępne operacje
/*

template<uint _NMax = 0, uint _MMax = 0, typename _VExtra = VOID, typename _EExtra = VOID>
class G
{
public:
	static const uint NMax = _NMax;
	static const uint MMax = _MMax;
	typename _VExtra VExtra;
	typename _EExtra EExtra;

	struct VC//ontext
	{
		struct TO<E>
		{
			struct iterator : public PE
			{
				// ...
			};
			iterator begin();
			iterator end();

			EContext operator[](PE&);
		} to;
	};
	}

	struct EC//ontext
	{
		EExtra& operator->()
		VContext fr();
		VContext to();
		EContext rev() const;		// rev must be enabled
		bool revEnabled() const;
		void erase();
		void disable();				// EC becomes invalid
		void enableRev();			// rev must be disabled
	}


	struct PE;

	struct VS<V>
	{
		struct iterator : public PV
		{
			// ...
		};
		iterator begin();
		iterator end();

		VContext operator[](PV&);
	} vs;

	struct ES<E>
	{
		struct iterator : public PE
		{
			// ...
		};
		iterator begin();
		iterator end();

		EContext operator[](PE&);
	} es;

	struct V : public VExtra
	{
		V(VContext);
	};
	struct E : public EExtra
	{
		E(EContext);
	};
};
*/
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
// 2014-05-12
//              Vs			Es
// Iterate		no			yes
// Erase		no			no
// point		int			pointer
// rev						no
//
// Avoid vs reallocation! (?)
//
// Good for: Kruskal
//
// dummy vertices
//
template<typename VExtra=Void, typename EExtra=Void, uint NMax=0, uint MMax=0> class GraphU_EI{};
template<typename _EExtra, uint _NMax, uint _MMax>
class GraphU_EI<Void,_EExtra,_NMax,_MMax>	// edges iterable
{
private:
	class V; class E;

public:
	static const uint NMax = _NMax;
	static const uint MMax = _MMax;
	typedef Void VExtra;
	typedef _EExtra EExtra;
	typedef uint PV; typedef E* PE;

private:
	class V : public VExtra{
	};
	class E : public EExtra{
		friend class GraphU_EI;
		E(const PV& _fr, const PV& _to, const EExtra& eExtra) : EExtra(eExtra), fr(_fr), to(_to) {}
		PV fr,to;
	};

public:
	class VC; class EC; class VS; class ES;

	class VC{
	public:
		VC(GraphU_EI&_g, const PV& _v) : g(_g), v(_v){}
		operator PV()const{return v;}
	private:
		GraphU_EI& g;
		PV v;
	};
	class EC{
	public:
		EExtra* operator->(){return e;}
		template<class T> decltype(PE()->*T()) operator->*(const T& t){return e->*t;}
		operator PE()const{return e;}
		VC fr(){return VC(g,e->fr);}
		VC to(){return VC(g,e->to);}
		void fr(const PV& _fr){e->fr=_fr;}
		void to(const PV& _to){e->to=_to;}
		EC(GraphU_EI&_g, const PE& _e) : g(_g), e(_e) {}
	private:
		GraphU_EI& g;
		PE e;
	};

	class VS{
		friend class GraphU_EI;
	public:
		typedef uint iterator;
		uint size()const{return g.vsSize;}
		void resize(uint _size){g.vsSize=_size;}
		iterator begin()const{return 0;}
		iterator end()const{return g.vsSize;}
	private:
		GraphU_EI& g;
		VS(GraphU_EI& _g) : g(_g) {}
	} vs;

	class ES{
		friend class GraphU_EI;
	public:
		typedef PE iterator;
		inline iterator begin() {return &*g._es.begin();}
		inline iterator end()   {return &*g._es.end();}

		inline uint size()const{return g._es.size();}
		inline void resize(unsigned int _size) {g._es.resize(_size);}
		inline void reserve(unsigned int _size) {g._es.reserve(_size);}

		inline void push_back(const PV& from, const PV& to, const EExtra& eExtra = EExtra()){
			g._es.push_back(E(from,to,eExtra));
		}

		inline EC operator[](const PE& pe) {return EC(g,pe);}
		inline EC operator[](unsigned int idx){
			ASS(MMax==0 || idx < MMax);
			return EC(g,&g._es[idx]);
		}
	private:
		GraphU_EI& g;
		ES(GraphU_EI& _g) : g(_g) {}
	} es;

	GraphU_EI() : vs(*this), es(*this), vsSize(0) {}

private:
	typedef typename _vector<E,NMax>::R _ES;
	_ES _es;
	uint vsSize;
};




////////////////////////////////////////////////////////////////////////////////
// 2013-11-15
//              Vs			Es
// Iterate		yes			no
// Erase		no			yes
// point		pointer		pointer
// rev						yes
//
// Avoid vs reallocation!
// Es paired or single
//
// Good for: Dinic
//
// use BaseG::VExtra
// otherwise use G
template<typename _VExtra = Void, typename _EExtra = Void, uint _NMax = 0, uint _MMax = 0>
class BaseGraphU_ERD
{
public:
	static const uint NMax = _NMax;
	static const uint MMax = _MMax;
	typedef _VExtra VExtra;
	typedef _EExtra EExtra;

private:
	class V; class E;

public:
	typedef V* PV;
	typedef E* PE;

	typedef typename _vector<V,NMax>::R _VS;
public:
	inline BaseGraphU_ERD(_VS& __vs) : _vs(__vs), vs(*this) {}
	_VS& _vs;
private:
	struct E : public EExtra
	{
		inline E(const PV& _to, const PE& _rev, const EExtra& ee = EExtra()) :
			EExtra(ee), to(_to), rev(_rev) {}
		inline E(const E& o) : EExtra(o), to(o.to), rev(o.rev)
		{
			if(rev) rev->rev = this;
		}
		inline E& operator=(const E& o)
		{
			EExtra::operator=(o);
			to = o.to;
			rev = o.rev;
			if(rev) rev->rev = this;
			return *this;
		}
		PV to;
		PE rev;
	};

	class V : public VExtra
	{
		friend class BaseGraphU_ERD;
	private:
		std::vector<E> to;
	};

	//_VS& _vs;


public:
	class VC; class EC; class VS;

	class VC//ontext
	{
		friend class VS;
		friend class EC;
	public:
		struct TO
		{
			friend class BaseGraphU_ERD;
		private:
			BaseGraphU_ERD& g;
			PV pv;
			inline TO(BaseGraphU_ERD& _g, PV& _pv) : g(_g), pv(_pv) {}
			inline void push_back(const E& e)
			{
				pv->to.push_back(e);
			}
		public:
			typedef PE iterator;
			inline iterator begin() {return &*pv->to.begin();}
			inline iterator end()   {return &*pv->to.end();}
			inline void reserve(unsigned int size) {pv->to.reserve(size);}
			inline E& back() {return pv->to.back();}
			inline const E& back() const {return pv->to.back();}
			inline E& front() {return pv->to.front();}
			inline const E& front() const {return pv->to.front();}

			inline EC operator[](PE& pe)
			{
				return EC(g,pe,pv);
			}
		} to;

		inline V& operator*() {return *to.pv;}
		inline const V& operator*() const {return *to.pv;}
		inline V* operator->() {return to.pv;}
		inline const V* operator->() const {return to.pv;}
		inline operator PV() const {return to.pv;}
	private:
		inline VC(BaseGraphU_ERD& _g, PV _pv) : to(_g,_pv) {}
	};


	class EC//ontext
	{
		friend struct VC::TO;
	public:
		inline VC to() const {return VC(g,pe->to);}
		inline EC rev() const {return EC(g,pe->rev,pe->to);}		// rev must be enabled
		inline bool revEnabled() const {return pe->rev != 0;}
		inline void delSingle()
		{
			A(!pe->rev);	// single
			g._delE(pvfrom,pe);
		}
		inline void delPaired()
		{
			g._delE(pe->to, pe->rev);
			g._delE(pvfrom, pe);
		}
		inline void del()	// !!! SLOWER !!!
		{
			if(pe->rev) _delE(pe->to, pe->rev);
			g._delE(pvfrom, pe);
		}
		inline void disable()	// EC becomes invalid, rev must be enabled
		{
			PE reve = pe->rev;
			g._delE(pvfrom, pe);
			reve->rev = 0;
		}
		inline void enableRev()	// rev must be disabled
		{
			A(pe->rev==0);
			pe->rev = &*pe->to->to.begin() + pe->to->to.size();
			pe->to->to.push_back(E(pvfrom,pe));
		}

		inline operator PE() const {return pe;}
	private:
		inline EC(BaseGraphU_ERD& _g, const PE& _pe, const PV& _pvfrom) :
			g(_g), pe(_pe), pvfrom(_pvfrom) {}
		BaseGraphU_ERD& g;
		PE pe;
		PV pvfrom;
	};

	class VS
	{
		friend class BaseGraphU_ERD;
	public:
		typedef PV iterator;
		inline iterator begin() {return &*g._vs.begin();}
		inline iterator end()   {return &*g._vs.end();}

		inline void resize(unsigned int size) {g._vs.resize(size);}

		inline VC operator[](const PV& pv) {return VC(g,pv);}
		inline VC operator[](unsigned int idx)
		{
			ASS(NMax==0 || idx < NMax);
			return VC(g,&g._vs[idx]);
		}
	private:
		BaseGraphU_ERD& g;
		VS(BaseGraphU_ERD& _g) : g(_g) {}
	} vs;

	inline void addSingle(PV vot, PV vto, EExtra extra = EExtra())
	{
		vot->to.push_back(E(vto,0,extra));
	}

	inline void addPaired(const PV& vot, const PV& vto, const EExtra& extra1 = EExtra(), const EExtra& extra2 = EExtra())
	{
		vot->to.push_back(E(vto,0,extra1));
		vto->to.push_back(E(vot,0,extra2));
		vot->to.back().rev = &*vto->to.begin()+vto->to.size()-1;
		vto->to.back().rev = &*vot->to.begin()+vot->to.size()-1;
		/*
		vs[vot].to.push_back();
		E& e1 = vs[vot].to.back();
		vs[vto].to.push_back();
		E& e2 = vs[vto].to.back();
		e1.~E();	// avoid copy constructors
		new(&e1) E(vto, &e2, extra1);
		e2.~E();
		new(&e2) E(vot, &e1, extra2);*/
	}
private:
	inline void _delE(const PV& pvfrom, const PE& pe)
	{
		*pe = pvfrom->to.back();
		pvfrom->to.pop_back();
	}
};
template<typename _VExtra = Void, typename _EExtra = Void, uint _NMax = 0, uint _MMax = 0>
class GraphU_ERD	// edges removable, disableable
	: public BaseGraphU_ERD<_VExtra,_EExtra,_NMax,_MMax>//raphU_ERD
{
public:
	typedef BaseGraphU_ERD<_VExtra,_EExtra,_NMax,_MMax> BASE;
	inline GraphU_ERD() : BASE(__vs) {}
	typename BASE::_VS __vs;
};
////////////////////////////////////////////////////////////////////////////////





}
