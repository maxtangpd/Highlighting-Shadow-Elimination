#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

// calculate entropy of an image
double Entropy(Mat img)
{
	double temp[256] = { 0.0 };

	// 计算每个像素的累积值
	for (int m = 0; m<img.rows; m++)
	{// 有效访问行列的方式
		const uchar* t = img.ptr<uchar>(m);
		for (int n = 0; n<img.cols; n++)
		{
			int i = t[n];
			temp[i] = temp[i] + 1;
		}
	}

	// 计算每个像素的概率
	for (int i = 0; i<256; i++)
	{
		temp[i] = temp[i] / (img.rows*img.cols);
	}

	double result = 0;
	// 计算图像信息熵
	for (int i = 0; i<256; i++)
	{
		if (temp[i] == 0.0)
			result = result;
		else
			result = result - temp[i] * (log(temp[i]) / log(2.0));
	}

	return result;

}

double ComEntropy(Mat img1, Mat img2, double img1_entropy, double img2_entropy)
{
	double temp[256][256] = { 0.0 };

	// 计算联合图像像素的累积值
	for (int m1 = 0, m2 = 0; m1 < img1.rows, m2 < img2.rows; m1++, m2++)
	{    // 有效访问行列的方式
		const uchar* t1 = img1.ptr<uchar>(m1);
		const uchar* t2 = img2.ptr<uchar>(m2);
		for (int n1 = 0, n2 = 0; n1 < img1.cols, n2 < img2.cols; n1++, n2++)
		{
			int i = t1[n1], j = t2[n2];
			temp[i][j] = temp[i][j] + 1;
		}
	}

	// 计算每个联合像素的概率
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)

		{
			temp[i][j] = temp[i][j] / (img1.rows*img1.cols);
		}
	}

	double result = 0.0;
	//计算图像联合信息熵
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)

		{
			if (temp[i][j] == 0.0)
				result = result;
			else
				result = result - temp[i][j] * (log(temp[i][j]) / log(2.0));
		}
	}

	//得到两幅图像的互信息熵
	img1_entropy = Entropy(img1);
	img2_entropy = Entropy(img2);
	result = img1_entropy + img2_entropy - result;

	return result;

}

int main(int argc, char ** argv)
{
	const char* imagename = "pano_b.jpg";

	//从文件中读入图像
	Mat img = imread("F:\\project\\lapla\\n0000.jpg",0);
	Mat imgr = imread("F:\\project\\lapla\\ll.jpg", 0);
	Mat imgl = imread("F:\\project\\lapla\\rr.jpg",0);
	namedWindow("left", 0);
	imshow("left", img);
	//如果读入图像失败
	//if (img.empty())
	//{
	//	fprintf(stderr, "Can not load image %s\n", imagename);
	//	return -1;
	//}

	double a1 = Entropy(img);
	double a2 = Entropy(imgr);
	double a3 = Entropy(imgl);
	double a=ComEntropy(img,  imgr,a1,a2);
	double b = ComEntropy(img, imgl, a1, a3);
	double c = (a + b)/2;
	cout << c << endl;
	waitKey(0);
	return 0;
	system("pause");

}