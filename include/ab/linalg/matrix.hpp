//////////////////////////////////////////////////////////////////////////////
//
// Implementation: Adam Blaszkiewicz (atablash.pl), 2014
//
/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once


#include<atablash/debug.hpp>

#include<vector>

namespace ab{

using namespace std;

// row major MxN matrix
template<typename F>
class Matrix{
public:
	Matrix(){
		//es.reserve(2*2);
	}

	//template<bool zero=true>
	Matrix(const size_t& m, const size_t& n){
		reservedN = n;
		es.resize(m*n);
		//if(zero) memset(&es[0],0,sizeof(F)*m*n);
		_m=m; _n=n;
	}

	F& operator()(size_t i, size_t j){return es[i*reservedN + j];}
	const F& operator()(const size_t& i, const size_t& j)const{return es[i*reservedN + j];}

	template<bool zero=true>
	void resize(size_t m, size_t n){
		resizeN(n);
		resize(m);
	}

	void resize(size_t m){
		//reserveM(m);
		es.resize(m*reservedN);
		_m=m;
	}

	template<bool zero = true>
	void resizeN(size_t n){
		if(n > _n){
			reserveN(n);
			if(zero){
				for(int i=0; i<_m; ++i){
					memset(&es[reservedN*i + _n],0,sizeof(F)*(n-_n));
				}
			}
		}
		_n=n;
	}

	void reserve(size_t m, size_t n){
		DE if(es.capacity() >= m*n) LOG("Performance warning");

		es.reserve(m*n);
		relocate(reservedN, n);
		reservedN = n;
	}

	void reserveM(size_t m){
		DE if(es.capacity() >= m*reservedN) LOG("Performance warning");
		reservedM = m;
		es.reserve(reservedM * reservedN);
	}

	void reserveN(size_t n){
		if(n <= reservedN){
			DE LOG("Performance warning");
			return;
		}
		DE if(es.capacity() >= n*_m) LOG("Performance warning");
		es.reserve(reservedM * n);
		es.resize(_m * n);
		relocate(reservedN, n);
		reservedN = n;
	}



	pair<size_t,size_t> size() const{
		return make_pair(_m,_n);
	}

	pair<size_t,size_t> capacity() const{
		return make_pair(reservedM,reservedN);
	}

	class row_context{
	public:
		row_context(Matrix&_mat, const size_t& _row) : mat(_mat),row(_row){}

		void operator*=(const F& a){
			for(int j=0; j<mat._n; ++j){
				mat(row,j) *= a;
			}
		}
	private:
		Matrix& mat;
		size_t row;
	};

	row_context operator()(const size_t& row){
		return row_context(*this,row);
	}

private:
	//template<bool zero = true>
	void relocate(size_t oldN, size_t newN){
		if(oldN == newN){
			//L("Performance warning");
			return;
		}
		OF(i,0,_m){
			//E<<"memcpy "<<newN*i << " " << oldN*i << " " << sizeof(F)*_n << N;
			memcpy(&es[newN*i], &es[oldN*i], sizeof(F)*_n);
			//if(zero) memset(&es[newN*i + _n], 0, sizeof(F)*(newN-_n));
		}
	}

	size_t reservedM = 2;
	size_t reservedN = 2;
	size_t _m=0, _n=0;
	vector<F> es;
};

template<typename F>
ostream& operator<<(ostream&o,const Matrix<F>&m){
	o.precision(2);
	for(int i=0; i<m.size().first; ++i){
		for(int j=0; j<m.size().second; ++j){
			o<<m(i,j)<<'\t';
		}
		o<<'\n';
	}
	return o;
}

}
