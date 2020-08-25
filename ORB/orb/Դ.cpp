#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "code.h"
using namespace std;
using namespace cv;

	int main()
	{
		Mat img_1 = imread("F:\\project\\lapla\\3jpg");
		Mat img_2 = imread("F:\\project\\lapla\\5.jpg");
		//pyrDown(img_1, img_1, Size(img_1.cols / 2, img_1.rows / 2));
		//pyrDown(img_2, img_2, Size(img_2.cols / 2, img_2.rows / 2));

		// -- Step 1: Detect the keypoints using STAR Detector 
		std::vector<KeyPoint> keypoints_1, keypoints_2;
		int nkeypoint = 100;//特征点个数
		Ptr<ORB> orb = ORB::create(nkeypoint);

		orb->detect(img_1, keypoints_1);
		orb->detect(img_2, keypoints_2);

		// -- Stpe 2: Calculate descriptors (feature vectors) 
		Mat descriptors_1, descriptors_2;
		orb->compute(img_1, keypoints_1, descriptors_1);
		orb->compute(img_2, keypoints_2, descriptors_2);



		
		//-- Step 3: Matching descriptor vectors with a brute force matcher 
		BFMatcher matcher(NORM_HAMMING);
		std::vector<DMatch> matches;
		matcher.match(descriptors_1, descriptors_2, matches);

		double max_dist = 0; double min_dist = 100;
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

		// -- dwaw matches 
		Mat img_mathes;
		drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_mathes);
		// -- show 
		namedWindow("OPENCV_ORB",0);
		imshow("OPENCV_ORB", img_mathes);
		namedWindow("tst", 0);
		imshow("tst", Register(img_1, img_2, leftPoint, rightPoint));
		waitKey(0);
		return 0;
	}