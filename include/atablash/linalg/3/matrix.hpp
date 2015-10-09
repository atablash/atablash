#pragma once


#include "translation.hpp"
#include "quaternion.hpp"
#include "../2/vector.hpp"
#include "../2/matrix.hpp"

namespace ab{

template<typename TFLOAT>
class Matrix33
{
public:
	TFLOAT m[3][3];

public:
	Matrix33(const Quaternion<TFLOAT>& quat)
	{
		TFLOAT wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

		// calculate coefficients
		x2 = quat.x + quat.x; y2 = quat.y + quat.y;
		z2 = quat.z + quat.z;
		xx = quat.x * x2; xy = quat.x * y2; xz = quat.x * z2;
		yy = quat.y * y2; yz = quat.y * z2; zz = quat.z * z2;
		wx = quat.w * x2; wy = quat.w * y2; wz = quat.w * z2;

		m[0][0] = 1.0 - (yy + zz);
		m[1][0] = xy - wz;
		m[2][0] = xz + wy;

		m[0][1] = xy + wz;
		m[1][1] = 1.0 - (xx + zz);
		m[2][1] = yz - wx;


		m[0][2] = xz - wy;
		m[1][2] = yz + wx;
		m[2][2] = 1.0 - (xx + yy);
	}
};




template<typename TFLOAT>
class Matrix32
{
public:
	TFLOAT m[3][2];

public:

	inline TFLOAT& operator()(int i, int j){
		return m[i][j];
	}
	inline const TFLOAT& operator()(int i, int j) const {
		return m[i][j];
	}
	
	class Col
	{
	public:
		inline Col& operator=(const Vector3<TFLOAT>& newcol)
		{
			mat(0, col) = newcol.x;
			mat(1, col) = newcol.y;
			mat(2, col) = newcol.z;

			return *this;
		}

		inline Col& operator=(const Translation3<TFLOAT>& newcol)
		{
			mat(0, col) = newcol.x;
			mat(1, col) = newcol.y;
			mat(2, col) = newcol.z;

			return *this;
		}
	public:
		Col(Matrix32& _mat, int _col) : mat(_mat), col(_col) {}
		Matrix32& mat;
		int col;
	};
	
	Col col(int i){
		return Col(*this,i);
	}
	
	static Matrix32 Cols(const Vector3<TFLOAT>& col0, const Vector3<TFLOAT>& col1){
		Matrix32 result;
		result.col(0) = col0;
		result.col(1) = col1;
		return result;
	}
	
};







template<typename TFLOAT>
class Matrix23
{
public:
	TFLOAT m[2][3];

public:

	inline TFLOAT& operator()(int i, int j){
		return m[i][j];
	}
	inline const TFLOAT& operator()(int i, int j) const {
		return m[i][j];
	}

	class Col
	{
	public:
		inline Col& operator=(const Vector2<TFLOAT>& newcol)
		{
			mat(0, col) = newcol.x;
			mat(1, col) = newcol.y;

			return *this;
		}

		/*inline Col& operator=(const Translation2<TFLOAT>& newcol)
		{
			mat(0, col) = newcol.x;
			mat(1, col) = newcol.y;

			return *this;
		}*/
	public:
		Col(Matrix32& _mat, int _col) : mat(_mat), col(_col) {}
		Matrix32& mat;
		int col;
	};

	Col col(int i){
		return Col(*this, i);
	}


	class Row
	{
	public:
		inline Row& operator=(const Vector3<TFLOAT>& newrow)
		{
			mat(row, 0) = newrow.x;
			mat(row, 1) = newrow.y;
			mat(row, 2) = newrow.z;

			return *this;
		}

		inline Row& operator=(const Translation3<TFLOAT>& newrow)
		{
			mat(row, 0) = newrow.x;
			mat(row, 1) = newrow.y;
			mat(row, 2) = newrow.z;

			return *this;
		}
	public:
		Row(Matrix23& _mat, int _row) : mat(_mat), row(_row) {}
		Matrix23& mat;
		int row;
	};

	Row row(int i){
		return Row(*this, i);
	}



	static Matrix23 Cols(const Vector2<TFLOAT>& col0, const Vector2<TFLOAT>& col1, const Vector2<TFLOAT>& col2){
		Matrix23 result;
		result.col(0) = col0;
		result.col(1) = col1;
		result.col(2) = col2;
		return result;
	}

	static Matrix23 Rows(const Vector3<TFLOAT>& row0, const Vector3<TFLOAT>& row1){
		Matrix23 result;
		result.row(0) = row0;
		result.row(1) = row1;
		return result;
	}

};








template<class F>
inline Matrix32<F> operator*(const Matrix32<F>& a, const Matrix2<F>& b){
	Matrix32<F> result;
	result.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0];
	result.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1];
	result.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0];
	result.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1];
	result.m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0];
	result.m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1];
	return result;
}




template<class F>
inline Vector3<F> operator*(const Matrix32<F>& a, const Vector2<F>& b){
	Vector3<F> result;
	result.x = a.m[0][0] * b.x + a.m[0][1] * b.y;
	result.y = a.m[1][0] * b.x + a.m[1][1] * b.y;
	result.z = a.m[2][0] * b.x + a.m[2][1] * b.y;
	return result;
}



template<class F>
inline Vector2<F> operator*(const Matrix23<F>& a, const Vector3<F>& b){
	Vector2<F> result;
	result.x = a.m[0][0] * b.x + a.m[0][1] * b.y + a.m[0][2] * b.z;
	result.y = a.m[1][0] * b.x + a.m[1][1] * b.y + a.m[1][2] * b.z;
	return result;
}








}


