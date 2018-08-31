#pragma once

namespace ab{

template<typename TFLOAT>
class Vector4
{
public:
	TFLOAT x = 0;
	TFLOAT y = 0;
	TFLOAT z = 0;
	TFLOAT w = 0;

public:
	Vector4() = default;
	Vector4(TFLOAT _x, TFLOAT _y, TFLOAT _z, TFLOAT _w) : x(_x), y(_y), z(_z), w(_w) {}

	uint size() const { return 4; }

	void Normalize()
	{
		TFLOAT leninv = 1.0 / sqrt(x*x + y*y + z*z + w*w);
		x *= leninv;
		y *= leninv;
		z *= leninv;
		w *= leninv;
	}

	TFLOAT Length() const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}


	Vector4& operator+=(const Vector4& o)
	{
		x += o.x;
		y += o.y;
		z += o.z;
		w += o.w;
		return *this;
	}
	Vector4 operator+(const Vector4& o) const
	{
		return Vector4(x + o.x, y + o.y, z + o.z, w + o.w);
	}
	Vector4& operator-=(const Vector4& o)
	{
		x -= o.x;
		y -= o.y;
		z -= o.z;
		w -= o.w;
		return *this;
	}
	Vector4 operator-(const Vector4& o) const
	{
		return Vector4(x - o.x, y - o.y, z - o.z, w - o.w);
	}
	TFLOAT operator*(const Vector4& o) const
	{
		return x*o.x + y*o.y + z*o.z + w*o.w;
	}
};




// abbrev
typedef ab::Vector4<float>  v4f;
typedef ab::Vector4<double> v4d;



}



