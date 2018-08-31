#pragma once

#include<cstring>

#include<atablash/debug.hpp>
#include<algorithm>

namespace ab
{

	// Niepotrzebne, bo ProxyComp na funkcjach mo¿e zast¹piæ wskaŸniki
	/*
	// compare by element1.*proxyPtr < element1.*proxyPtr
	template<typename ProxyPtr>
	class PtrProxyComp
	{
	public:
		PtrProxyComp(const ProxyPtr& _proxyPtr) : proxyPtr(_proxyPtr) {}
		
		template<typename Elem>
		inline bool operator()(const Elem& e1, const Elem& e2) const
		{
			return e1.*proxyPtr < e2.*proxyPtr;
		}
	private:
		const ProxyPtr& proxyPtr;
		//const CompFun& compFun;
	};
	template<typename ProxyPtr> PtrProxyComp<ProxyPtr> makePtrProxyComp(const ProxyPtr& proxyPtr)
	{
		return PtrProxyComp<ProxyPtr>(proxyPtr);
	}
	*/

	// compare by compFun( proxyFun(element1), proxyFun(element1) )
	template<typename ProxyFun>
	class ProxyComp
	{
	public:
		ProxyComp(const ProxyFun& _proxyFun) : proxyFun(_proxyFun) {}
		
		template<typename Elem>
		inline bool operator()(const Elem& e1, const Elem& e2) const
		{
			return proxyFun(e1) < proxyFun(e2);
		}
	private:
		const ProxyFun& proxyFun;
		//const CompFun& compFun;
	};
	template<typename ProxyFun> ProxyComp<ProxyFun> makeProxyComp(const ProxyFun& proxyFun)
	{
		return ProxyComp<ProxyFun>(proxyFun);
	}

	// compare indices by comparing _container elements at those indices
	template<typename Cont, typename CompFun>
	class IndexProxyComp
	{
	public:
		IndexProxyComp(const Cont& _container, const CompFun& _compFun /* = Cont::value_type::operator< */ ) : cont(_container), compFun(_compFun) {}
		
		template<typename Index>
		inline bool operator()(const Index& a, const Index& b) const
		{
			return compFun(cont[a], cont[b]);
		}
	private:
		const Cont& cont;
		const CompFun& compFun;
	};
	template<typename Cont, typename CompFun> IndexProxyComp<Cont,CompFun> makeIndexProxyComp(const Cont& _container, const CompFun& _compFun = Cont::value_type::operator<)
	{
		return IndexProxyComp<Cont,CompFun>(_container, _compFun);
	}

	template<typename T, typename Cont, typename Comp>
	void sort_indices(const T& indicesBegin, const T& indicesEnd, const Cont& container, const Comp& comp = Cont::value_type::operator<)
	{
		int num = indicesEnd - indicesBegin;
		ASS(num >= 0);
		T iter = indicesBegin;
		for(int i=0; iter!=indicesEnd; ++i, ++iter) *iter = i;
		
		std::sort(indicesBegin, indicesEnd, makeIndexProxyComp(container, comp));
	}
	
	template<typename T>
	void countingSort(const T& inBegin, const T& inEnd, const T& outBegin, int univ)
	{
		A(outBegin + (inEnd - inBegin) <= inBegin || inEnd <= outBegin);	// in i out nie mog¹ na siebie zachodziæ
		
		int *count = new int[univ];
		memset(count, 0, sizeof(int)*univ);
		
		for(T inCur = inBegin; inCur != inEnd; ++inCur)
		{
			int val = *inCur;
			++count[val];
		}
		
		for(int i=1; i<univ; ++i) count[i] += count[i-1];
		
		for(T inCur = inEnd;;)
		{
			--inCur;
			
			int val = *inCur;
			
			T out = outBegin + --count[val];
			
			*out = *inCur;
			
			if(inCur==inBegin)break;
		}
		
		delete[] count;
	}
	
	template<typename T, typename GetVal>
	void countingSort(const T& inBegin, const T& inEnd, const T& outBegin, int univ, const GetVal& getVal)
	{
		ASS(outBegin + (inEnd - inBegin) <= inBegin || inEnd <= outBegin);	// in i out nie mog¹ na siebie zachodziæ
		
		int *count = new int[univ];
		memset(count, 0, sizeof(int)*univ);
		
		for(T inCur = inBegin; inCur != inEnd; ++inCur)
		{
			int val = getVal(*inCur);
			++count[val];
		}
		
		for(int i=1; i<univ; ++i) count[i] += count[i-1];
		
		for(T inCur = inEnd;;)
		{
			--inCur;
			
			int val = getVal(*inCur);
			
			T out = outBegin + --count[val];
			
			*out = *inCur;
			
			if(inCur==inBegin)break;
		}
		
		delete[] count;
	}
}