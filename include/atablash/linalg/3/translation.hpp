#pragma once


#include "vector.hpp"

namespace ab{

template<typename F>
class Translation3
{
public:
	F x = 0;
	F y = 0;
	F z = 0;

public:
	Translation3() {}
	Translation3(F _x, F _y, F _z) : x(_x), y(_y), z(_z){
		ASS(FLOAT_OK(x));
		ASS(FLOAT_OK(y));
		ASS(FLOAT_OK(z));
	}

	Translation3(const Translation3& o) : x(o.x), y(o.y), z(o.z){
		ASS(FLOAT_OK(x));
		ASS(FLOAT_OK(y));
		ASS(FLOAT_OK(z));
	}

	// FLOAT cast
	template<typename T>
	Translation3(const Translation3<T>& o) : x((F)o.x), y((F)o.y), z((F)o.z) {}
	
	
	
	// Vector3 casts
	template<typename FF>
	explicit Translation3(const Vector3<FF>& vec) : x((F)vec.x), y((F)vec.y), z((F)vec.z){}
	
	template<typename FF>
	explicit operator Vector3<FF>(){
		return Vector3<FF>((FF)x,(FF)y,(FF)z);
	}
	
	

	F& operator[](unsigned int idx){
		ASS(0 <= idx && idx < 3);
		return *(&x + idx);
	}
	const F& operator[](unsigned int idx) const
	{
		ASS(0 <= idx && idx < 3);
		return *(&x + idx);
	}

	bool operator==(const Translation3& o)const{
		return x == o.x && y == o.y && z == o.z;
	}

	bool operator!=(const Translation3& o)const{
		return !(*this == o);
	}

	bool operator<(const Translation3& o)const{
		return x < o.x && y < o.y && z < o.z;
	}
	bool operator<=(const Translation3& o)const{
		return x <= o.x && y <= o.y && z <= o.z;
	}

	// not really intuitive because multiplication is for applying transformations
	Translation3 operator-() const {
		return Translation3(-x, -y, -z);
	}

	Translation3& normalize(){
		F leninv = (F)1.0 / sqrt(x*x + y*y + z*z);
		x *= leninv;
		y *= leninv;
		z *= leninv;
		return *this;
	}

	Translation3 normalized() const
	{
		F leninv = (F)1.0 / sqrt(x*x + y*y + z*z);
		return Vector3(x*leninv, y*leninv, z*leninv);
	}

	F magnitude() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	F magnitudeSqr() const
	{
		return (x*x + y*y + z*z);
	}
	
	Translation3& operator+=(const Vector3<F>& o){
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}
	
	Translation3& operator-=(const Vector3<F>& o){
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}

	Translation3& operator+=(const Translation3& o){
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}
	
	Translation3& operator*=(const Translation3& o){
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}
	
	Translation3 operator+(const Translation3& o) const {
		return Translation3(x + o.x, y + o.y, z + o.z);
	}
	
	Translation3& operator-=(const Translation3& o) {
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}
	
	Translation3 operator-(const Translation3& o) const {
		return Translation3(x - o.x, y - o.y, z - o.z);
	}

	Translation3& operator*=(const F& a){
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	
	Translation3 operator*(const F& a) const{
		return Translation3(x*a, y*a, z*a);
	}
	
	Translation3& operator/=(const F& a){
		F f = 1.0f / a;
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	
	Translation3 operator/(const F& a) const{
		F f = 1.0f / a;
		return Translation3(x*f, y*f, z*f);
	}

	Translation3& RotateZ(const F& angle){
		F c = cos(angle);
		F s = sin(angle);
		F nx = c*x - s*y;
		F ny = s*x + c*y;
		x = nx;
		y = ny;
		return *this;
	}

	Translation3& RotateY(const F& angle){
		F c = cos(angle);
		F s = sin(angle);
		F nz = c*z - s*x;
		F nx = s*z + c*x;
		z = nz;
		x = nx;
		return *this;
	}

	Translation3& RotateX(const F& angle)
	{
		F c = cos(angle);
		F s = sin(angle);
		F ny = c*y - s*z;
		F nz = s*y + c*z;
		y = ny;
		z = nz;
		return *this;
	}
};



typedef Translation3<float>  tran3f;
typedef Translation3<double> tran3d;


}


