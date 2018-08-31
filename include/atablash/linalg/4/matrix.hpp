#pragma once


#include <atablash/linalg/3/vector.hpp>
#include <atablash/linalg/4/vector.hpp>
#include <atablash/linalg/3/translation.hpp>
#include <atablash/linalg/3/quaternion.hpp>


namespace ab{



	template<typename TFLOAT>
	class Matrix34
	{
	public:
		TFLOAT m[3][4];

	public:
		class TCol
		{
		public:
			inline TCol& operator=(const Vector3<TFLOAT>& newcol)
			{
				mat(0, col) = newcol.x;
				mat(1, col) = newcol.y;
				mat(2, col) = newcol.z;

				return *this;
			}

			inline TCol& operator=(const Translation3<TFLOAT>& newcol)
			{
				mat(0, col) = newcol.x;
				mat(1, col) = newcol.y;
				mat(2, col) = newcol.z;

				return *this;
			}
		public:
			TCol(Matrix34& _mat, int _col) : mat(_mat), col(_col) {}
			Matrix34& mat;
			int col;
		};
		inline typename Matrix34::TCol col(int j)
		{
			return TCol(*this, j);
		}

	public:
		Matrix34() {
			// optionally make identity
			memset(&m, 0, sizeof(m));
			m[0][0] = 1;
			m[1][1] = 1;
			m[2][2] = 1;
		}


		Matrix34(const Translation3<TFLOAT>& t) : Matrix34() {
			col(3) = t;
		}

		Matrix34(const Quaternion<TFLOAT>& quat)
		{
			TFLOAT wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

			// calculate coefficients
			x2 = quat.x + quat.x; y2 = quat.y + quat.y;
			z2 = quat.z + quat.z;
			xx = quat.x * x2; xy = quat.x * y2; xz = quat.x * z2;
			yy = quat.y * y2; yz = quat.y * z2; zz = quat.z * z2;
			wx = quat.w * x2; wy = quat.w * y2; wz = quat.w * z2;



			(*this)(0, 0) = (TFLOAT)(1.0 - (yy + zz));
			(*this)(0, 1) = (TFLOAT)(xy - wz);
			(*this)(0, 2) = xz + wy;
			(*this)(0, 3) = (TFLOAT)0.0;

			(*this)(1, 0) = xy + wz;
			(*this)(1, 1) = (TFLOAT)1.0 - (xx + zz);
			(*this)(1, 2) = yz - wx;
			(*this)(1, 3) = (TFLOAT)0.0;


			(*this)(2, 0) = xz - wy;
			(*this)(2, 1) = yz + wx;
			(*this)(2, 2) = (TFLOAT)1.0 - (xx + yy);
			(*this)(2, 3) = (TFLOAT)0.0;
		}
		Matrix34(const Translation3<TFLOAT>& translation, const Quaternion<TFLOAT>& quat) : Matrix34(quat) {
			this->col(3) = translation;
		}

		TFLOAT& operator()(int i, int j)
		{
			return m[i][j];	// row-major
		}
		const TFLOAT& operator()(int i, int j) const
		{
			return m[i][j];	// row-major
		}


