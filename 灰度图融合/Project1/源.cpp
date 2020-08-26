
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
/************************************************************************/
/* 说明：
*金字塔从下到上依次为 [0,1，...，level-1] 层
*blendMask 为图像的掩模
*maskGaussianPyramid为金字塔每一层的掩模
*resultLapPyr 存放每层金字塔中直接用左右两图Laplacian变换拼成的图像
*/
/************************************************************************/

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
		resultHighestLevel = leftHighestLevel.mul(maskGaussianPyramid.back()) +
			rightHighestLevel.mul(maskGaussianPyramid.back());
		for (int l = 0; l<levels; l++) {
			Mat A = leftLapPyr[l].mul(maskGaussianPyramid[l]);
			Mat B = rightLapPyr[l].mul(maskGaussianPyramid[l]);
			for (int i = 0; i < A.rows; i++) {
				for (int j = 0; j < A.cols; j++) {
					for (int k = 0; k < 3; k++)
						if (A.at<Vec3f>(i, j)[k] > B.at<Vec3f>(i, j)[k] ) {
							A.at<Vec3f>(i, j)[k] = B.at<Vec3f>(i, j)[k];
						}
						else if (B.at<Vec3f>(i, j)[k] > A.at<Vec3f>(i, j)[k]) {
							B.at<Vec3f>(i, j)[k] = A.at<Vec3f>(i, j)[k];
						}//去高光
				}
			}
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
	Mat grayl, grayr,colorl,colorr;
	Mat maskedl, maskedr, lastl, finl, lastr, finr;
	Mat templ, tempr;
	Mat l8u = imread("F:\\project\\lapla\\ll.jpg");
	Mat r8u = imread("F:\\project\\lapla\\rr.jpg");
	
	cvtColor(l8u, grayl, COLOR_RGB2GRAY);
	cvtColor(r8u, grayr, COLOR_RGB2GRAY);

	Mat tst = (grayl + grayr) / 2;
//	threshold(grayl, templ, 245, 255, 0);
//	threshold(grayr, tempr, 245, 255, 0);
//	grayr.copyTo(maskedl, templ); //掩膜提取另一张图非反光部分
//	grayl.copyTo(maskedr, tempr);

//	addWeighted(grayl, 1, templ, -1, 0, lastl);//反光部分取反
//	addWeighted(lastl, 1, maskedl, 1, 0, finl);
//	addWeighted(grayr, 1, tempr, -1, 0, lastr);//反光部分替换
//	addWeighted(lastr, 1, maskedr, 1, 0, finr);

	cvtColor(grayl, colorl, CV_GRAY2BGR);
	cvtColor(grayr, colorr, CV_GRAY2BGR);

	namedWindow("left", 0);
	namedWindow("right", 0);
	imshow("left", grayl);
	imshow("right", grayr);

//	namedWindow("highr", 0);
//	imshow("highr", tempr);
//	namedWindow("highl", 0);
//	imshow("highl", templ);

	Mat_<Vec3f> l; colorl.convertTo(l, CV_32F, 1.0 / 255.0);//Vec3f表示有三个通道，即 l[row][column][depth]  
	Mat_<Vec3f> r; colorr.convertTo(r, CV_32F, 1.0 / 255.0);
	/*****************    void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;******************/
	/* Performs linear transformation on every source array element:
	dst(x,y,c) = scale*src(x,y,alpha)+beta.
	Arbitrary combination of input and output array depths are allowed
	(number of channels must be the same), thus the function can be used
	for type conversion */

	//create blend mask matrix m  
	Mat_<float> m(l.rows, l.cols, 0.0);                 //将m全部赋值为0  
	m(Range::all(), Range::all()) = 0.5;    //取m全部行&全部列，赋值为1.0  

	Mat_<Vec3f> blend = LaplacianBlend(l, r, m);
	namedWindow("blended", 0);
	imshow("blended", blend);

	Mat ben;
	cvtColor(blend, ben, COLOR_RGB2GRAY);
	imwrite("F:\\project\\lapla\\lap.jpg", ben);


	namedWindow("tst", 0);
	imshow("tst", tst);
	waitKey(0);
	return 0;
	system("pause");
}
