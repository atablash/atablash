#pragma once

#include<atablash/debug.hpp>


namespace ab{


template<typename TFLOAT>
class Vector3
{
public:
	TFLOAT x = 0;
	TFLOAT y = 0;
	TFLOAT z = 0;

public:
	Vector3() {}
	Vector3(TFLOAT _x, TFLOAT _y, TFLOAT _z) : x(_x), y(_y), z(_z){
		//ASS(FLOAT_OK(x));
		//ASS(FLOAT_OK(y));
		//ASS(FLOAT_OK(z));
	}
	Vector3(const Vector3& o) : x(o.x), y(o.y), z(o.z){
		//ASS(FLOAT_OK(x));
		//ASS(FLOAT_OK(y));
		//ASS(FLOAT_OK(z));
	}

	uint size() const { return 3; }

	operator std::string()const{
		return "{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}";
	}

	// cast
	template<typename T>
	Vector3(const Vector3<T>& o) : x((TFLOAT)o.x), y((TFLOAT)o.y), z((TFLOAT)o.z) {}

	TFLOAT& operator[](unsigned int idx)
	{
		ASS(0 <= idx && idx < 3);
		return *(&x + idx);
	}
	const TFLOAT& operator[](unsigned int idx) const
	{
		ASS(0 <= idx && idx < 3);
		return *(&x + idx);
	}

	bool operator==(const Vector3& o)const{
		return x == o.x && y == o.y && z == o.z;
	}

	bool operator!=(const Vector3& o)const{
		return !(*this == o);
	}

	bool operator<(const Vector3& o)const{
		return x < o.x && y < o.y && z < o.z;
	}
	bool operator<=(const Vector3& o)const{
		return x <= o.x && y <= o.y && z <= o.z;
	}

	Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	void normalize(){
		TFLOAT leninv = (TFLOAT)1.0 / sqrt(x*x + y*y + z*z);
		x *= leninv;
		y *= leninv;
		z *= leninv;
	}

	Vector3 Normalized() const {
		TFLOAT leninv = (TFLOAT)1.0 / sqrt(x*x + y*y + z*z);
		return Vector3(x*leninv, y*leninv, z*leninv);
	}

	TFLOAT length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	TFLOAT lengthSqr() const
	{
		return (x*x + y*y + z*z);
	}

	TFLOAT taxi() const{
		return x + y + z;
	}

	Vector3& operator+=(const Vector3& o)
	{
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}
	Vector3 operator+(const Vector3& o) const
	{
		return Vector3(x + o.x, y + o.y, z + o.z);
	}
	Vector3& operator-=(const Vector3& o)
	{
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}
	Vector3 operator-(const Vector3& o) const
	{
		return Vector3(x - o.x, y - o.y, z - o.z);
	}

	// dot
	TFLOAT operator*(const Vector3& o) const
	{
		return x*o.x + y*o.y + z*o.z;
	}

	Vector3& operator%=(const Vector3& o)
	{
		x *= o.x;
		y *= o.y;
		z *= o.z;
		return *this;
	}

	Vector3 operator%(const Vector3& o)
	{
		Vector3 result(*this);
		result %= o;
		return result;
	}

	Vector3& operator*=(const TFLOAT& a)
	{
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	Vector3 operator*(const TFLOAT& a) const
	{
		return Vector3(x*a, y*a, z*a);
	}
	Vector3& operator/=(const TFLOAT& a)
	{
		TFLOAT f = 1.0f / a;
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	Vector3 operator/(const TFLOAT& a) const
	{
		TFLOAT f = 1.0f / a;
		return Vector3(x*f, y*f, z*f);
	}
	Vector3 operator^(const Vector3& o) const
	{
		return Vector3(y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x);
	}

	Vector3& RotateZ(const TFLOAT& angle){
		TFLOAT c = cos(angle);
		TFLOAT s = sin(angle);
		TFLOAT nx = c*x - s*y;
		TFLOAT ny = s*x + c*y;
		x = nx;
		y = ny;
		return *this;
	}

	Vector3& RotateY(const TFLOAT& angle){
		TFLOAT c = cos(angle);
		TFLOAT s = sin(angle);
		TFLOAT nz = c*z - s*x;
		TFLOAT nx = s*z + c*x;
		z = nz;
		x = nx;
		return *this;
	}

	Vector3& RotateX(const TFLOAT& angle)
	{
		TFLOAT c = cos(angle);
		TFLOAT s = sin(angle);
		TFLOAT ny = c*y - s*z;
		TFLOAT nz = s*y + c*z;
		y = ny;
		z = nz;
		return *this;
	}
};





// multiply by scalar
template<class S, class F>
inline Vector3<decltype(S()*F())> operator*(const S& a, const Vector3<F>& v){
	return Vector3<decltype(S()*F())>(a*v.x, a*v.y, a*v.z);
}




// abbrev
typedef ab::Vector3<float>  v3f;
typedef ab::Vector3<double> v3d;

typedef ab::Vector3<ui8>    v3b;
typedef ab::Vector3<ui8>    v3i;




/*
template<typename F>
inline bool isSame(const ab::Vector3<F>& a, const ab::Vector3<F>& b, const F eps)
{
	ab::Vector3<F> d = a - b;
	return fabs(d.x) < eps && fabs(d.y) < eps && fabs(d.z) < eps;
}
*/



} // namespace ab




// to_string
namespace std{
	template<class F>
	string to_string(const ab::Vector3<F>& v){
		return "{" + to_string(v.x) + ", " + to_string(v.y) + ", " + to_string(v.z) + "}";
	}
}



// hash Vector3
namespace std{
	/*template<>*/ template<class F>struct hash<::ab::Vector3<F> >{
		size_t operator()(const ::ab::Vector3<F>& v) const{
			return hash<F>()(v.x)
				^ ab::cbitl(hash<F>()(v.y), sizeof(size_t)* 8 / 3)
				^ ab::cbitl(hash<F>()(v.z), sizeof(size_t)* 8 / 3 * 2);
		}
	};
}





