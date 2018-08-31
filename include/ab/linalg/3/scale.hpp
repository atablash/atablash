#pragma once

#include "vector.hpp"


namespace ab{

template<typename F>
class Scale3
{
public:
	Scale3() {}
	Scale3(F _x, F _y, F _z) : x(_x), y(_y), z(_z) {}
	explicit Scale3(const Vector3<F>& v) : x(v.x), y(v.y), z(v.z) {}

	//Matrix34<F> operator*(const Matrix34<F>& m)
	//{}

	Scale3 operator*(const Scale3& o) const
	{
		Scale3 result(*this);
		result *= o;
		return result;
	}

	Vector3<F> operator*(const Vector3<F>& v) const
	{
		return Vector3<F>(v.x*x, v.y*y, v.z*z);
	}

	Scale3& operator*=(const Scale3& o)
	{
		x *= o.x;
		y *= o.y;
		z *= o.z;
		return *this;
	}

	Scale3 inverse() const
	{
		Scale3 result(*this);
		result.invert();
		return result;
	}

	Scale3& invert()
	{
		x = (F)1.0/x;
		y = (F)1.0/y;
		z = (F)1.0/z;
		return *this;
	}

	explicit operator Vector3<F>() const
	{
		return Vector3<F>(x, y, z);
	}

public:
	F x = 0;
	F y = 0;
	F z = 0;
};



}
