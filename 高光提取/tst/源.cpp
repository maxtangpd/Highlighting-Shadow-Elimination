
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <iostream>
using namespace std;
using namespace cv;

int main() {
	Mat srcl = imread("F:\\project\\lapla\\left.jpg");
	Mat srcr = imread("F:\\project\\lapla\\right.jpg");
	
	
	Mat grayl, templ;
	Mat grayr, tempr;
	cvtColor(srcl, grayl, COLOR_RGB2GRAY);
	cvtColor(srcr, grayr, COLOR_RGB2GRAY);

	threshold(grayl, templ, 240, 255, 0);
	threshold(grayr, tempr, 240, 255, 0);
	dilate(templ, templ, Mat(), Point(-1, -1), 3);
	dilate(tempr, tempr, Mat(), Point(-1, -1), 3);
	Mat masked,last;
	grayr.copyTo(masked, templ);

	namedWindow("right", 0);
	imshow("right", tempr);
	namedWindow("left", 0);
	imshow("left", templ);
	namedWindow("masked", 0);
	imshow("masked", masked);

	waitKey(0);
	return 0;
	system("pause");
}