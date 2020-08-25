#include <iostream>
#include <opencv2/opencv.hpp>  
#include <opencv2/xfeatures2d.hpp>
#include "code.h"
using namespace cv;
using namespace std;

int main()
{
	//Create SIFT class pointer
	Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
	//读入图片
	Mat img_1 = imread("F:\\project\\lapla\\gg1.jpg");
	Mat img_2 = imread("F:\\project\\lapla\\gg2.jpg");
	//Detect the keypoints
	vector<KeyPoint> keypoints_1, keypoints_2;
	f2d->detect(img_1, keypoints_1);
	f2d->detect(img_2, keypoints_2);
	//Calculate descriptors (feature vectors)
	Mat descriptors_1, descriptors_2;
	f2d->compute(img_1, keypoints_1, descriptors_1);
	f2d->compute(img_2, keypoints_2, descriptors_2);
	//Matching descriptor vector using BFMatcher
	BFMatcher matcher;
	vector<DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);


	double max_dist = 0; double min_dist = 50;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_1.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	/*printf("-- Max dist : %f \n", max_dist );
	printf("-- Min dist : %f \n", min_dist );*/

	//筛选匹配点
	std::vector< DMatch > good_matches;
	vector<Point2f> leftPoint, rightPoint;
	for (int i = 0; i < descriptors_1.rows; i++) {
		if (matches[i].distance < 0.6*max_dist) {
			good_matches.push_back(matches[i]);
			leftPoint.push_back(keypoints_1[matches[i].queryIdx].pt);//保存左图像的关键点
			rightPoint.push_back(keypoints_2[matches[i].trainIdx].pt);//保存右图像的关键点
		}
	}

	//绘制匹配出的关键点
	Mat img_matches;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, good_matches, img_matches);
	namedWindow("【match图】",0);
	imshow("【match图】", img_matches);
	namedWindow("tst", 0);
	//imshow("tst", Register(img_1, img_2, leftPoint, rightPoint));
	Mat trans = Register(img_1, img_2, leftPoint, rightPoint);
	imwrite("F:\\project\\lapla\\trans1.jpg", trans);
	//等待任意按键按下
	waitKey(600000);
}