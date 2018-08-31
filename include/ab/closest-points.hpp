#pragma once

#include<atablash/templates.hpp>

#include<vector>
#include<limits>

namespace ab{


// closest 2 points in 2D
// normal recursive algorithm (not "miotła Kunysza" - unfortunately)
//
// can change x,y to first,second - templates are for this
template<bool xsorted, class ITER,
	class X = decltype(&RR<decltype(*ITER())>::R::x),
	class Y = decltype(&RR<decltype(*ITER())>::R::y)>
auto closest2d(const ITER& begin, const ITER& end,
	const X& x = &RR<decltype(*ITER())>::R::x,
	const Y& y = &RR<decltype(*ITER())>::R::y)
		-> typename FloatFrom<typename RR<decltype(ITER()->*y)>::R>::R
{
	typedef typename std::remove_reference<decltype(*begin)>::type Point;
	typedef typename RR<decltype(Point().*x)>::R F;
	if(!xsorted) sort(begin,end,[x,y](const Point& p1, const Point& p2){return p1.*x < p2.*x;});

	uint size = end-begin;
	std::vector<Point> temp(size);

	typename FloatFrom<F>::R result = std::numeric_limits<typename FloatFrom<F>::R>::max();

	for(uint d=1; d<size; d<<=1){
		for(uint ii=0; ii+d<size; ii+=2*d){
			auto i0 = begin + ii;
			auto i1 = i0 + d;
			auto i2 = min(i1 + d,end);

			auto i = i0;
			auto j = i1;

			F a = (j-1)->*x;
			F b = (j)->*x;

			typename Double<F>::R best = std::numeric_limits<typename Double<F>::R>::max();

			// TODO: store mins i maxs to avoid recomputing every time
			auto emin = std::numeric_limits<F>::max();
			auto emax = -std::numeric_limits<F>::max();	// TODO: support unsigned

			// find new
			for(; i!=i1 && i->*y != std::numeric_limits<F>::max(); ++i){
				emin = min(emin,i->*x);
				if(i->*x < b-result)continue;
				while(j!=i2 && j->*y != std::numeric_limits<F>::max() && (j)->*y + result < (i)->*y){
					emax = max(emax, j->*x);
					++j;
				}
				for(auto jj=j; jj!=i2 && jj->*y!=std::numeric_limits<F>::max() && (jj)->*y - result <= (i)->*y; ++jj){
					if(jj->*x > a+result)continue;
					typename Double<F>::R dy = (jj)->*y - (i)->*y;
					typename Double<F>::R dx = (jj)->*x - (i)->*x;
					typename Double<F>::R cand2 = dy*dy + dx*dx;
					best = min(best,cand2);
				}
			}
			while(j!=i2 && j->*y != std::numeric_limits<F>::max()){
				emax = max(emax, j->*x);
				++j;
			}
			result = min(result, sqrt(best));

			// merge points
			i=i0;
			j=i1;
			auto t=temp.begin();
			for(;;){
				if(i->*x > emin+result){
					++i;
				} else if(j->*x < emax-result){
					++j;
				} else if((i)->*y < (j)->*y) *(t++) = *(i++);
				else *(t++)=*(j++);
				if(i==i1 || i->*y==std::numeric_limits<F>::max()){
					while(j!=i2 && j->*y!=std::numeric_limits<F>::max())*(t++)=*(j++);
					break;
				} else if(j==i2 || j->*y==std::numeric_limits<F>::max()){
					while(i!=i1 && i->*y!=std::numeric_limits<F>::max())*(t++)=*(i++);
					break;
				}
			}
			copy(temp.begin(), t, i0);
			if(t-temp.begin()<2*d) (i0+(t-temp.begin()))->*y = std::numeric_limits<F>::max();
		}
	}
	return result;
}


}
