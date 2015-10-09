#include<atablash/spantree.h>
#include<atablash/vector.h>
#include<atablash/functional.h>

#include<cstdio>

struct Graph
{
	struct Vertices
	{
		int num;
		inline int size()
		{
			return num;
		}
	};
	
	struct E
	{
		int from, to;
		int waga;
		int wynik;
	};
	
	Vertices vertices;
	ab::Vector<E> edges;
};

int main()
{
	int n,m;
	scanf("%d%d",&n,&m);
	
	Graph g;
	g.vertices.num = n;
	g.edges.resize(m);
	
	ab::Vector<Graph::E> tempEdges(m);
	
	for(int i=0; i<m; ++i)
	{
		int a,b,w;
		scanf("%d%d%d",&a,&b,&w);
		
		tempEdges[i].from = a;
		tempEdges[i].to   = b;
		tempEdges[i].waga = w;
		tempEdges[i].wynik = 0;
	}
	
	for(int i=0; i<10; ++i)
	// z testu wynika, ¿e bind nie jest wolniejszy od wskaznikow
	{
		ab::countingSort(&tempEdges[0], &tempEdges[0] + m, &g.edges[0], 1000, ab::bind(&Graph::E::waga));
		ab::kruskalSorted(g, ab::bind(&Graph::E::wynik), ab::bind(&Graph::E::waga) /*&Graph::E::wynik, &Graph::E::waga*/);
	}
	int val = 0;
	for(int i=0; i<m; ++i) if(g.edges[i].wynik) val += g.edges[i].waga; //printf("%d\n",i);
	
	// drukuje wartosc minspan tree
	printf("%d\n",val);
	
	return 0;
}