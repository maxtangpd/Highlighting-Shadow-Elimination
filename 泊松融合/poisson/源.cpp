#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
class LaplacianBlending {
private:
	Mat_<Vec3f> left;
	Mat_<Vec3f> right;
	Mat_<float> blendMask;

	vector<Mat_<Vec3f> > leftLapPyr, rightLapPyr, resultLapPyr;//Laplacian Pyramids数组 左 右 结果  
	Mat leftHighestLevel, rightHighestLevel, resultHighestLevel;
	vector<Mat_<Vec3f> > maskGaussianPyramid; //masks are 3-channels for easier multiplication with RGB  

	int levels;

	void buildPyramids() {
		buildLaplacianPyramid(left, leftLapPyr, leftHighestLevel);
		buildLaplacianPyramid(right, rightLapPyr, rightHighestLevel);
		buildGaussianPyramid();
	}

	void buildGaussianPyramid() {//金字塔内容为每一层的掩模  
		assert(leftLapPyr.size()>0);
		maskGaussianPyramid.clear();
		Mat currentImg;
		cvtColor(blendMask, currentImg, CV_GRAY2BGR);//store color img of blend mask into maskGaussianPyramid  
		maskGaussianPyramid.push_back(currentImg); //0-level  

		currentImg = blendMask;
		for (int l = 1; l<levels + 1; l++) {
			Mat _down;
			if (leftLapPyr.size() > l)
				pyrDown(currentImg, _down, leftLapPyr[l].size());
			else
				pyrDown(currentImg, _down, leftHighestLevel.size()); //lowest level  

			Mat down;
			cvtColor(_down, down, CV_GRAY2BGR);
			maskGaussianPyramid.push_back(down);//add color blend mask into mask Pyramid  
			currentImg = _down;
		}
	}

	void buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& HighestLevel) {
		lapPyr.clear();
		Mat currentImg = img;
		for (int l = 0; l<levels; l++) {
			Mat down, up;
			pyrDown(currentImg, down);
			pyrUp(down, up, currentImg.size());
			Mat lap = currentImg - up;
			lapPyr.push_back(lap);
			currentImg = down;
		}
		currentImg.copyTo(HighestLevel);
	}

	Mat_<Vec3f> reconstructImgFromLapPyramid() {
		//将左右laplacian图像拼成的resultLapPyr金字塔中每一层  
		//从上到下插值放大并相加，即得blend图像结果  
		Mat currentImg = resultHighestLevel;
		for (int l = levels - 1; l >= 0; l--) {
			Mat up;

			pyrUp(currentImg, up, resultLapPyr[l].size());
			currentImg = up + resultLapPyr[l];
		}
		return currentImg;
	}

	void blendLapPyrs() {
		//获得每层金字塔中直接用两图Laplacian变换拼成的图像resultLapPyr  
		resultHighestLevel = leftHighestLevel.mul(maskGaussianPyramid.back()) +
			rightHighestLevel.mul(maskGaussianPyramid.back());
		for (int l = 0; l<levels; l++) {
			Mat A = leftLapPyr[l].mul(maskGaussianPyramid[l]);
			Mat B = rightLapPyr[l].mul(maskGaussianPyramid[l]);
			Mat_<Vec3f> blendedLevel = A + B;

			resultLapPyr.push_back(blendedLevel);
		}
	}

public:
	LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, const Mat_<float>& _blendMask, int _levels) ://construct function, used in LaplacianBlending lb(l,r,m,4);  
		left(_left), right(_right), blendMask(_blendMask), levels(_levels)
	{
		assert(_left.size() == _right.size());
		assert(_left.size() == _blendMask.size());
		buildPyramids();  //construct Laplacian Pyramid and Gaussian Pyramid  
		blendLapPyrs();   //blend left & right Pyramids into one Pyramid  
	};

	Mat_<Vec3f> blend() {
		return reconstructImgFromLapPyramid();//reconstruct Image from Laplacian Pyramid  
	}
};

Mat_<Vec3f> LaplacianBlend(const Mat_<Vec3f>& l, const Mat_<Vec3f>& r, const Mat_<float>& m) {
	LaplacianBlending lb(l, r, m, 4);
	return lb.blend();
}

