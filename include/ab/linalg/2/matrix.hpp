#pragma once

#include<atablash/linalg/2/vector.hpp>

namespace ab{



template<class F>
class Matrix2{
public:
	F m[2][2];

public:

	inline F& operator()(int i, int j){
		return m[i][j];
	}
	inline const F& operator()(int i, int j) const {
		return m[i][j];
	}

	class Col
	{
	public:
		inline Col& operator=(const Vector2<F>& newcol)
		{
			mat(0, col) = newcol.x;
			mat(1, col) = newcol.y;

			return *this;
		}
	public:
		Col(Matrix2& _mat, int _col) : mat(_mat), col(_col) {}
		Matrix2& mat;
		int col;
	};
	
	Col col(int i){
		return Col(*this,i);
	}
	
	
	class Row
	{
	public:
		inline Row& operator=(const Vector2<F>& newrow){
			mat(0, row) = newrow.x;
			mat(1, row) = newrow.y;

			return *this;
		}
	public:
		Row(Matrix2& _mat, int _row) : mat(_mat), row(_row) {}
		Matrix2& mat;
		int row;
	};
	
	Row row(int i){
		return Row(*this,i);
	}
	
	
	
	
	Matrix2 operator*(const Matrix2& o)const{
		Matrix2 result;
		result(0,0) = (*this)(0,0)*o(0,0) + (*this)(0,1)*o(1,0);
		result(0,1) = (*this)(0,0)*o(0,1) + (*this)(0,1)*o(1,1);
		result(1,0) = (*this)(1,0)*o(0,0) + (*this)(1,1)*o(1,0);
		result(1,1) = (*this)(1,0)*o(0,1) + (*this)(1,1)*o(1,1);
		return result;
	}
	
	
	
	
	Matrix2(){
		m[0][0] = 1;
		m[1][1] = 1;
		m[1][0] = 0;
		m[0][1] = 0;
	}
	
	static inline Matrix2 cols(const Vector2<F>& col0, const Vector2<F>& col1){
		Matrix2 result;
		result.col(0) = col0;
		result.col(1) = col1;
		return result;
	}
	
	static inline Matrix2 rows(const Vector2<F>& row0, const Vector2<F>& row1){
		Matrix2 result;
		result.row(0) = row0;
		result.row(1) = row1;
		return result;
	}
	
	Matrix2& invert(){
		*this = inverted();	// TODO: can do in-place with only 1 swap
		return *this;
	}
	
	Matrix2 inverted()const{
		Matrix2 result;
		F mult = (F)1.0 / det();
		result.m[0][0] = m[1][1]*mult;
		result.m[1][1] = m[0][0]*mult;
		result.m[0][1] = -m[0][1]*mult;
		result.m[1][0] = -m[1][0]*mult;
		return result;
	}
	
	inline typename Double<F>::R det()const{
		return (typename Double<F>::R)m[0][0]*m[1][1]
			- (typename Double<F>::R)m[1][0]*m[0][1];
	}
};





// returns det(b-a, c-a)
template<class T>
inline typename Double<T>::R orient(const Vector2<T>& a, const Vector2<T>& b, const Vector2<T>& c){
	return Matrix2<T>::rows(b-a, c-a).det();
}
	
	
	
	
	
}

