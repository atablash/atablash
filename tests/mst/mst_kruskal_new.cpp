
#include<atablash/graph/graph.h>
#include<atablash/graph/spantree.h>

#include<cstdio>

struct EExtra{
	EExtra(int _weight, int _wynik) : weight(_weight), wynik(_wynik) {}
	int weight;
	int wynik;
		bool operator<(const EExtra&o)const{
			return weight < o.weight;
		}
};

typedef ab::GraphU_EI<ab::VOID,EExtra> G;

int main()
{
	int n,m;
	scanf("%d%d",&n,&m);
	
	G g;
	g.vs.resize(n);
	g.es.reserve(m);
	
	for(int i=0; i<m; ++i)
	{
		int a,b,w;
		scanf("%d%d%d",&a,&b,&w);
		
		g.es.push_back(a,b,EExtra(w,0));
	}
	
	for(int i=0; i<10; ++i)
	ab::kruskal(g, &EExtra::weight, &EExtra::wynik);
	
	int val = 0;
	for(int i=0; i<m; ++i) if(g.es[i]->wynik) val += g.es[i]->weight;
	
	// drukuje wartosc minspan tree
	printf("%d\n",val);
	
	return 0;
}