#pragma once

#include<atablash/templates.h>
#include<atablash/unordered_smart_vector.h>
#include<atablash/unordered_vector.h>
#include<atablash/subcontainer.h>
#include<atablash/p.h>
#include<atablash/vector.h>
#include<atablash/tuple.h>

#include<cstring>

namespace ab
{



// ----------------------------------- VERTEX --------------------------------------- //

// Zwyk³y vertex z list¹ wskaŸników na inne vertexy
template<typename GRAPH_INFO>
struct VERTEX
{
	public: VERTEX(){}
	private: VERTEX(const VERTEX& o) {}
	private: VERTEX& operator=(const VERTEX&) {}

	public:	typename GRAPH_INFO::VEL to;
};



// ----------------------------------- EDGE --------------------------------------- //

template<typename GRAPH_INFO, typename EExtra, bool EErasable, bool Directed>
struct EDGE
{
	inline void erase() {}
	EExtra extra;
};

template<typename GRAPH_INFO, bool EErasable, bool Directed>
struct EDGE<GRAPH_INFO, void, EErasable, Directed>
{
	inline void erase() {}
};

// e-erasable, directed
template<typename GRAPH_INFO, typename EExtra>
struct EDGE<GRAPH_INFO, EExtra, true, true>
{
	EExtra extra;

	inline void erase()
	{
		pContEnd1->erase(pEnd1);
	}

	typename GRAPH_INFO::VEL* pContEnd1;
	typename GRAPH_INFO::PVELNODE pEnd1;

};

// e-erasable, not directed
template<typename GRAPH_INFO, typename EExtra>
struct EDGE<GRAPH_INFO, EExtra, true, false>
{
	EExtra extra;

	inline void erase()
	{
		pContEnd1->erase(pEnd1);
		pContEnd2->erase(pEnd2);
	}

	typename GRAPH_INFO::VEL* pContEnd1;
	typename GRAPH_INFO::PVELNODE pEnd1;

	typename GRAPH_INFO::VEL* pContEnd2;
	typename GRAPH_INFO::PVELNODE pEnd2;

};

// e-erasable, directed
template<typename GRAPH_INFO>
struct EDGE<GRAPH_INFO, void, true, true>
{
	inline void erase()
	{
		pContEnd1->erase(pEnd1);
	}

	typename GRAPH_INFO::VEL* pContEnd1;
	typename GRAPH_INFO::PVELNODE pEnd1;

};

// e-erasable, not directed
template<typename GRAPH_INFO>
struct EDGE<GRAPH_INFO, void, true, false>
{
	inline void erase()
	{
		pContEnd1->erase(pEnd1);
		pContEnd2->erase(pEnd2);
	}

	typename GRAPH_INFO::VEL* pContEnd1;
	typename GRAPH_INFO::PVELNODE pEnd1;

	typename GRAPH_INFO::VEL* pContEnd2;
	typename GRAPH_INFO::PVELNODE pEnd2;
};












// ----------------------------------- Graph --------------------------------------- //

// public types:
// V
// E
// PV
// PE
// REFV - V& if vertices are stored internally or V if they are created on demand
//
// public funtions:
//
// unsigned int getNumV() const;	// if exists, also following may be needed:
//         unsigned int getVIndex(iterator) const;
//                   V& getV(iterator);
//             const V& getV(iterator) const;
//
// PV getPV(iterator);
//


// Graph with adjacency lists
// undirected graphs only use vertex.to, never vertex.from
template<bool _VErasable, bool _EErasable, bool _Directed = false, bool _EIterable = false, typename _EExtra = void, typename VParentContainer = void, typename VPredicate = void>
class Graph
{
	
	// TYPES INFO ----------

	typedef VERTEX<Graph> V;
	typedef EDGE<Graph, _EExtra, _EErasable, _Directed> E;

	public: typedef typename IF<SAME_TYPE<VParentContainer, void>::RET,
					typename IF<_VErasable, UnorderedSmartVector<p<V>, true >, Vector<p<V> > >::RET,
					Subcontainer<VParentContainer,VPredicate> >::RET VCONTAINER;

