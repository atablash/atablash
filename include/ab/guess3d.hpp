#pragma once

#include<cstring>
#include<cstdlib>
#include<cstdio>

#include<queue>

namespace ab
{
	
	struct g3dpqnode
	{
		g3dpqnode(unsigned long long _dist, int _x, int _y) : dist(_dist), x(_x), y(_y) {}
		unsigned long long dist;
		int x,y;
		
		bool operator<(const g3dpqnode& o) const
		{
			return dist > o.dist;	// na odwrot!
		}
	};
	
	template<typename Image>
	int getw(const Image& image, int x0, int y0, int x1, int y1)
	{
		int r = 0;
		for(int i=0; i<image.getNumChannels(); ++i)
		{
			r += abs(image.pixel(x0,y0,i) - image.pixel(x1,y1,i));
			//r = std::max(r,abs(image.pixel(x0,y0,i) - image.pixel(x1,y1,i)));
		}
		return r;
	}
	
	template<typename TDEST, typename TSRC>
	void boxBlurX(TDEST* dest, TSRC* img, int w, int h, int size)
	{
		for(int y=0; y<h; ++y)
		{
			int num = 0;
			TSRC sum = 0;
			for(int x=0; x<size; ++x)
			{
				sum += img[y*w + x];
				++num;
			}
			for(int x=0; x<=size; ++x)
			{
				sum += img[y*w + x+size];
				++num;
				dest[y*w + x] = (TDEST)sum/num;
			}
			for(int x=size+1; x<w-size; ++x)
			{
				sum -= img[y*w + x-size-1];
				sum += img[y*w + x+size];
				dest[y*w + x] = (TDEST)sum/num;
			}
			for(int x=w-size; x<w; ++x)
			{
				sum -= img[y*w + x-size-1];
				--num;
				dest[y*w + x] = (TDEST)sum/num;
			}
		}
	}
	
	/*
	template<typename TSRC, typename TMAP>
	void boxBlurXHoles(TSRC* img, TMAP* imgmap, int w, int h, int size)
	{
		for(int y=0; y<h; ++y)
		{
			int num = 0;
			TSRC sum = 0;
			for(int x=0; x<size; ++x)
			{
				if(imgmap[y*w + x])
				{
					sum += img[y*w + x];
					++num;
				}
			}
			for(int x=0; x<=size; ++x)
			{
				if(imgmap[y*w + x+size])
				{
					sum += img[y*w + x+size];
					++num;
				}
				
				TMAP& im = imgmap[y*w + x];
				if(!im && num)
				{
					TSRC val = (TSRC)sum/num;
					img[y*w + x] = val;
					//im=true;
					//sum += val;
					//++num;
				}
			}
			for(int x=size+1; x<w-size; ++x)
			{
				if(imgmap[y*w + x-size-1])
				{
					sum -= img[y*w + x-size-1];
					--num;
				}
				if(imgmap[y*w + x+size])
				{
					sum += img[y*w + x+size];
					++num;
				}
				TMAP& im = imgmap[y*w + x];
				if(!im && num)
				{
					TSRC val = (TSRC)sum/num;
					img[y*w + x] = val;
					//im = true;
					//sum += val;
					//++num;
				}
			}
			for(int x=w-size; x<w; ++x)
			{
				if(imgmap[y*w + x-size-1])
				{
					sum -= img[y*w + x-size-1];
					--num;
				}
				TMAP& im = imgmap[y*w + x];
				if(!im && num)
				{
					TSRC val = (TSRC)sum/num;
					img[y*w + x] = val;
					//im = true;
					//sum += val;
					//++num;
				}
			}
			//printf("num=%d\n",num);
		}
	}
	*/
	
	template<typename TDEST, typename TSRC>
	void boxBlurY(TDEST* dest, TSRC* img, int w, int h, int size)
	{
		for(int x=0; x<w; ++x)
		{
			int num = 0;
			TSRC sum = 0;
			for(int y=0; y<size; ++y)
			{
				sum += img[y*w + x];
				++num;
			}
			for(int y=0; y<=size; ++y)
			{
				sum += img[(y+size)*w + x];
				++num;
				dest[y*w + x] = (TDEST)sum/num;
			}
			for(int y=size+1; y<h-size; ++y)
			{
				sum -= img[(y-size-1)*w + x];
				sum += img[(y+size)*w + x];
				dest[y*w + x] = (TDEST)sum/num;
			}
			for(int y=h-size; y<h; ++y)
			{
				sum -= img[(y-size-1)*w + x];
				--num;
				dest[y*w + x] = (TDEST)sum/num;
			}
		}
	}
	
