#include<cstdio>
#include<ctime>

#include<unordered_set>
using namespace __gnu_cxx;

#include<algorithm>
using namespace std;


typedef pair<int,int> pii;

struct piicomp
{
	size_t operator()(const pii& p) const
	{
		return p.first^p.second;
	}
};

int main()
{
	srand(time(NULL));
	int n = 1000;
	int mtries = 1000000;

	unordered_set<pii,piicomp> hs;

	// upewnij siê, ¿e spójny
	for(int i=1; i<n; ++i)
	{
		int a = rand()%i;
		int b = i;
		hs.insert(make_pair(a,b));
	}

	for(int i=0; i<mtries; ++i)
	{
		int a = rand()%n;
		int b = rand()%n;
		if(a > b) swap(a,b);
		hs.insert(make_pair(a,b));
	}

	vector<pii> es(hs.begin(), hs.end());
	random_shuffle(es.begin(), es.end());

	// wypisz
	printf("%d %d\n",n,es.size());
	for(unsigned int i=0; i<es.size(); ++i)
	{
		int a = es[i].first;
		int b = es[i].second;
		if(rand()%2)swap(a,b);
		printf("%d %d %d\n", a, b, rand()%999 + 1);
	}

	return 0;
}
