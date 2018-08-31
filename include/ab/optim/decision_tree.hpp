#pragma once

namespace ab{


// PDATA could be sptr<vector<vector<F>>>
template<class F, class PDATA>
class DecisionTree{
public:
	DecisionTree(const PDATA& data) : mData(data){
		mRoot.elems.resize(data->size());
		FOR(i,data->size()){
			mRoot.elems[i] = i;
		}
	}
	
private:
	PDATA mData;
	struct Node{
		vector<int> elems;
		int splitVar = -1;
		F splitVal;
		uptr<Node> le, ri;
	} mRoot;
};




}