		// missing row is interpreted like in the identity matrix (0 0 0 1)
		Matrix34<TFLOAT> operator*(const Matrix34<TFLOAT>& o) const
		{
			Matrix34<TFLOAT> result;

			result(0, 0) = (*this)(0, 0) * o(0, 0) + (*this)(0, 1) * o(1, 0) + (*this)(0, 2) * o(2, 0); // +(*this)(0, 3) * o(3, 0);
			result(0, 1) = (*this)(0, 0) * o(0, 1) + (*this)(0, 1) * o(1, 1) + (*this)(0, 2) * o(2, 1);
			result(0, 2) = (*this)(0, 0) * o(0, 2) + (*this)(0, 1) * o(1, 2) + (*this)(0, 2) * o(2, 2);
			result(0, 3) = (*this)(0, 0) * o(0, 3) + (*this)(0, 1) * o(1, 3) + (*this)(0, 2) * o(2, 3) + (*this)(0, 3);

			result(1, 0) = (*this)(1, 0) * o(0, 0) + (*this)(1, 1) * o(1, 0) + (*this)(1, 2) * o(2, 0);
			result(1, 1) = (*this)(1, 0) * o(0, 1) + (*this)(1, 1) * o(1, 1) + (*this)(1, 2) * o(2, 1);
			result(1, 2) = (*this)(1, 0) * o(0, 2) + (*this)(1, 1) * o(1, 2) + (*this)(1, 2) * o(2, 2);
			result(1, 3) = (*this)(1, 0) * o(0, 3) + (*this)(1, 1) * o(1, 3) + (*this)(1, 2) * o(2, 3) + (*this)(1, 3);

			result(2, 0) = (*this)(2, 0) * o(0, 0) + (*this)(2, 1) * o(1, 0) + (*this)(2, 2) * o(2, 0);
			result(2, 1) = (*this)(2, 0) * o(0, 1) + (*this)(2, 1) * o(1, 1) + (*this)(2, 2) * o(2, 1);
			result(2, 2) = (*this)(2, 0) * o(0, 2) + (*this)(2, 1) * o(1, 2) + (*this)(2, 2) * o(2, 2);
			result(2, 3) = (*this)(2, 0) * o(0, 3) + (*this)(2, 1) * o(1, 3) + (*this)(2, 2) * o(2, 3) + (*this)(2, 3);

			return result;
		}

		Matrix34<TFLOAT>& operator*=(const Translation3<TFLOAT>& o)
		{
			(*this)(0, 3) += o.x * (*this)(0, 0) + o.y * (*this)(0, 1) + o.z * (*this)(0, 2);
			(*this)(1, 3) += o.x * (*this)(1, 0) + o.y * (*this)(1, 1) + o.z * (*this)(1, 2);
			(*this)(2, 3) += o.x * (*this)(2, 0) + o.y * (*this)(2, 1) + o.z * (*this)(2, 2);
			return *this;
		}

		Matrix34<TFLOAT> operator*(const Translation3<TFLOAT>& o) const
		{
			Matrix34<TFLOAT> mat(*this);
			mat *= o;
			return mat;
		}

		Vector3<TFLOAT> operator*(const Vector3<TFLOAT>& v) const
		{
			Vector3<TFLOAT> result;
			result.x = v.x*(*this)(0, 0) + v.y*(*this)(0, 1) + v.z*(*this)(0, 2) + (*this)(0, 3);
			result.y = v.x*(*this)(1, 0) + v.y*(*this)(1, 1) + v.z*(*this)(1, 2) + (*this)(1, 3);
			result.z = v.x*(*this)(2, 0) + v.y*(*this)(2, 1) + v.z*(*this)(2, 2) + (*this)(2, 3);
			return result;
		}

	};

	template<typename TFLOAT>
	class Matrix44
	{
	private:
		TFLOAT m[4][4];

	public:
		class TCol
		{
		public:
			inline TCol& operator=(const Vector4<TFLOAT>& newcol){
				mat(0, col) = newcol.x;
				mat(1, col) = newcol.y;
				mat(2, col) = newcol.z;
				mat(3, col) = newcol.w;
				return *this;
			}
			inline TCol& operator=(const Vector3<TFLOAT>& newcol){
				mat(0, col) = newcol.x;
				mat(1, col) = newcol.y;
				mat(2, col) = newcol.z;
				return *this;
			}

			inline TCol& operator=(const Translation3<TFLOAT>& newcol){
				mat(0, col) = newcol.x;
				mat(1, col) = newcol.y;
				mat(2, col) = newcol.z;
				return *this;
			}
		public:
			TCol(Matrix44& _mat, int _col) : mat(_mat), col(_col) {}
			Matrix44& mat;
			int col;
		};
		inline typename Matrix44::TCol col(int j)
		{
			return TCol(*this, j);
		}

	public:
		Matrix44(){
			// optionally make identity
			memset(&m, 0, sizeof(m));
			m[0][0] = 1;
			m[1][1] = 1;
			m[2][2] = 1;
			m[3][3] = 1;
		}

		Matrix44(const Translation3<TFLOAT>& t) : Matrix44() {
			col(3) = t;
		}