	public: typedef typename IF<_EErasable, UnorderedSmartVector<E>, Vector<E> >::RET ECONTAINER;


	static const bool VErasable = _VErasable;
	static const bool EErasable = _EErasable;
	static const bool EIterable = _EIterable;
	static const bool Directed  = _Directed;

	typedef _EExtra EExtra;


	private: struct VELNode;
	private: struct VELNodeWithPOtherEnd;
	private: struct VELNodeWithPEdge;
	
	public: typedef typename IF<_VErasable, typename UnorderedSmartVector<p<V>,true>::magic_pointer, unsigned int>::RET PV;



	typedef typename IF< EIterable,                                VELNodeWithPEdge,
			typename IF<!EIterable &&  EErasable &&  Directed,              VELNode,
			typename IF<!EIterable &&  EErasable && !Directed, VELNodeWithPOtherEnd,
		/*	typename IF<!EIterable && !EErasable, */                        VELNode /*>::RET*/ >::RET >::RET >::RET VELNODE;


						// mamy globaln¹  listê krawêdzi i krawêdzie s¹ usuwalne
	typedef typename IF< EIterable &&  EErasable,		       typename UnorderedSmartVector<VELNODE>::magic_pointer,
						// mamy globaln¹  listê krawêdzi i krawêdzie nie s¹ usuwalne
			typename IF< EIterable && !EErasable,                       unsigned int,
						// brak globalnej listy krawêdzi i krawêdzie s¹ jednokierunkowe, usuwalne
			typename IF<!EIterable &&  EErasable &&  Directed,      unsigned int,
						// brak globalnej listy krawêdzi i krawêdzie s¹ dwukierunkowe,   usuwalne
			typename IF<!EIterable &&  EErasable && !Directed, typename UnorderedSmartVector<VELNODE>::magic_pointer,
						// brak globalnej listy krawêdzi i krawêdzie s¹ nieusuwalne
						/* !EIterable && !EErasable */                     unsigned int >
						::RET >::RET >::RET >::RET PVELNODE;

	
	typedef V& REFV;

	

	

	


	


	

						// mamy globaln¹  listê krawêdzi i krawêdzie s¹ usuwalne
	public: typedef typename IF< EIterable &&  EErasable,		       UnorderedSmartVector<VELNODE >,
						// mamy globaln¹  listê krawêdzi i krawêdzie nie s¹ usuwalne
					typename IF< EIterable && !EErasable,                       Vector<VELNODE >,
						// brak globalnej listy krawêdzi i krawêdzie s¹ jednokierunkowe, usuwalne
					typename IF<!EIterable &&  EErasable &&  Directed,      UnorderedVector<VELNODE >,
						// brak globalnej listy krawêdzi i krawêdzie s¹ dwukierunkowe,   usuwalne
					typename IF<!EIterable &&  EErasable && !Directed, UnorderedSmartVector<VELNODE >,
						// brak globalnej listy krawêdzi i krawêdzie s¹ nieusuwalne
						/* !EIterable && !EErasable */                     Vector<VELNODE > >
						::RET >::RET >::RET >::RET VEL;

			
	
	public: typedef typename IF<_EErasable, typename UnorderedSmartVector<E>::magic_pointer, unsigned int>::RET PE;

	// pamiêta tylko wskaŸnik do wierzcho³ka docelowego
	private: struct VELNode
	{
		typename Graph::PV pDest;

		//typedef unsigned int PThisEndType;
		//typedef void*		 POtherEndType;
	};

	// EErasable = true
	// pamiêta dodatkowo wskaŸnik na drugi koniec krawêdzi;
	// zak³adamy, ¿e potrzeba takiej struktury wynika z potrzeby usuwania krawêdzi
	private: struct VELNodeWithPOtherEnd
	{
		typename Graph::PV pDest;

		//typedef typename UnorderedSmartVector<VELNodeWithPOtherEnd<V,VErasable> >::magic_pointer PThisEndType;
		//typedef typename UnorderedSmartVector<VELNodeWithPOtherEnd<V,VErasable> >::magic_pointer POtherEndType;

		typename Graph::VEL* pContOtherEnd;
		typename Graph::PVELNODE pOtherEnd;
	};

