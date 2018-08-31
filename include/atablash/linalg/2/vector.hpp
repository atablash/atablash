#pragma once

#include <atablash/debug.hpp>


namespace ab{




template<typename TFLOAT>
class Vector2
{
public:
	TFLOAT x, y;

public:
	Vector2() {}
	Vector2(TFLOAT _x, TFLOAT _y) : x(_x), y(_y)
	{
		//ASS(FLOAT_OK(x));
		//ASS(FLOAT_OK(y));
	}
	Vector2(const Vector2& o) : x(o.x), y(o.y)
	{
		//ASS(FLOAT_OK(x));
		//ASS(FLOAT_OK(y));
	}

	uint size() const { return 2; }

	template<typename T>
	explicit Vector2(const Vector2<T>& o) : x((TFLOAT)o.x), y((TFLOAT)o.y){
	}

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

	bool operator==(const Vector2& o) const
	{
		return x == o.x && y == o.y;
	}

	bool operator!=(const Vector2& o) const
	{
		return !(*this == o);
	}

	Vector2 operator-()
	{
		return Vector2(-x, -y);
	}

	void normalize()
	{
		TFLOAT leninv = 1.0 / sqrt(x*x + y*y);
		x *= leninv;
		y *= leninv;
	}

	TFLOAT length() const
	{
		return sqrt(x*x + y*y);
	}

	TFLOAT lengthSqr() const
	{
		return (x*x + y*y);
	}

	Vector2& operator+=(const Vector2& o)
	{
		x += o.x;
		y += o.y;
		return *this;
	}
	Vector2 operator+(const Vector2& o) const
	{
		return Vector2(x + o.x, y + o.y);
	}
	Vector2& operator-=(const Vector2& o)
	{
		x -= o.x;
		y -= o.y;
		return *this;
	}
	Vector2 operator-(const Vector2& o) const
	{
		return Vector2(x - o.x, y - o.y);
	}
	TFLOAT operator*(const Vector2& o) const
	{
		return x*o.x + y*o.y;
	}
	Vector2& operator*=(const TFLOAT& a)
	{
		x *= a;
		y *= a;
		return *this;
	}
	Vector2 operator*(const TFLOAT& a) const
	{
		return Vector2(x*a, y*a);
	}
	Vector2& operator/=(const TFLOAT& a)
	{
		TFLOAT f = 1.0f / a;
		x *= f;
		y *= f;
		return *this;
	}
	Vector2 operator/(const TFLOAT& a) const
	{
		TFLOAT f = 1.0f / a;
		return Vector2(x*f, y*f);
	}

	Vector2& rotate(const TFLOAT& angle)
	{
		TFLOAT c = cos(angle);
		TFLOAT s = sin(angle);
		TFLOAT nx = c*x - s*y;
		TFLOAT ny = s*x + c*y;
		x = nx;
		y = ny;
		return *this;
	}

	// niech nic nie zwraca, �eby si� nie myli�o
	void ewMultBy(const Vector2& o){
		x *= o.x;
		y *= o.y;
	}
};

template<class T>
inline Vector2<T> ewMult(const Vector2<T>& a, const Vector2<T>& b){
	Vector2<T> r(a);
	r.ewMultBy(b);
	return r;
}



// determinant |a b|
template<class F>
inline F det(const Vector2<F>& a, const Vector2<F>& b){
	return a.x * b.y - a.y * b.x;
}


// multiply by scalar
template<class F>
inline Vector2<F> operator*(const F& a, const Vector2<F>& v){
	return Vector2<F>(a*v.x, a*v.y);
}



// abbrevs
typedef ab::Vector2<float>  v2f;
typedef ab::Vector2<double> v2d;
typedef ab::Vector2<int>    v2i;


} // namespace ab




// hash Vector2
namespace std{
	/*template<>*/ template<class F>struct hash<::ab::Vector2<F> >{
		size_t operator()(const ::ab::Vector2<F>& v) const{
			return hash<F>()(v.x) ^ ab::cbitl(hash<F>()(v.y), sizeof(size_t)* 8 / 2);
		}
	};
}


// to_string
namespace std{
	template<class F>
	string to_string(const ab::Vector2<F>& v){
		return "{" + to_string(v.x) + ", " + to_string(v.y) + "}";
	}
}




