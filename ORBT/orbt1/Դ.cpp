#include "highgui/highgui.hpp"    
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
#include <iostream>  

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
	Mat image01 = imread("F:\\project\\lapla\\t2.jpg");
	Mat image02 = imread("F:\\project\\lapla\\t1r.jpg");
	imshow("原始测试图像", image01);
	imshow("基准图像", image02);

	//灰度图转换  
	Mat image1, image2;
	cvtColor(image01, image1, CV_RGB2GRAY);
	cvtColor(image02, image2, CV_RGB2GRAY);


	//提取特征点    
	SurfFeatureDetector surfDetector(800);  // 海塞矩阵阈值  
	vector<KeyPoint> keyPoint1, keyPoint2;
	surfDetector.detect(image1, keyPoint1);
	surfDetector.detect(image2, keyPoint2);

	//特征点描述，为下边的特征点匹配做准备    
	SurfDescriptorExtractor SurfDescriptor;
	Mat imageDesc1, imageDesc2;
	SurfDescriptor.compute(image1, keyPoint1, imageDesc1);
	SurfDescriptor.compute(image2, keyPoint2, imageDesc2);

	//获得匹配特征点，并提取最优配对     
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, Mat());
	sort(matchePoints.begin(), matchePoints.end()); //特征点排序    

													//获取排在前N个的最优匹配特征点  
	vector<Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i<10; i++)
	{
		imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
	}

	//获取图像1到图像2的投影映射矩阵 尺寸为3*3  
	Mat homo = findHomography(imagePoints1, imagePoints2, CV_RANSAC);
	////也可以使用getPerspectiveTransform方法获得透视变换矩阵，不过要求只能有4个点，效果稍差  
	//Mat   homo=getPerspectiveTransform(imagePoints1,imagePoints2);     
	cout << "变换矩阵为：\n" << homo << endl << endl; //输出映射矩阵  
												//图像配准  
	Mat imageTransform1, imageTransform2;
	warpPerspective(image01, imageTransform1, homo, Size(image02.cols, image02.rows));
	imshow("经过透视矩阵变换后", imageTransform1);

	waitKey();
	return 0;
}