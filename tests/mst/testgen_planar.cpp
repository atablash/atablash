#include<cstdio>
#include<ctime>


#include<algorithm>
using namespace std;

int main()
{
	srand(time(NULL));
	
	// robimy wielki grid bok*bok wierzcholkow
	int bok = 1000;

	// wypisz
	printf("%d %d\n",bok*bok,2*(bok-1)*bok);
	for(int y=0; y<bok; ++y)
	for(int x=0; x<bok-1; ++x)
	{
		printf("%d %d %d\n", x*bok+y, (x+1)*bok+y, rand()%999 + 1);
		printf("%d %d %d\n", y*bok+x, y*bok+x+1, rand()%999 + 1);
	}

	return 0;
}
