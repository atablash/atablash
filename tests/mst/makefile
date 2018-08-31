all:
	g++ -Wall -O2 -o mst_kruskal mst_kruskal.cpp `pkg-config --cflags --libs atablash`
	g++ -Wall -O2 -o mst_cheriton_planar mst_cheriton_planar.cpp `pkg-config --cflags --libs atablash`
	g++ -Wall -O2 -o mst_kruskal_counting mst_kruskal_counting.cpp `pkg-config --cflags --libs atablash`
	g++ -Wall -O2 -o testgen -Wno-deprecated testgen.cpp
	g++ -Wall -O2 -o testgen_planar testgen_planar.cpp

clean:
	rm mst_kruskal.exe
	rm mst_cheriton_planar.exe
	rm testgen.exe
	rm testgen_planar.exe