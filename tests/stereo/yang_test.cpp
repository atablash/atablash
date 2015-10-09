#include<atablash/stereo_match.h>

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
	Image imLeft;
	Image imRight;
	
	imLeft.img  = imread("mama/l.jpg", CV_LOAD_IMAGE_UNCHANGED);
	imRight.img = imread("mama/r.jpg", CV_LOAD_IMAGE_UNCHANGED);
    
	//imLeft.img  = imread("ja3/l.jpg", CV_LOAD_IMAGE_UNCHANGED);
	//imRight.img = imread("ja3/r.jpg", CV_LOAD_IMAGE_UNCHANGED);
	
	//imLeft.img  = imread("tsukuba/left.ppm", CV_LOAD_IMAGE_UNCHANGED);
	//imRight.img = imread("tsukuba/right.ppm", CV_LOAD_IMAGE_UNCHANGED);
    
	//imLeft.img  = imread("my1/l.bmp", CV_LOAD_IMAGE_UNCHANGED);
	//imRight.img = imread("my1/r.bmp", CV_LOAD_IMAGE_UNCHANGED);
	
	Image imResult(imLeft);
	cvtColor(imResult.img, imResult.img, CV_BGR2GRAY);
	
	//image.pixel(10,10,1) = 255;
	
	ab::stereoMatchYang<float>(imResult, imLeft, imRight, 100, 0.2f, 0.0f);
	
	
	equalizeHist(imResult.img,imResult.img);

    namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
	imshow("MyWindow", imResult.img);

	waitKey(0);

    destroyWindow("MyWindow");

	return 0;
}