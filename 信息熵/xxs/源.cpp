#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

// calculate entropy of an image
double Entropy(Mat img)
{
	// 将输入的矩阵为图像
	double temp[256];
	for (int i = 0; i<256; i++)
	{
		temp[i] = 0.0;
	}

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
	// 根据定义计算图像熵
	for (int i = 0; i<256; i++)
	{
		if (temp[i] == 0.0)
			result = result;
		else
			//result = result - temp[i] * (log(temp[i]) / log(2.0));
		result = result - temp[i] * (log2(temp[i]));
	}
	return result;
}

int main(int argc, char ** argv)
{
	const char* imagename = "pano_b.jpg";

	//从文件中读入图像
	Mat img = imread("F:\\project\\lapla\\nDimfus2.jpg",0);
	namedWindow("left", 0);
	imshow("left", img);
	//如果读入图像失败
	//if (img.empty())
	//{
	//	fprintf(stderr, "Can not load image %s\n", imagename);
	//	return -1;
	//}
	double a = Entropy(img);
	cout << a << endl;
	waitKey(0);
	return 0;
	system("pause");

}