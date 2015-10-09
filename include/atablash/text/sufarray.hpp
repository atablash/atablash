#pragma once

#include<atablash/sort.hpp>

namespace ab{


// O(n log n) suffix array
inline vector<ui32> kmr(const string& s){
	vector<ui32> v(s.size()*2,128);
	for(ui32 i=0; i<s.size(); ++i){
		v[i] = s[i];
	}
	ui32 univ = 129;

	for(ui32 k=1,nextk; (nextk=(k<<1))<=s.size(); k=nextk){

		std::vector<std::pair<std::pair<ui32,ui32>,ui32>> pairs,pairs2;
		for(ui32 from=0; from<s.size(); ++from){
			pairs.push_back(make_pair(make_pair(v[from],v[from+k]),from));
		}
		pairs2.resize(pairs.size());

		//E<pairs;

		// radix
		countingSort(pairs.begin(), pairs.end(), pairs2.begin(), univ,
			[](std::pair<std::pair<ui32,ui32>,ui32>&p){return p.first.second;});
		countingSort(pairs2.begin(), pairs2.end(), pairs.begin(), univ,
			[](std::pair<std::pair<ui32,ui32>,ui32>&p){return p.first.first;});

		//E<pairs;

		std::pair<ui32,ui32> last(-1,-1);
		int ith = -1;
		for(auto&p:pairs){
			if(p.first != last){
				last = p.first;
				++ith;
			}
			ui32 dest = p.second;
			v[dest] = ith;
		}
		++ith;
		for(ui32 i=s.size(); i<2*s.size(); ++i){
			v[i]=ith;
		}
		univ = ith+1;

		//E<v;
	}

	//v.resize(s.size());

	vector<ui32> result(s.size());
	for(ui32 i=0; i<s.size(); ++i){
		result[v[i]]=i;
	}
	return result;
}




}



