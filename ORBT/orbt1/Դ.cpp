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
	imshow("ԭʼ����ͼ��", image01);
	imshow("��׼ͼ��", image02);

	//�Ҷ�ͼת��  
	Mat image1, image2;
	cvtColor(image01, image1, CV_RGB2GRAY);
	cvtColor(image02, image2, CV_RGB2GRAY);


	//��ȡ������    
	SurfFeatureDetector surfDetector(800);  // ����������ֵ  
	vector<KeyPoint> keyPoint1, keyPoint2;
	surfDetector.detect(image1, keyPoint1);
	surfDetector.detect(image2, keyPoint2);

	//������������Ϊ�±ߵ�������ƥ����׼��    
	SurfDescriptorExtractor SurfDescriptor;
	Mat imageDesc1, imageDesc2;
	SurfDescriptor.compute(image1, keyPoint1, imageDesc1);
	SurfDescriptor.compute(image2, keyPoint2, imageDesc2);

	//���ƥ�������㣬����ȡ�������     
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(imageDesc1, imageDesc2, matchePoints, Mat());
	sort(matchePoints.begin(), matchePoints.end()); //����������    

													//��ȡ����ǰN��������ƥ��������  
	vector<Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i<10; i++)
	{
		imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);
		imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
	}

	//��ȡͼ��1��ͼ��2��ͶӰӳ����� �ߴ�Ϊ3*3  
	Mat homo = findHomography(imagePoints1, imagePoints2, CV_RANSAC);
	////Ҳ����ʹ��getPerspectiveTransform�������͸�ӱ任���󣬲���Ҫ��ֻ����4���㣬Ч���Բ�  
	//Mat   homo=getPerspectiveTransform(imagePoints1,imagePoints2);     
	cout << "�任����Ϊ��\n" << homo << endl << endl; //���ӳ�����  
												//ͼ����׼  
	Mat imageTransform1, imageTransform2;
	warpPerspective(image01, imageTransform1, homo, Size(image02.cols, image02.rows));
	imshow("����͸�Ӿ���任��", imageTransform1);

	waitKey();
	return 0;
}