	// zawiera dodatkowo wskaŸnik na krawêdŸ w globalnej liœcie krawêdzi
	// nie zawiera pOtherEnd, bo wskaŸniki do koñców krawêdzi zawiera krawêdŸ na globalnej liœcie
	private: struct VELNodeWithPEdge
	{
		typename Graph::PV pDest;
		typename Graph::PE pEdge;
		//UnorderedSmartVector<VELNodeWithPEdge<E> >::magic_pointer pRev;
	};



	// ACTUAL IMPLEMENTATION ----------

	public: Graph() : mVertices(*new VCONTAINER())
	{
		ASSERT_VParentContainer_is_void<void>();
	}

	private: template<typename T>
	void ASSERT_VParentContainer_is_void(typename ENABLE_IF<SAME_TYPE<VParentContainer,void>::RET,T>::RET* dummy = 0)
	{
	}

	public: ~Graph()
	{
		if(SAME_TYPE<VParentContainer,void>::RET) delete &mVertices;
	}

	Graph(unsigned int numV) : mVertices(*new VCONTAINER(numV))
	{
		ASSERT_VParentContainer_is_void<void>();
	}

	// Subgraph constructor
	private: template<typename SubgraphOF, typename T>
	Graph(const SubgraphOF& c, typename ENABLE_IF<!SAME_TYPE<VParentContainer,void>::RET,T>::RET* dummy = 0) : mVertices(c)
	{
	}


	public: template<typename VNewPredicate>    Graph<VErasable, EErasable, Directed, EIterable, EExtra, VCONTAINER, VNewPredicate> getSubgraph() const
	{
		return Graph<VErasable, EErasable, Directed, EIterable, EExtra, VCONTAINER, VNewPredicate>(mVertices,NULL);
	}

	//unsigned int addVertex()
	//{
	//}


	/*
	HAS_MEMBER(HAS_PEND1,pEnd1);
	HAS_MEMBER(HAS_PEND2,pEnd2);
	HAS_MEMBER(HAS_POTHEREND,pOtherEnd);
	*/

	/*
	// setEdgePVelFrom
	private: template<typename PVEL, typename T>
	inline void setEdgePVelFrom(E& edge, PVEL& pvel, typename ENABLE_IF<EIterable && HAS_PEND1<E>::RET, T>::TYPE* dummy = 0)
	{
		edge.pEnd1 = pvel;
	}
	private: template<typename PVEL, typename T>
	inline void setEdgePVelFrom(E& edge, PVEL& pvel, typename ENABLE_IF<!EIterable || !HAS_PEND1<E>::RET, T>::TYPE* dummy = 0) const
	{
	}
	*/


	/*
	// setEdgePVelTo
	private: template<typename PVEL, typename T>
	inline void setEdgePVelTo(E& edge, PVEL& pvel, typename ENABLE_IF<EIterable && HAS_PEND2<E>::RET, T >::TYPE* dummy = 0)
	{
		edge.pEnd2 = pvel;
	}
	private: template<typename PVEL, typename T, typename ENABLE_IF<!EIterable || !HAS_PEND2<E>::RET, T >::TYPE>
	inline void setEdgePVelTo(E& edge, PVEL& pvel) const
	{
	}
	*/


	/*
	// setVELPOtherEnd
	private: template<typename VELNODE, typename PVELNODE, typename ENABLE_IF<HAS_POTHEREND<VELNODE>::RET >::TYPE >
	inline void setVELPOtherEnd(VELNODE& velnode, PVELNODE& pvelnode)
	{
		velnode.pOtherEnd = pvelnode;
	}
	private: template<typename VELNODE, typename PVELNODE, typename ENABLE_IF<!HAS_POTHEREND<VELNODE>::RET >::TYPE >
	inline void setVELPOtherEnd(VELNODE& velnode, PVELNODE& pvelnode)
	{
	}
	*/

