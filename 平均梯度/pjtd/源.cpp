#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
double calcAvG(Mat img)
{
	if (img.channels() != 1)
	{
		std::cout << "输入必须是单通道图像！" << std::endl;
		return 0.0;
	}


	img.convertTo(img, CV_64FC1);

	double tmp = 0;
	int rows = img.rows - 1;
	int cols = img.cols - 1;
	for (int i = 0; i<rows; i++)
	{
		for (int j = 0; j<cols; j++)
		{
			double dx = img.at<double>(i, j + 1) - img.at<double>(i, j);
			double dy = img.at<double>(i + 1, j) - img.at<double>(i, j);
			double ds = std::sqrt((dx*dx + dy*dy) / 2);
			tmp = tmp + ds;
		}
	}
	double imageAvG = tmp / (rows*cols);
	return imageAvG;
}

int main()
{
	const char* imagename = "a.jpg";

	//从文件中读入图像
	Mat img = imread("F:\\project\\lapla\\n0000.jpg", 0);
	namedWindow("left", 0);
	imshow("left", img);
	//如果读入图像失败
	//if (img.empty())
	//{
	//	fprintf(stderr, "Can not load image %s\n", imagename);
	//	return -1;
	//}
	double a = calcAvG(img);
	cout << a << endl;
	waitKey(0);
	return 0;
	system("pause");

}