	// set x0 to -1 to start from the center pixel
	template<typename ResultImage, typename InputImage>
	void guess3d(ResultImage& resultImage, const InputImage& inputImage, int x0 = -1, int y0 = -1)
	{
		int w = inputImage.getWidth();
		int h = inputImage.getHeight();
		
		if(x0 == -1)
		{
			x0 = w/2;
			y0 = h/2;
		}
		
		unsigned long long *dists = new unsigned long long[w*h];
		memset(dists, 0xff, sizeof(long long)*w*h);
		
		std::priority_queue<g3dpqnode> pq;
		pq.push(g3dpqnode(0,x0,y0));
		dists[y0*w + x0] = 0;
		
		unsigned long long maxd = 1;
		
		while(!pq.empty())
		{
			unsigned long long cdist = pq.top().dist;
			int cx = pq.top().x;
			int cy = pq.top().y;
			pq.pop();
			
			maxd = cdist;
			
			if(dists[cy*w + cx] != cdist) continue;
			
			if(cx < w-1)
			{
				int nx = cx + 1;
				int ny = cy;
				unsigned long long ndist = cdist + getw(inputImage,cx,cy,nx,ny);
				unsigned long long& pdist = dists[ny*w + nx];
				if(ndist < pdist)
				{
					pdist = ndist;
					pq.push(g3dpqnode(ndist,nx,ny));
				}
			}
			if(cx > 0)
			{
				int nx = cx - 1;
				int ny = cy;
				unsigned long long ndist = cdist + getw(inputImage,cx,cy,nx,ny);
				unsigned long long& pdist = dists[ny*w + nx];
				if(ndist < pdist)
				{
					pdist = ndist;
					pq.push(g3dpqnode(ndist,nx,ny));
				}
			}
			if(cy < h-1)
			{
				int nx = cx;
				int ny = cy + 1;
				unsigned long long ndist = cdist + getw(inputImage,cx,cy,nx,ny);
				unsigned long long& pdist = dists[ny*w + nx];
				if(ndist < pdist)
				{
					pdist = ndist;
					pq.push(g3dpqnode(ndist,nx,ny));
				}
			}
			if(cy > 0)
			{
				int nx = cx;
				int ny = cy - 1;
				unsigned long long ndist = cdist + getw(inputImage,cx,cy,nx,ny);
				unsigned long long& pdist = dists[ny*w + nx];
				if(ndist < pdist)
				{
					pdist = ndist;
					pq.push(g3dpqnode(ndist,nx,ny));
				}
			}
		}
		
		unsigned char* vis = new unsigned char[w*h];
		memset(vis, 0, sizeof(unsigned char)*w*h);
		
		
		unsigned long long* distsb = new unsigned long long[w*h];
		
		boxBlurX(distsb,dists,w,h,10);
		boxBlurY(dists,distsb,w,h,10);
		
		// fix holes
		std::vector<std::pair<int,int> > q;
		for(int x=0; x<w; ++x)
		{
			q.push_back(std::make_pair(x,0));
			q.push_back(std::make_pair(x,h-1));
			
			vis[0*w + x] = 1;
			vis[(h-1)*w + x] = 1;
		}
		for(int y=1; y<h-1; ++y)
		{
			q.push_back(std::make_pair(0,y));
			q.push_back(std::make_pair(w-1,y));
			
			vis[y*w + 0] = 1;
			vis[y*w + w-1] = 1;
		}
		
		while(!q.empty())
		{
			int cx = q.back().first;
			int cy = q.back().second;
			q.pop_back();
			
			unsigned long long& cdist = dists[cy*w + cx];
			
			if(cx < w-1)
			{
				int nx = cx + 1;
				int ny = cy;
				unsigned long long& ndist = dists[ny*w + nx];
				unsigned char& nvis = vis[ny*w + nx];
				if(nvis == 0 && ndist < cdist)
				{
					q.push_back(std::make_pair(nx,ny));
					nvis = 1;
				}
			}
			
			if(cx > 0)
			{
				int nx = cx - 1;
				int ny = cy;
				unsigned long long& ndist = dists[ny*w + nx];
				unsigned char& nvis = vis[ny*w + nx];
				if(nvis == 0 && ndist < cdist)
				{
					q.push_back(std::make_pair(nx,ny));
					nvis = 1;
				}
			}
			
			if(cy < h-1)
			{
				int nx = cx;
				int ny = cy + 1;
				unsigned long long& ndist = dists[ny*w + nx];
				unsigned char& nvis = vis[ny*w + nx];
				if(nvis == 0 && ndist < cdist)
				{
					q.push_back(std::make_pair(nx,ny));
					nvis = 1;
				}
			}
			
			if(cy > 0)
			{
				int nx = cx;
				int ny = cy - 1;
				unsigned long long& ndist = dists[ny*w + nx];
				unsigned char& nvis = vis[ny*w + nx];
				if(nvis == 0 && ndist < cdist)
				{
					q.push_back(std::make_pair(nx,ny));
					nvis = 1;
				}
			}
		}
		
		for(int y=0; y<h; ++y)
		{
			int from = 0;
			int to = w-1;
			unsigned long long fromval;
			unsigned long long toval;
			
			int x=0;
			for(;;)
			{
				while(x<w && vis[y*w + x]) ++x;
				if(x==w) break;
				
				from = x-1;
				fromval = dists[y*w + x-1];
				
				while(x<w && !vis[y*w + x]) ++x;
				to = x;
				toval = dists[y*w + x];
				
				for(int xx=from+1; xx<=to-1; ++xx)
				{
					dists[y*w + xx] = ((xx-from)*toval + (to-xx)*fromval) / (to-from);
				}
			}
		}
		
		//boxBlurXHoles(dists,vis,w,h,20);
		
		/*
		unsigned long long prev = distsb[1*w+0];
		for(int y=1; y<h-1; ++y)
		for(int x=1; x<w-1; ++x)
		{
			if(vis[y*w + x]) prev = distsb[y*w + x];
			else distsb[y*w + x] = 0;//prev;
		}*/
		
		
		delete[] vis;
		
		for(int y=0; y<h; ++y)
		for(int x=0; x<w; ++x)
		{
			resultImage.pixel(x,y,0) = 255 - dists[y*w + x]*255 / maxd;
		}
		
		delete[] dists;
		delete[] distsb;
	}
	
	/*
	template<typename ResultImage>
	void guess3dFixHoles(ResultImage& resultImage)
	{
		std::queue<std::pair<int,int> > q;
		int width = resultImage.getWidth();
		int height = retultImage.getHeight();
		for(int x=0; x<width; ++x)
		{
		}
	}
	*/
	
}