		// wastes last row - use Matrix34 instead
		template<class G>
		Matrix44(const Quaternion<G>& quat){
			TFLOAT wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

			// calculate coefficients
			x2 = quat.x + quat.x; y2 = quat.y + quat.y;
			z2 = quat.z + quat.z;
			xx = quat.x * x2; xy = quat.x * y2; xz = quat.x * z2;
			yy = quat.y * y2; yz = quat.y * z2; zz = quat.z * z2;
			wx = quat.w * x2; wy = quat.w * y2; wz = quat.w * z2;


			(*this)(0, 0) = (TFLOAT)(1.0 - (yy + zz));
			(*this)(0, 1) = (TFLOAT)(xy - wz);
			(*this)(0, 2) = xz + wy;
			(*this)(0, 3) = (TFLOAT)0.0;

			(*this)(1, 0) = xy + wz;
			(*this)(1, 1) = (TFLOAT)1.0 - (xx + zz);
			(*this)(1, 2) = yz - wx;
			(*this)(1, 3) = (TFLOAT)0.0;


			(*this)(2, 0) = xz - wy;
			(*this)(2, 1) = yz + wx;
			(*this)(2, 2) = (TFLOAT)1.0 - (xx + yy);
			(*this)(2, 3) = (TFLOAT)0.0;


			(*this)(3, 0) = (TFLOAT)0;
			(*this)(3, 1) = (TFLOAT)0;
			(*this)(3, 2) = (TFLOAT)0;
			(*this)(3, 3) = (TFLOAT)1;
		}

		// wastes last row - use Matrix34 instead
		Matrix44(const Translation3<TFLOAT>& pos, const Quaternion<TFLOAT>& rot) : Matrix44(rot){
			col(3) = pos;
		}

		TFLOAT& operator() (int i, int j) {
			return m[i][j]; // row - major
		}


		const TFLOAT& operator() (int i, int j) const {
			return m[i][j];	// row-major
		}

		Matrix44<TFLOAT> operator* (const Matrix44<TFLOAT>& o) const {
			Matrix44<TFLOAT> result;
			memset(&result, 0, sizeof(result));

			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					for (int k = 0; k < 4; ++k)
					{
						result.m[i][j] += m[i][k] * o.m[k][j];
					}
			return result;
		}

		static Matrix44<TFLOAT> Identity()
		{
			Matrix44<TFLOAT> result;
			memset(&result, 0, sizeof(result));
			result.m[0][0] = 1;
			result.m[1][1] = 1;
			result.m[2][2] = 1;
			result.m[3][3] = 1;
			return result;
		}

		// http://www.songho.ca/opengl/gl_projectionmatrix.html
		static Matrix44<TFLOAT> ProjectionRH(float tanHalfX, float tanHalfY, float near, float far){
			Matrix44<TFLOAT> result;
			memset(&result, 0, sizeof(result));
			result(0, 0) = 1.0f / tanHalfX;
			result(1, 1) = 1.0f / tanHalfY;
			result(2, 2) = -(far + near) / (far - near);
			result(2, 3) = -2.0f * far*near / (far - near);
			result(3, 2) = -1.0f;
			return result;
		}
		static Matrix44<TFLOAT> ProjectionLH(float tanHalfX, float tanHalfY, float near, float far){
			Matrix44<TFLOAT> result;
			memset(&result, 0, sizeof(result));
			result(0, 0) = 1.0f / tanHalfX;
			result(1, 1) = 1.0f / tanHalfY;
			result(2, 2) = (far + near) / (far - near);
			result(2, 3) = -2.0f * far*near / (far - near);
			result(3, 2) = 1.0f;
			return result;
		}

	};



	template<class F>
	inline Matrix34<F> operator*(const Translation3<F>& t, const Matrix34<F>& m){
		Matrix34<F> r(m);
		r.col(3) += t;
		return r;
	}
	
	
	template<class F, class G>
	inline Vector3<decltype(F()*G())>& operator*=(Vector3<F>& v, const Matrix34<G>& m){
		return v = m*v;
	}





	typedef Matrix44<float>  mat44f;
	typedef Matrix44<double> mat44d;

	typedef Matrix34<float>  mat34f;
	typedef Matrix34<double> mat34d;


}





