#pragma once

#include<atablash/graph/spantree.hpp>
#include<atablash/functional.hpp>
#include<atablash/math.hpp>

#include<cmath>
#include<climits>


namespace ab
{
	template<typename FLOAT = float>
	struct _yangGraph
	{
		struct V
		{
			unsigned char numDests;
			int dests[4];
			int weights[4];

			FLOAT cost;
			FLOAT spar;		// S(this,parent)
			FLOAT sparx;	// 1 - S^2(this,parent)

			FLOAT bestAggCost;
			short bestDisparity;

			short disparity;

			FLOAT bestAggCost2nd;	// optional
		};

		struct E
		{
			int from;
			int to;

			unsigned char weight;
			unsigned char inMST;
		};

		// buduje graf z obrazka i znajduje MST
		template<typename Image>
		_yangGraph(const Image& im, FLOAT sigma)
		{
			int w = im.getWidth();
			int h = im.getHeight();

			vertices.resize(w*h);
			edges.resize(w*(h-1) + h*(w-1));
			Vector<E> tedges(edges.size());

			int numChannels = im.getNumChannels();

			int cEdge = 0;

			// poziome
			for(int y=0; y<h; ++y)
			for(int x=0; x<w-1; ++x)
			{
				unsigned char diff = 0;
				for(int i=0; i<numChannels; ++i)
				{
					unsigned char a = im.pixel(x,y,i);
					unsigned char b = im.pixel(x+1,y,i);
					//diff = (unsigned char)std::max(diff,(unsigned char)abs(a-b));
					diff += abs(a-b);
				}
				tedges[cEdge].inMST = false;
				tedges[cEdge].from = y*w + x;
				tedges[cEdge].to   = y*w + x+1;
				tedges[cEdge++].weight = diff;
			}

			// pionowe
			for(int y=0; y<h-1; ++y)
			for(int x=0; x<w; ++x)
			{
				unsigned char diff = 0;
				for(int i=0; i<numChannels; ++i)
				{
					unsigned char a = im.pixel(x,y,i);
					unsigned char b = im.pixel(x,y+1,i);
					//diff = std::max(diff,(unsigned char)abs(a-b));
					diff += abs(a-b);
				}
				tedges[cEdge].inMST = false;
				tedges[cEdge].from = y*w + x;
				tedges[cEdge].to   = (y+1)*w + x;
				tedges[cEdge++].weight = diff;
			}
			// koniec budowy

			// find MST
			countingSort(&tedges[0], &tedges[0] + tedges.size(), &edges[0], 256, bind(&E::weight));
			kruskalSorted(*this, bind(&E::inMST), bind(&E::weight));
			//boruvkaCleanup(*this, bind(&E::inMST), bind(&E::weight), false);

			for(unsigned int i=0; i<vertices.size(); ++i)
			{
				vertices[i].numDests = 0;
				vertices[i].bestAggCost = FLT_MAX;
			}

			// build VEL
			for(unsigned int i=0; i<edges.size(); ++i)
			{
				if(!edges[i].inMST) continue;

				int a = edges[i].from;
				int b = edges[i].to;

				vertices[a].weights[vertices[a].numDests] = edges[i].weight;
				vertices[a].dests[vertices[a].numDests++] = b;
				vertices[b].weights[vertices[b].numDests] = edges[i].weight;
				vertices[b].dests[vertices[b].numDests++] = a;
			}

			// root the tree & topo sort
			topoSorted.resize(vertices.size());
			int whereTo = 0;
			rootTree(0,whereTo,-1);
			ASS(whereTo == (int)vertices.size());

			FLOAT s = sigma*255.0f;

			// precompute spar & sparx
			for(unsigned int i=1; i<vertices.size(); ++i)
			{
				int ww = vertices[i].weights[0];
				vertices[i].spar  = exp(-ww/s);
				vertices[i].sparx = 1 - vertices[i].spar*vertices[i].spar;
			}
		}

		void rootTree(int cRoot, int& whereTo, int from)
		{
			topoSorted[whereTo++] = cRoot;
			for(int i=0; i<vertices[cRoot].numDests; ++i)
			{
				int& dest = vertices[cRoot].dests[i];
				if(dest == from)
				{
					swap(dest, vertices[cRoot].dests[0]);
					swap(vertices[cRoot].weights[0], vertices[cRoot].weights[i]);
					continue;
				}

				rootTree(dest, whereTo, cRoot);
			}
		}

		void aggregate(int level)
		{
			//printf("a %f\n", vertices[0].cost);
			// aggregate the costs up
			for(int i=vertices.size()-1; i>=1; --i)
			{
				int idx = topoSorted[i];
				int par = vertices[idx].dests[0];
				vertices[par].cost += vertices[idx].cost * vertices[idx].spar;
			}
			//printf("b %f\n", vertices[0].cost);

			//// select the best one for vertex 0
			if(vertices[0].cost < vertices[0].bestAggCost)
			{
				vertices[0].bestAggCost2nd = vertices[0].bestAggCost;	// optional

				vertices[0].bestAggCost = vertices[0].cost;
				vertices[0].bestDisparity = level;
			}
			////

			// aggregate the costs down
			for(unsigned int i=1; i<vertices.size(); ++i)
			{
				int idx = topoSorted[i];
				int par = vertices[idx].dests[0];
				vertices[idx].cost = vertices[idx].spar * vertices[par].cost  +  vertices[idx].sparx * vertices[idx].cost;

				// select the best one
				if(vertices[idx].cost <= vertices[idx].bestAggCost)
				{
					vertices[idx].bestAggCost2nd = vertices[idx].bestAggCost;	// optional

					vertices[idx].bestAggCost = vertices[idx].cost;
					vertices[idx].bestDisparity = level;
				}
			}
		}

