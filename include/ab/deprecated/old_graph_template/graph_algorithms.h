#pragma once

#include<queue>


namespace ab
{
	namespace GRAPH_SEARCH_ACTION
	{
		const unsigned int CONTINUE = 0x0;
		const unsigned int END_PATH = 0x1;
		const unsigned int ERASE	= 0x2;
		const unsigned int END_ALGORITHM = 0x10;
	}

	// Pamiêta wartoœæ typu T dla ka¿dego wierzcho³ka
	// TODO: oprócz tej wersji, wersja z CanEraseV == true
	template<typename GRAPH, typename T, false>
	class ExternalVertexData
	{
	public: inline ExternalVertexData(const GRAPH& g) : g(g)
		{
			unsigned int numv = g.getNumV();
			mData = new T[numv];
			memset(mData, 0, sizeof(T*numv);
		}

		public: inline ~ExternalVertexData()
		{
			delete[] mData;
		}

		public: template<typename Iterator> inline T& get(Iterator iterator)
		{
			return mData[g.getVIndex(iterator)];
		}
		public: template<typename Iterator> inline const T& get(Iterator iterator) const
		{
			return mData[g.getVIndex(iterator)];
		}

		private: T *mData;
		private: GRAPH& g;
	};

	template<typename GRAPH, typename T, unsigned int byteOffset>
	class InternalVertexData
	{
		public: inline InternalVertexData(const GRAPH& g) : g(g)
		{
		}

		public: template<typename Iterator> inline T& get(Iterator iterator) const
		{
			return *(T*)((unsigned char *)&g.getV(iterator) + offset);
		}
		public: template<typename Iterator> inline const T& get(Iterator iterator) const
		{
			return *(const T*)((const unsigned char *)&g.getV(iterator) + offset);
		}

		private: GRAPH& g;
	};


	// if Extra is void, struct does not contain it
	template<typename Data, typename Extra>
	struct StructWithExtra
	{
		Data data;
		Extra extra;
	};
	template<typename Data>
	struct StructWithExtra<Data, void>
	{
		Data data;
	};


	// calls fun with an optional extra parameter
	// 1 arg, 1 extra
	template<typename ReturnType, typename Fun, typename Arg1, typename Data, typename Extra>
	inline ReturnType callFunWithExtra(Fun& fun, Arg1& arg1, StructWithExtra<Data,Extra>& s)
	{
		return fun(arg1, extra);
	}
	template<typename ReturnType, typename Fun, typename Arg1, typename Data>	// Extra == void
	inline ReturnType callFunWithExtra<ReturnType,Fun,Arg1,Data,void>(Fun& fun, Arg1& arg1, StructWithExtra<Data,void>& s)
	{
		return fun(arg1);
	}
	template<typename Fun, typename Arg1, typename Data, typename Extra>
	inline void callFunWithExtra<void,Fun,Arg1,Data,Extra>(Fun& fun, Arg1& arg1, StructWithExtra<Data,Extra>& s)
	{
		fun(arg1, extra);
	}
	template<typename Fun, typename Arg1, typename Data>	// Extra == void
	inline void callFunWithExtra<void,Fun,Arg1,Data,void>(Fun& fun, Arg1& arg1, StructWithExtra<Data,void>& s)
	{
		fun(arg1);
	}
	// 3 args, 2 extra
	template<typename ReturnType, typename Fun, typename Arg1, typename Arg2, typename Arg3, typename Data1, typename Extra1, typename Data2, typename Extra2>
	inline ReturnType callFunWithExtra(Fun& fun, Arg1& arg1, Arg2& arg2, Arg3& arg3, StructWithExtra<Data,Extra>& s1, StructWithExtra<Data,Extra>& s2)
	{
		return fun(arg1,arg2,arg3,s1.extra,s2.extra);
	}
	template<typename ReturnType, typename Fun, typename Arg1, typename Arg2, typename Arg3, typename Data1, typename Data2, typename Extra2>	// Extra1 == void
	inline ReturnType callFunWithExtra<ReturnType,Fun,Arg1,Arg2,Arg3,Data1,void,Data2,Extra2>(Fun& fun, Arg1& arg1, Arg2& arg2, Arg3& arg3, StructWithExtra<Data,void>& s1, StructWithExtra<Data,Extra2>& s2)
	{
		return fun(arg1,arg2,arg3,s2.extra);
	}
	template<typename ReturnType, typename Fun, typename Arg1, typename Arg2, typename Arg3, typename Data1, typename Extra1, typename Data2>	// Extra2 == void
	inline ReturnType callFunWithExtra<ReturnType,Fun,Arg1,Arg2,Arg3,Data1,Extra1,Data2,void>(Fun& fun, Arg1& arg1, Arg2& arg2, Arg3& arg3, StructWithExtra<Data,Extra1>& s1, StructWithExtra<Data,void>& s2)
	{
		return fun(arg1,arg2,arg3,s1.extra);
	}
	template<typename ReturnType, typename Fun, typename Arg1, typename Arg2, typename Arg3, typename Data1, typename Data2>	// Extra1 == void, Extra2 == void
	inline ReturnType callFunWithExtra<ReturnType,Fun,Arg1,Arg2,Arg3,Data1,Extra1,Data2,void>(Fun& fun, Arg1& arg1, Arg2& arg2, Arg3& arg3, StructWithExtra<Data,void>& s1, StructWithExtra<Data,void>& s2)
	{
		return fun(arg1,arg2,arg3);
	}


