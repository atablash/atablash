#pragma once

#include<atablash/common.hpp>
#include<atablash/static-queue.hpp>

namespace ab{

////////////////////////////////////////////////////////////////////////////////
// 2013-11-15
////// DINIC - DFS ON ALL EDGES ////// EDGES DISABLABLE
//
// Implementation (c) 2013 by Adam Błaszkiewicz. All rights reserved.
//
// modifies original graph by enabling/disabling edges and changing edge capacity
// DFS runs on all edges, not only layer edges found by BFS
// Tested on: FASTFLOW (0.32, 0.34, 0.39) with vs[i].to.reserve(15)
// If performance is critical, use DynamicArray instead of vector for vs[i].to
// assumes no loops v->v
//
// TODO: sprawdzić, czy działa po zmianach
//
namespace Dinic
{
	template<class G> struct EExtra{};
	template<class G> struct VExtra{
		int level;
		typename G::VC::TO::iterator lasti;
	};
template<typename G, typename GetECap>
struct _Dinic
{
private:
	typedef typename G::PV PV;
	typedef typename G::PE PE;

	typedef typename G::VC VC;
	typedef typename G::EC EC;

	typename G::VS& vs;

	const GetECap& getECap;
	PV t;
public:
	_Dinic(G& g, const GetECap& _getECap) : vs(g.vs), getECap(_getECap) {}

	ull run(const PV& _ps, const PV& _t){
		t = _t;

		VC s = vs[_ps];

		FE(e,s.to) s.to[e].rev().disable();

		ull maxflow = 0;
		while(bfs(s)){
			FE(v,vs) vs[v]->lasti = vs[v].to.begin();
			FE(e,s.to){
				uint nflow = dfs(e.to(), e->*getECap);
				e->*getECap -= nflow;
				if(e->*getECap == 0) e.delSingle(), --e; // ok?
				maxflow += nflow;
			}
		}
		return maxflow;
	}

private:

	bool bfs(const PV& s)
	{
		FE(pv,vs) vs[pv]->level = 0;
		typename _queue<PV,G::NMax>::R q;
		q.push(s);
		s->level = 1;
		while(!q.empty())
		{
			const PV& pv = q.front();	VC v = vs[pv];
			//cout<<"end-begin "<<v.to.end()-v.to.begin()<<" beg "<<v.to.begin()<<endl;
			FE(e,v.to)
			{
				VC destv = v.to[e].to(); //cout<<"k "<<destv->level<<endl;
				if(destv->level) continue;
				destv->level = v->level + 1;
				if(destv == t) return true;
				q.push(destv);
			}
			q.pop();
		}
		return false;
	}

	uint dfs(const PV& pv, uint _flow)
	{
		//cout<<"dfs at "<<pv-vs[0u]<<" "<<t-vs[0u]<<" "<<_flow<<endl;
		if(pv==t) return _flow;
		uint flow = _flow;
		typename VC::TO::iterator pe = pv->lasti;
		VC v = vs[pv];
		for(; pe != v.to.end(); ++pe)
		{
			EC e = v.to[pe];
			VC destv = e.to();
			if(destv->level != v->level + 1) continue;

			uint nflow = dfs(destv, min(flow, e->*getECap));
			//cout<<"Flowed "<<nflow<<"/"<<"min("<<flow<<", "<<e->*getECap<<")"<<endl;
			if(nflow==0)continue;

			flow -= nflow;
			if(!e.revEnabled()) e.enableRev();
			e.rev()->*getECap += nflow;
			e->*getECap -= nflow;				A(e->*getECap >= 0);
			if(e->*getECap == 0)
			{
				e.disable();
				if(flow>0)--pe;
			}
			if(flow==0)break;
		}
		pv->lasti = pe;
		return _flow-flow;
	}
};
}
template<typename G, typename GetECap>
Dinic::_Dinic<G,GetECap> dinic(G& g, const GetECap& getECap)
{return Dinic::_Dinic<G,GetECap>(g,getECap);}
////////////////////////////////////////////////////////////////////////////////

}

