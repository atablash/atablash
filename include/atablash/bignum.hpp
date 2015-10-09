#pragma once


namespace ab{


// based on long double, int exponent
struct superdouble
{
	superdouble(const long double& f) : mantisa(f), pot(0) {}

	void fix()
	{
		if(mantisa==0)return;
		while(mantisa>65536)mantisa/=65536,++pot;
		while(mantisa<1.0/65536)mantisa*=65536,--pot;
	}

	superdouble& operator+=(const superdouble& o)
	{
		long double omant = o.mantisa;
		int opot = o.pot;
		while(pot>opot)omant/=65536,++opot;
		while(pot<opot)mantisa/=65536,++pot;
		mantisa += omant;
		return *this;
	}

	superdouble& operator-=(const superdouble& o)
	{
		long double omant = o.mantisa;
		int opot = o.pot;
		while(pot>opot)omant/=65536,++opot;
		while(pot<opot)mantisa/=65536,++pot;
		mantisa += omant;
		fix();
		return *this;
	}

	superdouble& operator*=(const superdouble& o)
	{
		mantisa*=o.mantisa;
		pot += o.pot;
		fix();
		return *this;
	}

	superdouble& operator/=(const superdouble& o)
	{
		mantisa/=o.mantisa;
		pot -= o.pot;
		fix();
		return *this;
	}

	superdouble operator*(const superdouble& o) const
	{
		superdouble s(*this);
		s*=o;
		return s;
	}

	operator long double() const
	{
		long double r = mantisa;
		int npot = pot;
		while(npot>0)r*=65536,--npot;
		while(npot<0)r/=65536,++npot;
		return r;
	}


	long double mantisa;
	int pot;
};

inline std::ostream& operator<<(std::ostream& o, const superdouble& s)
{
    o<<s.mantisa<<"   *   65536 ^ "<<s.pot;
    return o;
}

}