	template<typename GRAPH>
	visitEDefault(GRAPH::REFV& v, GRAPH::VEL::iterator)


// BFS dla grafów nieskierowanych
// zak³adamy, ¿e VisitedVertexData nie wymaga zerowania
template<typename GRAPH, typename VisitVFun, typename VisitEFun, typename NodeExtra = void, bool CanEraseV = false, typename VisitedVertexData = ExternalVertexData<GRAPH,unsigned char,CanEraseV>, typename IteratorFrom >
void BFS(GRAPH& g, IteratorFrom _from, VisitVFun& visitVFun, VisitEFun& visitEFun, typename ENABLE_IF<!GRAPH::Directed,GRAPH*>::RET* dummy = 0)
{
	VisitedVertexData visited(g);

	std::queue<StructWithExtra<GRAPH::PV,NodeExtra> > S;
	S.push(StructWithExtra(g.getPV(_from)));
	visited.get(_from) = 1;

	while(!S.empty())
	{
		GRAPH::REFV v = g.getV(S.front().data);
		//S.pop();

		callFunWithExtra(visitVFun, v, S.front());

		for(typename GRAPH::VEL::iterator itr = v.to.begin(); itr!=v.to.end(); )
		{
			if(!g.validV(itr->pDest))	// Subcontainer nie zawiera tego elementu
			{
				++itr;
				continue;
			}

			PV& destV = getPV(itr->pDest);
			unsigned char& destVisited = visited.get(destV);
			StructWithExtra<GRAPH::PV,NodeExtra> newNode;
			newNode.data = destV;

			unsigned int r = callFunWithExtra(visitEFun,v,itr,destVisited,S.front(), newNode);
			if(r == GRAPH_SEARCH_ACTION::CONTINUE)
			{
				// jeszcze nie odwiedzony
				S.push(newNode);
				destVisited = 1;

				++itr;
				continue;
			}
			else if(r == GRAPH_SEARCH_ACTION::END_PATH)
			{
				++itr;
				continue;
			}
			else if(r == GRAPH_SEARCH_ACTION::ERASE | GRAPH_SEARCH_ACTION::END_PATH)
			{
				g.eraseEdge(v.to, itr);
				continue;
			}
			else // if(r == GRAPH_SEARCH_ACTION::ERASE | GRAPH_SEARCH_ACTION::CONTINUE)
			{
				g.eraseEdge(v.to, itr);
				S.push(newNode);
				destVisited = 1;
				continue;
			}
			
		}
		S.pop();
	}
}










inline void removeCycles(unsigned int from)
{
	removeCycles<void>(from);		// COMPILATION ERROR means that graph doesn't support EErasable,
									//   please set proper template values when instantiating the graph
}
//private: template<typename T>
//void removeCycles(unsigned int from, typename ENABLE_IF<!EErasable,T>::RET* dummy = 0)
//{
//	printf("Cannot remove cycles - !EErasable\n");
//	ASSERT(0);
//}
// for directed graphs
template<typename T>
void removeCycles(unsigned int from, typename ENABLE_IF<EErasable && Directed,T>::RET* dummy = 0)
{
	// jeœli nie usuwamy ¿adnych wierzcho³ków, to nie zostan¹ przemianowane i mo¿emy odwo³ywaæ siê do nich za pomoc¹ indeksów
	unsigned char* visited = new unsigned char[mVertices.size()];
	memset(visited, 0, mVertices.size()*sizeof(unsigned char));

					// current, from, next neigh
	Vector<Tuple2<unsigned int, unsigned int> > S;
	S.push_back(MakeTuple(from,0u));
	visited[from] = 1;
	while(!S.empty())
	{
		printf("%d\n",S.back().v1);
		Tuple2<unsigned int, unsigned int>& cs = S.back();
		V& v = *mVertices[cs.v1];

		for(;;)
		{
			if(cs.v2 == v.to.size())
			{
				visited[cs.v1] = 2;	// rozdeptany
				S.pop_back();
				break;
			}
			if(!validV(cs.v1))	// Subcontainer nie zawiera tego elementu
			{
				++cs.v2;
				continue;
			}
			VELNODE& next = v.to[cs.v2];
			const V& destV = getV(next.pDest);

			unsigned int index = getVIndex(next.pDest);

			if(visited[index]==2)	// nic tu po nas
			{
				++cs.v2;
				continue;
			}
			else if(visited[index]==1)	// juz odwiedzony -> usuwamy krawedz
			{
				/*for(unsigned int i=0; i<mVertices.size(); ++i)
				{
					printf("---vertex %d\n",i);
					for(unsigned int j=0; j<mVertices[i]->to.size(); ++j)
					{
						printf("%d  %d\n",mVertices[i]->to[j].pContOtherEnd, mVertices[i]->to[j].pOtherEnd);
					}
				}*/

				eraseEdge(v.to, cs.v2);	// itr bedzie teraz wskazywal na zast¹piony element, przeszukujemy od niego dalej        TODO
					
				//++cs.v2;
				continue;
			}
			else //if(!visited[index])
			{
				++cs.v2;
				visited[index] = 1;
				S.push_back(MakeTuple(index, 0u));
				break;
			}
		}
		// tutaj albo mamy nowe cos na stosie, albo sie skonczylo cos
	}
}
// --- for not directed graphs:
template<typename T>
void removeCycles(unsigned int from, typename ENABLE_IF<EErasable && !Directed,T*>::RET* dummy = 0)
{
	// jeœli nie usuwamy ¿adnych wierzcho³ków, to nie zostan¹ przemianowane i mo¿emy odwo³ywaæ siê do nich za pomoc¹ indeksów
	unsigned char* visited = new unsigned char[mVertices.size()];
	memset(visited, 0, mVertices.size()*sizeof(unsigned char));

	Vector<unsigned int> S;
	S.push_back(from);
	visited[from] = 1;
	while(!S.empty())
	{
		V& v = *mVertices[S.back()];
		S.pop_back();

		typename VEL::iterator end = v.to.end();
		for(typename VEL::iterator itr = v.to.begin(); itr!=end; )
		{
			if(!validV(itr->pDest))	// Subcontainer nie zawiera tego elementu
			{
				++itr;
				continue;
			}
			V& destV = getV(itr->pDest);

			unsigned int index = getVIndex(itr->pDest);

			if(visited[index])	// juz odwiedzony -> usuwamy krawedz
			{
				/*for(unsigned int i=0; i<mVertices.size(); ++i)
				{
					printf("---vertex %d\n",i);
					for(unsigned int j=0; j<mVertices[i]->to.size(); ++j)
					{
						printf("%d  %d\n",mVertices[i]->to[j].pContOtherEnd, mVertices[i]->to[j].pOtherEnd);
					}
				}*/

				eraseEdge(v.to, itr);	// itr bedzie teraz wskazywal na zast¹piony element, przeszukujemy od niego dalej        TODO
					

				end = v.to.end();
				continue;
			}

			// jeszcze nie odwiedzony
			S.push_back(index);
			visited[index] = 1;

			++itr;
		}
	}
}



}	// namespace ab;