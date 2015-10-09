#pragma once


#include <atablash/kd.hpp>
#include <atablash/stlext.hpp>

namespace ab{






// returns means, assignment
template<class T = double>
inline std::pair< std::vector<std::vector<T> >, vector<int> > kmeans(const std::vector<std::vector<T> >& data, int k, int _dims, int iterLimit = INT_MAX){
	ASS(si data.siz >= k);
	std::vector<std::vector<T> > means(k);
	F(i,k) means[i] = data[i];

	vector<int> assign(data.siz);

	vector<int> nums(k);

	F(iter,iterLimit){
		// assignment
		int changes = 0;
		KD<T,int> kd(_dims);
		F(i,k) kd.insert(means[i],i);

		F(i,data.siz){
			int a = kd.closest(data[i]).se;
			if(assign[i] != a){
				++changes;
				assign[i] = a;
			}
		}

		E < "kmeans changes: " < changes < EN;
		if(changes == 0) break;

		// calculate means
		F(i,k){
			nums[i] = 0;
			F(j,_dims) means[i][j] = 0;
		}
		F(i,data.siz){
			F(d,_dims) means[assign[i]][d] += data[i][d];
			++nums[assign[i]];
		}
		F(i,k) means[i] *= 1.0/nums[i];
	}

	return mp(means, assign);
}






}