		private: template<typename iterator1, typename iterator2, typename T>
		inline void addEdge(const iterator1& vertexFrom, const iterator2& vertexTo, typename ENABLE_IF<!EIterable && Directed,T>::RET* dummy = 0)
		{
			V& v1 = getV(vertexFrom);
			v1.to.push_back(VELNODE());
			v1.to.back().pDest = getPV<void>(vertexTo);
		}
		private: template<typename iterator1, typename iterator2, typename T>
		inline void addEdge(const iterator1& vertexFrom, const iterator2& vertexTo, typename ENABLE_IF<!EIterable && !Directed && !EErasable,T*>::RET* dummy = 0)
		{
			V& v1 = getV(vertexFrom);
			V& v2 = getV(vertexTo);

			v1.to.push_back(VELNODE());
			v1.to.back().pDest = getPV<void>(vertexTo);

			v2.to.push_back(VELNODE());
			v2.to.back().pDest = getPV<void>(vertexFrom);
		}
		private: template<typename iterator1, typename iterator2, typename T>
		inline void addEdge(const iterator1& vertexFrom, const iterator2& vertexTo, typename ENABLE_IF<!EIterable && !Directed && EErasable,T***>::RET* dummy = 0)
		{
			V& v1 = getV(vertexFrom);
			V& v2 = getV(vertexTo);

			v1.to.push_back(VELNODE());
			v1.to.back().pDest = getPV<void>(vertexTo);

			v2.to.push_back(VELNODE());
			v2.to.back().pDest = getPV<void>(vertexFrom);

			v1.to.back().pContOtherEnd = &v2.to;
			v1.to.back().pOtherEnd = getVELPointer(v2.to, v2.to.size()-1u);

			v2.to.back().pContOtherEnd = &v1.to;
			v2.to.back().pOtherEnd = getVELPointer(v1.to, v1.to.size()-1u);
		}
		private: template<typename iterator1, typename iterator2, typename T>
		inline void addEdge(const iterator1& vertexFrom, const iterator2& vertexTo, typename ENABLE_IF<EIterable && Directed && !EErasable,T****>::RET* dummy = 0)
		{
			V& v1 = getV(vertexFrom);

			v1.to.push_back(typename V::ToVELNODE());
			v1.to.back().pDest = getPV(vertexTo);

			mEdges.push_back();
			v1.to.back().pEdge = getEPointer(mEdges, mEdges.size()-1);
		}
		private: template<typename iterator1, typename iterator2, typename T>
		inline void addEdge(const iterator1& vertexFrom, const iterator2& vertexTo, typename ENABLE_IF<EIterable && Directed && EErasable,T*****>::RET* dummy = 0)
		{
			V& v1 = getV(vertexFrom);

			v1.to.push_back(typename V::ToVELNODE());
			v1.to.back().pDest = getPV(vertexTo);

			mEdges.push_back();
			v1.to.back().pEdge = getEPointer(mEdges, mEdges.size()-1u);
			mEdges.back().pEnd1 = getVELPointer(v1.to, v1.to.size()-1u);
		}
		private: template<typename iterator1, typename iterator2, typename T>
		inline void addEdge(const iterator1& vertexFrom, const iterator2& vertexTo, typename ENABLE_IF<EIterable && !Directed && !EErasable,T******>::RET* dummy = 0)
		{
			V& v1 = getV(vertexFrom);
			V& v2 = getV(vertexTo);

			v1.to.push_back(typename V::ToVELNODE());
			v1.to.back().pDest = getPV(vertexTo);

			v2.to.push_back(typename V::ToVELNODE());
			v2.to.back().pDest = getPV(vertexFrom);

			mEdges.push_back();
			v1.to.back().pEdge = getEPointer(mEdges, mEdges.size()-1u);
		}
		private: template<typename iterator1, typename iterator2, typename T>
		inline void addEdge(const iterator1& vertexFrom, const iterator2& vertexTo, typename ENABLE_IF<EIterable && !Directed && EErasable,T*******>::RET* dummy = 0)
		{
			V& v1 = getV(vertexFrom);
			V& v2 = getV(vertexTo);

			v1.to.push_back(typename V::ToVELNODE());
			v1.to.back().pDest = getPV(vertexTo);

			v2.to.push_back(typename V::ToVELNODE());
			v2.to.back().pDest = getPV(vertexFrom);

			mEdges.push_back();
			v1.to.back().pEdge = getEPointer(mEdges, mEdges.size()-1u);
			mEdges.back().pEnd1 = getVELPointer(v1.to, v1.to.size()-1u);
			mEdges.back().pEnd2 = getVELPointer(v2.to, v2.to.size()-1u);
		}


