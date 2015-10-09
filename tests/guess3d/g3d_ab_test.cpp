#include<atablash/guess3d.h>

#include<cstdio>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

struct Image
{
	Mat img;
	int getWidth() const
	{
		return img.cols;
	}
	int getHeight() const
	{
		return img.rows;
	}
	int getNumChannels() const
	{
		return img.channels();
	}
	unsigned char& pixel(int x, int y, int ch)
	{
		return img.data[(y*img.cols + x)*img.channels() + ch];
	}
	const unsigned char& pixel(int x, int y, int ch) const
	{
		return img.data[(y*img.cols + x)*img.channels() + ch];
	}
};

int main( int argc, const char** argv )
{
	Image im;
	
	im.img  = imread("mama/mama.jpg", CV_LOAD_IMAGE_UNCHANGED);
	
	Image imResult(im);
	cvtColor(imResult.img, imResult.img, CV_BGR2GRAY);
	
	//image.pixel(10,10,1) = 255;
	
	
	ab::guess3d(imResult, im, -1, -1);
	
	
	equalizeHist(imResult.img,imResult.img);

    namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
	imshow("MyWindow", imResult.img);

	waitKey(0);

    destroyWindow("MyWindow");

	return 0;
}