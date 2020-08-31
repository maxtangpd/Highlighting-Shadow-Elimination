#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include<opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main() {
	Mat srcl = imread("F:\\project\\lapla\\t2r.jpg");
	Mat srcr = imread("F:\\project\\lapla\\trans.jpg");
	Mat grayl, templ;
	Mat grayr, tempr;
	cvtColor(srcl, grayl, COLOR_RGB2GRAY);
	cvtColor(srcr, grayr, COLOR_RGB2GRAY);
	threshold(grayl, templ, 235, 255, 0);
	threshold(grayr, tempr, 235, 255, 0);
	//	dilate(templ, templ, Mat(), Point(-1, -1), 1);
	//	dilate(tempr, tempr, Mat(), Point(-1, -1), 1);
	Mat  maskedl,maskedr,lastl,finl,lastr,finr,dstl;
	grayr.copyTo(maskedl, templ); //掩膜提取另一张图非反光部分
	grayl.copyTo(maskedr, tempr);
	addWeighted(grayl, 1, templ, -1, 0, lastl);//反光部分取反
	addWeighted(lastl, 1, maskedl, 1 , 0, finl);//反光部分替换
	addWeighted(grayr, 1, tempr, -1, 0, lastr);
	addWeighted(lastr, 1, maskedr, 1, 0, finr);


	namedWindow("right", 0);
	imshow("right", tempr);
	namedWindow("left", 0);
	imshow("left", templ);
	namedWindow("lastl", 0);
	imshow("lastl", finl);
	namedWindow("lastr", 0);
	imshow("lastr", finr);
	waitKey(0);
	return 0;
	system("pause");
}


