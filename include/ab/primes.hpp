#pragma once

#include<atablash/types.hpp>
#include<atablash/math.hpp>
#include<atablash/hash.hpp>


namespace ab{

using namespace std;

// fills a table with smallest divisors
// sito[i] to wartosc dla 2*i+1
// BUGGED: try toNumber=100; 11 is duplicated
template<typename T>
void eratostenesDiv(vector<T>& sito, T toNumber, vector<T>& primes)
{
	T sitoSize = toNumber/2;
	sito.resize(sitoSize);
	for(int i=0; i<sito.size(); ++i) sito[i] = 2*i + 1;

	primes.clear();

	int s = sqrt(toNumber);

	primes.push_back(2);	// optional 2

	// +3 causes the bug
	for(int i=3; i<s+3; i+=2)
	{
		if(sito[i/2] != i) continue;	// nie pierwsza

		primes.push_back(i);

		int c = i*i;
		for(;;)
		{
			if(c/2 >= sito.size()) break;

			if(sito[c/2] > i) sito[c/2] = i;
			c += 2*i;
		}
	}

	for(int i=s|1; i<sito.size()*2; i+=2)
	{
		if(sito[i/2] != i) continue;

		primes.push_back(i);
	}
}


inline void getPrimeDivisors(vector<pair<ull,unsigned char> >& out,
	ull number,
	const vector<uint>& sito,
	const vector<uint>& primes)
{
	out.clear();
	unsigned int maxsito = sito.size()*2 - 1;
	uint cdivisor = 0;
	int iprime = 1;
	while((number&1)==0)
	{
		number >>= 1;
		if(cdivisor==2) ++out.back().second;
		else
		{
			cdivisor = 2;
			out.push_back(make_pair(2,1));
		}
	}
	while(number > maxsito)
	{
		uint prime = primes[iprime];
		if((ull)prime*prime > number)
		{
			out.push_back(make_pair(number,1));
			return;
		}
		if(number % prime == 0)
		{
			number /= prime;
			if(prime==cdivisor) ++out.back().second;
			else
			{
				out.push_back(make_pair(prime,1));
				cdivisor = prime;
			}
		}
		else ++iprime;
	}

	ui32 inumber = (ui32)number;

	while (inumber > 1)
	{
		uint div = sito[inumber >> 1];
		inumber /= div;
		if(div==cdivisor) ++out.back().second;
		else
		{
			out.push_back(make_pair(div,1));
			cdivisor = div;
		}
	}
}

inline void getAllDivisors(vector<ull>& out,
	const vector<pair<ull, unsigned char> >& primeDivs)
{
	out.clear();
	out.push_back(1);
	for(uint i=0; i<primeDivs.size(); ++i)
	{
		uint psize = out.size();
		ull mult = 1;
		for(int k=0; k<primeDivs[i].second; ++k)
		{
			mult *= primeDivs[i].first;
			for(uint j=0; j<psize; ++j)
			{
				out.push_back(out[j]*mult);
			}
		}
	}
}




// p_max == 4,759,123,141 ( > 2^32)
inline bool millerRabin(const unsigned int p){
	if((p&1) == 0) return false;
	if(p < 2) return false;

	unsigned int d = p-1;
	int s = 0;
	while((d&1)==0)
	{
		++s;
		d>>=1;
	}

	const unsigned int tt[] = {2,7,61};

	for(int i=0; i<3; ++i)
	{
		if(p <= tt[i])continue;

		H1X a(p, tt[i]); // mod p
		//int a = rand()%(p-2) + 2;
		H1X x = fpow(a,d);
		if(x==1 || x==p-1)continue;

		int j;
		for(j=0; j<s-1; ++j)
		{
			x *= x; // mod p
			if(x==1) return false;
			if(x==p-1)break;
		}
		if(j==s-1)return false;
	}

	return true;
}

// p < 2^(64-1)
// uses slow "fast multiplication"
/* TODO: napisaæ mult (u¿yæ fpow?)
bool millerRabin(ull p, int numtests = 10)
{
	if((p&1) == 0) return false;
	if(p < 2) return false;

	ull d = p-1;
	int s = 0;
	while((d&1)==0)
	{
		++s;
		d>>=1;
	}

	for(int i=0; i<numtests; ++i)
	{
		int a = rand()%(p-2) + 2;
		ull x = fpow(a,d,p);
		if(x==1 || x==p-1)continue;

		int j;
		for(j=0; j<s-1; ++j)
		{
			x = mult(x,x,p);
			if(x==1) return false;
			if(x==p-1)break;
		}
		if(j==s-1)return false;
	}

	return true;
}*/


}