int main() {
	Mat srcl = imread("F:\\project\\lapla\\t2.jpg");
	Mat srcr = imread("F:\\project\\lapla\\t2r.jpg");
	Mat grayl, templ;
	Mat grayr, tempr;
	cvtColor(srcl, grayl, COLOR_RGB2GRAY);
	cvtColor(srcr, grayr, COLOR_RGB2GRAY);
	threshold(grayl, templ, 230, 255, 0);
	threshold(grayr, tempr, 230, 255, 0);
	//	dilate(templ, templ, Mat(), Point(-1, -1), 3);
	//	dilate(tempr, tempr, Mat(), Point(-1, -1), 3);
	Mat  maskedl, maskedr, lastl, finl, lastr, finr, dstl;
	dilate(templ, templ, Mat(), Point(-1, -1),3);
	dilate(tempr, tempr, Mat(), Point(-1, -1), 3);	
	grayr.copyTo(maskedl, templ); //掩膜提取另一张图非反光部分
	grayl.copyTo(maskedr, tempr);
	addWeighted(grayl, 1, templ, -1, 0, lastl);//反光部分取反
	addWeighted(lastl, 1, maskedl, 1, 0, finl);//反光部分替换
	addWeighted(grayr, 1, tempr, -1, 0, lastr);
	addWeighted(lastr, 1, maskedr, 1, 0, finr);
//计算ROI区域的位置
	int minxl = INT_MAX, minyl = INT_MAX, maxxl = INT_MIN, maxyl = INT_MIN;
	int hl = templ.size().height;
	int wl = templ.size().width;
	for (int i = 0; i<hl; i++)
	{
		for (int j = 0; j<wl; j++)
		{

			if (templ.at<uchar>(i, j) == 255)
			{
				minxl = std::min(minxl, i);
				maxxl = std::max(maxxl, i);
				minyl = std::min(minyl, j);
				maxyl = std::max(maxyl, j);
			}
		}
	}
	int lenxl = (maxxl - minxl)/2+minxl;//计算矩形的宽  
	int lenyl = (maxyl - minyl)/2+minyl;//计算矩形的高  

	int minxr = INT_MAX, minyr = INT_MAX, maxxr = INT_MIN, maxyr = INT_MIN;
	int hr = tempr.size().height;
	int wr = tempr.size().width;
	for (int i = 0; i<hl; i++)
	{
		for (int j = 0; j<wl; j++)
		{

			if (tempr.at<uchar>(i, j) == 255)
			{
				minxr = std::min(minxr, i);
				maxxr = std::max(maxxr, i);
				minyr = std::min(minyr, j);
				maxyr = std::max(maxyr, j);
			}
		}
	}
	int lenxr = (maxxr - minxr) / 2 + minxr;//计算矩形的宽  
	int lenyr = (maxyr - minyr) / 2 + minyr;//计算矩形的高  


//	Mat src_mask = 255 * Mat::ones(templ.rows,templ.cols, templ.depth());
//泊松融合	
	Mat normal_clonel, normal_cloner;
	Mat colorr, colorl, colort;


	cvtColor(grayr, colorr, CV_GRAY2BGR);
	cvtColor(grayl, colorl, CV_GRAY2BGR);
	cvtColor(templ, colort, CV_GRAY2BGR);

	Point centerl(lenyl, lenxl);
	Point centerr(lenyr, lenxr);
	seamlessClone(colorr, colorl, templ, centerl, normal_clonel, NORMAL_CLONE);
	seamlessClone(colorl, colorr, tempr, centerr, normal_cloner, NORMAL_CLONE);

	Mat_<Vec3f> l; normal_clonel.convertTo(l, CV_32F, 1.0 / 255.0);//Vec3f表示有三个通道，即 l[row][column][depth]  
	Mat_<Vec3f> r; normal_cloner.convertTo(r, CV_32F, 1.0 / 255.0);
	Mat_<float> m(l.rows, l.cols, 0.0);                 //将m全部赋值为0  
	m(Range::all(), Range::all()) = 0.5;    //取m全部行&全部列，赋值为1.0  

	Mat_<Vec3f> blend = LaplacianBlend(l, r, m);
	namedWindow("blended", 0);
	imshow("blended", blend);

	namedWindow("泊松融合", 0);
	imshow("泊松融合", normal_clonel);
	imwrite("F:\\project\\lapla\\ggll.jpg", normal_clonel);
	imwrite("F:\\project\\lapla\\ggrr.jpg", normal_cloner);
	namedWindow("a", 0);
	imshow("a", maskedl);
	namedWindow("ba", 0);
	imshow("ba", templ);
//	namedWindow("原图", 0);
//	imshow("原图", colorr);
//	namedWindow("c", 0);
//	imshow("c", templ);
//	namedWindow("right", 0);
//	imshow("right", tempr);
//	namedWindow("left", 0);
//	imshow("left", templ);
//	namedWindow("去高光", 0);
//	imshow("去高光", finl);
//	namedWindow("lastr", 0);
//	imshow("lastr", finr);
	waitKey(0);
	return 0;
	system("pause");
}

