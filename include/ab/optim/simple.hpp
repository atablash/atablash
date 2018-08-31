#pragma once

#include<atablash/math/common.hpp>
#include<atablash/debug.hpp>

#include<atablash/math-tables.hpp>

namespace ab
{


// convert a value from [0..1] to [-inf..inf]
template<class F> inline F realFrom01(F x){
	ASS(x >= 0 and x <= 1);
	return (x-0.5) / (x*(1-x));
}

// ...and back
template<class F> inline F realTo01(F y){
	if(isinf(y)) return y>0;
	return 0.5 + (sqrt(y*y+1)-1) / (2*y);
}



template<class F> inline F realFromPositive(F x){
	ASS(x >= 0);
	return log(x);
}

template<class F> inline F realToPositive(F x){
	return exp(x);
}




// optimize naively a vector of real variables
template<class Eval>
class RandomOptimizer
{
public:
	RandomOptimizer(const Eval& eval, int dims) : mEval(eval), mDims(dims), mVars(dims) {}
	
	const Eval mEval;
	const int mDims;
	int mMaxIters = 100;
	double mDefDev = 1.0;
	vector<double> mVars;
	
	void minimize(){
		vector<double> new_vars(mDims);
		ASS(mVars.size() == mDims);
		vector<double> devs(mDims,mDefDev);
		double val = mEval(mVars);
		double dev = 1.0;

		for(int iter=0; iter<mMaxIters; ++iter){
			for(int j=0; j<mDims; ++j){
				double d = rand_normal()*dev*devs[j];
				new_vars[j] = mVars[j] + d;
			}
			double new_val = mEval(new_vars);
			if(new_val < val){
				val = new_val;
				for(int j=0; j<mDims; ++j){
					devs[j] = lerp(devs[j], fabs(new_vars[j] - mVars[j])*4, 0.9);
				}
				swap(mVars,new_vars);
			}
			else dev *= 0.5;
		}
	}
};
template<class Eval> inline RandomOptimizer<Eval> make_RandomOptimizer(const Eval& eval, int dims){
	return RandomOptimizer<Eval>(eval,dims);
}





}





