#pragma once

#include<atablash/union-find.hpp>

namespace ab
{
	// MST
	// O(m log n)
	//
	// minNumComponents is optional, for optimization; setting lower than actual number of components is ok
	//
	// zeroResult=false wydaje sie nic nie przyspieszac
	//
	// sorted=true znacznie przyspiesza. mo¿na u¿yæ counting sorta wczesniej
	//
	#ifdef OLD_COMPILER
	template<typename G, typename EWeight, typename EResult, bool sorted = false, bool zeroResult=true, int minNumComponents = 1>
	#else
	template<bool sorted = false, bool zeroResult=true, int minNumComponents = 1, typename G, typename EWeight, typename EResult>
	#endif
	int kruskal(G& g, const EWeight& eWeight, const EResult& eResult)
	{
		unsigned int numEdges = g.es.size();
		unsigned int numVerts = g.vs.size();

		if(!sorted)std::sort(g.es.begin(),g.es.end(),[eWeight](const typename G::EExtra& a, const typename G::EExtra& b){return a.*eWeight < b.*eWeight;});

		UnionFind uf(numVerts);
		int numFound = 0;

		int i=0;
		for(; i<numEdges && numFound < numVerts-minNumComponents; ++i)
		{
			typename G::PV a = g.es[i].fr();
			typename G::PV b = g.es[i].to();
			int aa = uf.find(a-g.vs.begin());
			int bb = uf.find(b-g.vs.begin());
			if(aa==bb){
				if(zeroResult)g.es[i]->*eResult = false;
				continue;
			}

			uf.unio(aa,bb);
			++numFound;
			g.es[i]->*eResult = true;
		}
		if(zeroResult) for(;i<numEdges;++i) g.es[i]->*eResult = false;

		return numFound;
	}

	#ifdef OLD_COMPILER
	template<typename G, typename EWeight, bool sorted = false, unsigned int minNumComponents = 1>
	#else
	template<bool sorted = false, unsigned int minNumComponents = 1, typename G, typename EWeight>
	#endif
	std::vector<typename G::PE> kruskal(G& g, const EWeight& eWeight)
	{
		unsigned int numEdges = g.es.size();
		unsigned int numVerts = g.vs.size();

		if(!sorted)std::sort(g.es.begin(),g.es.end(),[eWeight](const typename G::EExtra& a, const typename G::EExtra& b){return a.*eWeight < b.*eWeight;});

		UnionFind uf(numVerts);
		std::vector<typename G::PE> result;
		result.reserve(numVerts-minNumComponents);

		for(int i=0; i<numEdges && result.size() < numVerts-minNumComponents; ++i)
		{
			typename G::PV a = g.es[i].fr();
			typename G::PV b = g.es[i].to();
			int aa = uf.find(a-g.vs.begin());
			int bb = uf.find(b-g.vs.begin());
			if(aa==bb)continue;

			uf.unio(aa,bb);
			result.push_back(g.es[i]);
		}

		return result;
	}


	/*
	template<typename Weight>
	struct _boruvkaCleanupEdge
	{
		int from;
		int to;
		Weight val;
		int orig;

		static int getFrom(const _boruvkaCleanupEdge& e)
		{
			return e.from;
		}
		static int getTo(const _boruvkaCleanupEdge& e)
		{
			return e.to;
		}
	};
	// MST
	// sets resultEDataKey to true for edges in MST, does not set resultEDataKey for other edges
	// O(min(m log n, n^2))
	// O(n) if planar
	// set cleanupfirst = false if there are no multiple (u,v) edges
	// Old paper by Cheriton, ...
	template<bool cleanupFirst = true, typename G, typename EWeight>
	std::vector<G::PE> boruvkaCleanup(G& g, const EWeight& eWeight)
	{
		int numTrees = g.vs.size();

		std::vector<_boruvkaCleanupEdge<Weight>> edges(g.edges.size());
		for(unsigned int i=0; i<g.edges.size(); ++i)
		{
			edges[i].from = g.edges[i].from;
			edges[i].to   = g.edges[i].to;
			edges[i].val  = getWeightEData(g.edges[i]);
			edges[i].orig = i;
		}

		std::pair<int,Weight> *bests = new std::pair<int,Weight>[numTrees];
		int *vmap = new int[numTrees];

		int numFound = 0;

		for(;;)
		{
			// cleanup
			if(numFound != 0 || cleanupFirst)
			{
				std::vector<_boruvkaCleanupEdge<Weight> > helperEdges(edges.size());
				countingSort(&edges[0], &edges[0] + edges.size(), &helperEdges[0], numTrees, _boruvkaCleanupEdge<Weight>::getTo);
				countingSort(&helperEdges[0], &helperEdges[0] + edges.size(), &edges[0], numTrees, _boruvkaCleanupEdge<Weight>::getFrom);
				int curDest = 0;
				int prev1 = -1;
				int prev2 = -1;
				Weight bestval = 0;
				int bestarg = 0;
				//printf("edges size %d\n", edges.size());
				for(unsigned int i=0; i<edges.size(); ++i)
				{
					int n1 = edges[i].from;
					int n2 = edges[i].to;

					if(n1 == n2) continue;

					if(prev1 == n1 && prev2 == n2)
					{
						if(edges[i].val < bestval) bestval = edges[i].val, bestarg = i;
					}
					else
					{
						if(prev1 != -1) edges[curDest++] = edges[bestarg];
						bestval = edges[i].val;
						bestarg = i;
						prev1 = n1;
						prev2 = n2;
					}
				}
				if(prev1 != -1) edges[curDest++] = edges[bestarg];
				edges.resize(curDest);
			}
			//cleanup end

			//printf("Cheriton step: %d trees, %d edges, found %d\n",numTrees,edges.size(),numFound);

			// find bests
			memset(bests, 0xff, sizeof(std::pair<int,Weight>)*numTrees);
			for(unsigned int i=0; i<edges.size(); ++i)
			{
				//printf("%x\n",bests[a].first);
				int a = edges[i].from;
				if((unsigned int)bests[a].first == 0xffffffff || bests[a].second > edges[i].val)
				{
					bests[a].second = edges[i].val;
					bests[a].first = i;
				}

				int b = edges[i].to;
				if((unsigned int)bests[b].first == 0xffffffff || bests[b].second > edges[i].val)
				{
					bests[b].second = edges[i].val;
					bests[b].first = i;
				}
			}
			// find bests end

			UnionFind uf(numTrees);
			bool change = false;
			for(int i=0; i<numTrees; ++i)
			{
				int best = bests[i].first;
				if((unsigned int)best == 0xffffffff) continue;

				int a = edges[best].from;
				int b = edges[best].to;
				a = uf.find(a);
				b = uf.find(b);
				if(a==b)continue;

				uf.makeUnion(a,b);
				getResultEData(g.edges[edges[best].orig]) = true;
				++numFound;
				change = true;
			}

			if(!change) break;

			// przemianowanie
			int newNumTrees = 0;
			memset(vmap, 0xff, sizeof(int)*numTrees);
			for(int i=0; i<numTrees; ++i)
			{
				int a = uf.find(i);
				if((unsigned int)vmap[a] == 0xffffffff) vmap[a] = newNumTrees++;
				vmap[i] = vmap[a];
			}
			for(unsigned int i=0; i<edges.size(); ++i)
			{
				edges[i].from = vmap[edges[i].from];
				edges[i].to   = vmap[edges[i].to];
			}
			numTrees = newNumTrees;
			// przemianowanie end
		}

		delete[] bests;
		delete[] vmap;
		return numFound;
	}
	*/
}