	public: template<typename iterator1, typename iterator2>
	inline void addEdge(const iterator1& vertexFrom, const iterator2& vertexTo)
	{
		addEdge<iterator1,iterator2,void>(vertexFrom, vertexTo);
	}



	// ---------- eraseEdge ----------

		private: template<typename container, typename iterator, typename T>
		inline void eraseEdge(container& cont, iterator& pvelnode, typename ENABLE_IF<EErasable && !EIterable && !Directed,T>::RET* dummy = 0)
		{
			VELNODE& velnode = GetVELNode(cont, pvelnode);
			VEL& c2 = *velnode.pContOtherEnd;
			PVELNODE pother = velnode.pOtherEnd;
			containerErase(cont, pvelnode);
			containerErase(c2, pother);
		}

		private: template<typename container, typename iterator, typename T>
		inline void eraseEdge(container& cont, iterator& pvelnode, typename ENABLE_IF<EErasable && !EIterable && Directed,T>::RET** dummy = 0)
		{
			containerErase(cont, pvelnode);
		}

	private: template<typename container, typename iterator>
	inline void eraseEdge(container& cont, iterator& pvelnode)
	{
		eraseEdge<container,iterator, void>(cont,pvelnode);
	}


	public: void printVertices()
	{
		for(unsigned int i=0; i<mVertices.size(); ++i)
		{
			printf("VERTEX %d     ->     ", i);
			for(unsigned int j=0; j<mVertices[i]->to.size(); ++j)
			{
				printf("%u  ", getVIndex(mVertices[i]->to[j].pDest));
			}
			printf("\n");
		}
	}


	inline unsigned int& getVIndex(unsigned int& index) const
	{
		return index;
	}

	template<typename pointer>
	inline unsigned int getVIndex(pointer& mp) const
	{
		return mp - mVertices.begin();
	}


	private: template<typename Iterator>
	inline const V& getV(const Iterator& itr, typename ENABLE_IF<IS_CLASS<Iterator>::RET && HAS_OPERATOR_STAR<Iterator>::RET >::RET* dummy = 0) const
	{
		return **itr;
	}

	private: inline V& getV(unsigned int index) const
	{
		return *mVertices[index];
	}


	// getPV
	private: template<typename T>
	inline const PV& getPV(const PV& vp) const
	{
		return vp;
	}
	
	private: template<typename T>
	inline PV& getPV(PV& vp) const
	{
		return vp;
	}
	private: template<typename T>
	inline PV getPV(unsigned int index, typename ENABLE_IF<VErasable, T>::RET* dummy = 0)
	{
		return mVertices.getMagicPointer(index);
	}



	static inline PVELNODE getVELPointer(UnorderedSmartVector<VELNODE>& v, unsigned int index)
	{
		return v.getMagicPointer(index);
	}

	static inline PVELNODE getVELPointer(Vector<VELNODE>& v, unsigned int index)
	{
		return index;
	}

	static inline VELNODE& GetVELNode(const VEL& container, unsigned int index)
	{
		return container[index];
	}
	static inline VELNODE& GetVELNode(const VEL&, typename VEL::iterator iterator)
	{
		return *iterator;
	}

	// nie powinno byc dostêpne
	/*
	static inline unsigned int GetVELPointer(UnorderedVector<typename VELINFO::VELNODE>& v, unsigned int index)
	{
		return index;
	}
	*/


	private: template<typename T>
	bool validV(T&)
	{
		return true;
	}
	// TODO


	

private:
	VCONTAINER& mVertices;
	ECONTAINER mEdges;
};


} // namespace ab;
