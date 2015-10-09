#pragma once

#include "vector.hpp"

namespace ab{

template<typename F>
class Translation2
{
public:
	Translation2() {}
	Translation2(F _x, F _y) : x(_x), y(_y) {}
	Translation2(const Vector2<F>& v) : x(v.x), y(v.y) {}

	Translation2 operator*(const Translation2& o) const
	{
		Translation2 result(*this);
		result *= o;
		return result;
	}

	Translation2& operator*=(const Translation2& o)
	{
		x += o.x;
		y += o.y;
		return *this;
	}

	Translation2 inverse() const
	{
		Translation2 result(*this);
		result.invert();
		return result;
	}

	Translation2& invert()
	{
		x = -x;
		y = -y;
		return *this;
	}

	static Translation2 identity()
	{
		return Translation2(0, 0);
	}

public:
	F x = 0;
	F y = 0;
};


}