		Vector<V> vertices;
		Vector<E> edges;

		Vector<int> topoSorted;
	};


	const float max_color_difference=7;
	const float max_gradient_difference=2;

	// based on paper by:
	// Qingxiong Yang
	// City University of Hong Kong
	template<typename FLOAT, typename RGBImage, typename DisparityImage>
	void stereoMatchYang(DisparityImage& resultImage, const RGBImage& inLeft, const RGBImage& inRight, const int maxDisparity = 50, const FLOAT& sigma = 0.2, const FLOAT& epsilon = 1.0)
	{
		ASSERT(inLeft.getWidth()  == inRight.getWidth());
		ASSERT(inLeft.getHeight() == inRight.getHeight());
		ASSERT(inLeft.getNumChannels() == inRight.getNumChannels());

		int w = inLeft.getWidth();
		int h = inLeft.getHeight();
		int numChannels = inLeft.getNumChannels();

		printf("building graphs...\n");
		_yangGraph<FLOAT> left(inLeft,sigma);
		_yangGraph<FLOAT> right(inRight,sigma);
		printf("...done building graphs.\n");

		for(int level = -maxDisparity; level <= maxDisparity; ++level)	// left(x,y) == right(x-level,y) ?
		{
			//printf("level == %d\n",level);
			// base cost
			int xfrom = std::max(0,  level);
			int xto   = std::min(w,w+level)-1;
			for(int y=0; y<h; ++y)
			{
				for(int x=xto+1; x<w; ++x)	// piksele na brzegach obrazków
				{
					left.vertices[y*w + x].cost = 255;					// 255 czy 0 ??
					right.vertices[y*w + x - level - w].cost = 255;
				}
				for(int x=0; x<xfrom; ++x)	// piksele na brzegach obrazków
				{
					left.vertices[y*w + x].cost = 255;
					right.vertices[y*w + x - level + w].cost = 255;
				}

				for(int x=xfrom; x<=xto; ++x)
				{
					unsigned short diff = 0;
					for(int i=0; i<numChannels; ++i)
					{
						unsigned char a = inLeft.pixel(x,y,i);
						unsigned char b = inRight.pixel(x-level,y,i);
						diff += abs(a-b);
					}
					float fdiff = 0.33333333333 * diff;
					fdiff =  /* x<maxDisparity||w-x<maxDisparity? 255 : */ std::min(fdiff, max_color_difference);

					// TODO: dodac gradient

					left.vertices[y*w + x].cost = fdiff;
					right.vertices[y*w + x-level].cost = fdiff;
				}
			}

			//printf("start aggregation...\n");
			left.aggregate(level);
			right.aggregate(level);
		}

		int numStable = 0;

		// check for stability
		for(int y=0; y<h; ++y)
		for(int x=0; x<w; ++x)
		{
			if(left.vertices[y*w + x].bestAggCost2nd - left.vertices[y*w + x].bestAggCost >= epsilon)		// mamy 2 podobne koszty dla ró¿nych disparity?
			{
				int disp = left.vertices[y*w + x].bestDisparity;
				if(x-disp >= 0 && x-disp < w && right.vertices[y*w + x-disp].bestDisparity == disp)	// match!
				{
					if(right.vertices[y*w + x-disp].bestAggCost2nd - right.vertices[y*w + x-disp].bestAggCost >= epsilon)		// mamy 2 podobne koszty dla ró¿nych disparity?
					{
						++numStable;
						//resultImage.pixel(x,y,1) = 255;	////

						left.vertices[y*w + x].disparity = disp;

						left.vertices[y*w + x].bestAggCost = FLT_MAX;
						continue;
					}
				}
			}

			// piksel nie jest stabilny
			//left.vertices[y*w + x].disparity = left.vertices[y*w + x].bestDisparity;
			left.vertices[y*w + x].disparity = SHRT_MAX;

			left.vertices[y*w + x].bestAggCost = FLT_MAX;
		}

		printf("Found %d stable pixels\n", numStable);

		// hack
		//left.vertices[0*w + 0].disparity = -maxDisparity;
		//left.vertices[0*w + w-1].disparity = -maxDisparity;


		// non-local refinement

		for(int level = -maxDisparity; level <= maxDisparity; ++level)
		{
			for(int y=0; y<h; ++y)
			for(int x=0; x<w; ++x)
			{
				left.vertices[y*w + x].cost = left.vertices[y*w + x].disparity==SHRT_MAX ? 0 : abs(level - left.vertices[y*w + x].disparity);
			}

			left.aggregate(level);
		}


		for(int y=0; y<h; ++y)
		for(int x=0; x<w; ++x)
		{
			ASSERT(left.vertices[y*w + x].bestDisparity >= -maxDisparity && left.vertices[y*w + x].bestDisparity <= maxDisparity);

			//resultImage.pixel(x,y,0) = left.vertices[y*w + x].disparity != SHRT_MAX ? 128 + left.vertices[y*w + x].disparity : 0;

			resultImage.pixel(x,y,0) = 128 + left.vertices[y*w + x].bestDisparity;
		}
	}

}
