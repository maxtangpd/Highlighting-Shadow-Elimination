#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat Register(Mat img_1, Mat img_2, vector<Point2f> leftPoint, vector<Point2f> rightPoint) {
	int64 st, et;

	st = getTickCount();
	Mat Homography = findHomography(leftPoint, rightPoint, CV_RANSAC, 3);//生成单应性矩阵
	Mat tst;
	warpPerspective(img_1,tst,Homography,Size(img_1.cols,img_1.rows));
	//Mat shftMat=(Mat_<double>(3,3)<<1.0,0,img_1.cols, 0,1.0,0, 0,0,1.0);
	cout << Homography << endl;
	vector<Point2f> CorrectImg;//根据单应性矩阵校正之后的点
	vector<Point2f> ponits;//保存图像的所有的点
	
	//for (int i = 0; i<img_1.rows; i++) {
	//	for (int j = 0; j<img_2.cols; j++) {
	//		ponits.push_back(Point2f(j, i));
	//	}
	//}
	//perspectiveTransform(ponits, CorrectImg, Homography);//生成对应关系
	//													 //imshow("待配准",img_1);
	//													 // imshow("模板",img_2);
	//Mat img_trans = Mat::zeros(img_1.rows, img_1.cols, CV_8UC3);  //矫正后的图像
	//int count = 0;
	//for (int i = 0; i<img_1.rows; i++) {
	//	uchar* p = img_1.ptr<uchar>(i);
	//	for (int j = 0; j<img_1.cols; j++) {
	//		int y = CorrectImg[count].y;
	//		int x = CorrectImg[count].x;
	//		if (y < 0 || x < 0 || x >= img_1.cols || y >= img_1.rows) {
	//			count++;
	//			continue;
	//		}
	//		uchar* t = img_trans.ptr<uchar>(y);
	//		t[x * 3] = p[j * 3];
	//		t[x * 3 + 1] = p[j * 3 + 1];
	//		t[x * 3 + 2] = p[j * 3 + 2];
	//		count++;
	//	}
	//}
	//et = getTickCount() - st;
	//et = et * 1000 / getTickFrequency();
	//cout << "Register time:" << et << "ms" << endl;
	//cout << endl;
	//Size size(img_1.cols * 2, img_1.rows * 2);

	//Mat img_merge;
	//Mat outimg_1, outimg_2, outimg_3;

	//img_merge.create(size, CV_MAKETYPE(img_1.depth(), 3));
	//img_merge = Scalar::all(0);
	//outimg_1 = img_merge(Rect(0, 0, img_1.cols, img_1.rows));
	//outimg_2 = img_merge(Rect(img_1.cols, 0, img_2.cols, img_2.rows));
	//outimg_3 = img_merge(Rect(img_1.cols, img_1.rows, img_trans.cols, img_trans.rows));

	//img_1.copyTo(outimg_1);
	//img_2.copyTo(outimg_2);
	//img_trans.copyTo(outimg_3);

	return(tst);
}


