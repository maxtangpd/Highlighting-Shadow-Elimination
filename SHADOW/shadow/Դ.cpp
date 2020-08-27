#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include<opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
/************************************************************************/
/* ˵����
*���������µ�������Ϊ [0,1��...��level-1] ��
*blendMask Ϊͼ�����ģ
*maskGaussianPyramidΪ������ÿһ�����ģ
*resultLapPyr ���ÿ���������ֱ����������ͼLaplacian�任ƴ�ɵ�ͼ��
*/
/************************************************************************/

class LaplacianBlending {
private:
	Mat_<Vec3f> left;
	Mat_<Vec3f> right;
	Mat_<float> blendMask;

	vector<Mat_<Vec3f> > leftLapPyr, rightLapPyr, resultLapPyr;//Laplacian Pyramids���� �� �� ���  
	Mat leftHighestLevel, rightHighestLevel, resultHighestLevel;
	vector<Mat_<Vec3f> > maskGaussianPyramid; //masks are 3-channels for easier multiplication with RGB  

	int levels;

	void buildPyramids() {
		buildLaplacianPyramid(left, leftLapPyr, leftHighestLevel);
		buildLaplacianPyramid(right, rightLapPyr, rightHighestLevel);
		buildGaussianPyramid();
	}

	void buildGaussianPyramid() {//����������Ϊÿһ�����ģ  
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
		//������laplacianͼ��ƴ�ɵ�resultLapPyr��������ÿһ��  
		//���ϵ��²�ֵ�Ŵ���ӣ�����blendͼ����  
		Mat currentImg = resultHighestLevel;
		for (int l = levels - 1; l >= 0; l--) {
			Mat up;

			pyrUp(currentImg, up, resultLapPyr[l].size());
			currentImg = up + resultLapPyr[l];
		}
		return currentImg;
	}

	void blendLapPyrs() {
		//���ÿ���������ֱ������ͼLaplacian�任ƴ�ɵ�ͼ��resultLapPyr  
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
	Mat grayl, grayr, colorl, colorr;
	Mat maskedl, maskedr, lastl, finl, lastr, finr;
	Mat templ, tempr,tempa,graya;
	Mat l8u = imread("F:\\project\\lapla\\right.jpg");
	Mat r8u = imread("F:\\project\\lapla\\left.jpg");
	Mat a = imread("F:\\project\\lapla\\a.jpg");

	cvtColor(l8u, grayl, COLOR_RGB2GRAY);
	cvtColor(r8u, grayr, COLOR_RGB2GRAY);
	threshold(grayl, templ, 245, 255, 0);
	threshold(grayr, tempr, 245, 255, 0);
	grayr.copyTo(maskedl, templ); //��Ĥ��ȡ��һ��ͼ�Ƿ��ⲿ��
	grayl.copyTo(maskedr, tempr);

	addWeighted(grayl, 1, templ, -1, 0, lastl);//���ⲿ��ȡ��
	addWeighted(lastl, 1, maskedl, 1, 0, finl);
	addWeighted(grayr, 1, tempr, -1, 0, lastr);//���ⲿ���滻
	addWeighted(lastr, 1, maskedr, 1, 0, finr);



//	namedWindow("left", 0);
	namedWindow("right", 0);
//	imshow("left", grayl);
	imshow("right", finr);


//	namedWindow("highr", 0);
//	imshow("highr", tempr);
//	namedWindow("highl", 0);
//	imshow("highl", templ);


	Mat imageMaskl ;
	//ͨ����ֵ��������Mask
	threshold(finl, imageMaskl, 225, 255, 0);
	Mat Kernell = getStructuringElement(MORPH_RECT, Size(5, 5));
	//��Mask���ʹ�������Mask���
	//dilate(imageMaskl, imageMaskl, Kernell);
	dilate(imageMaskl, imageMaskl, Mat(), Point(-1, -1), 3);

	//ͼ���޸�
	Mat tstl;
	inpaint(finl, imageMaskl, tstl, 4, INPAINT_TELEA);


	
//	Mat imageMasktst, wa,graytst;
//	Mat tst = imread("F:\\project\\lapla\\1.jpg");
//	cvtColor(tst, graytst, COLOR_RGB2GRAY);
//	threshold(graytst, imageMasktst,220, 255, 0);
//	inpaint(graytst, imageMasktst, wa, 3, INPAINT_TELEA);
//	namedWindow("r", 0);
//	imshow("r", wa);


	Mat imageMaskr;
	//ͨ����ֵ��������Mask
	threshold(finr, imageMaskr, 225, 255, 0);
	Mat Kernelr = getStructuringElement(MORPH_RECT, Size(5,5));
	//��Mask���ʹ�������Mask���
	//dilate(imageMaskr, imageMaskr, Kernelr);
	dilate(imageMaskr, imageMaskr, Mat(), Point(-1, -1), 3);
	//ͼ���޸�
	Mat tstr;
	inpaint(finr, imageMaskr, tstr, 4, INPAINT_TELEA);
	namedWindow("tst", 0);
	imshow("tst", tstr);
	imwrite("F:\\project\\lapla\\showr.jpg", tstr);
	imwrite("F:\\project\\lapla\\showl.jpg", tstl);
	cvtColor(tstl , colorl, CV_GRAY2BGR);
	cvtColor(tstr, colorr, CV_GRAY2BGR);

	Mat_<Vec3f> l; colorl.convertTo(l, CV_32F, 1.0 / 255.0);//Vec3f��ʾ������ͨ������ l[row][column][depth]  
	Mat_<Vec3f> r; colorr.convertTo(r, CV_32F, 1.0 / 255.0);
	/*****************    void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;******************/
	/* Performs linear transformation on every source array element:
	dst(x,y,c) = scale*src(x,y,alpha)+beta.
	Arbitrary combination of input and output array depths are allowed
	(number of channels must be the same), thus the function can be used
	for type conversion */

	//create blend mask matrix m  
	Mat_<float> m(l.rows, l.cols, 0.0);                 //��mȫ����ֵΪ0  
	m(Range::all(), Range::all()) = 0.5;    //ȡmȫ����&ȫ���У���ֵΪ1.0  

	Mat_<Vec3f> blend = LaplacianBlend(l, r, m);
	namedWindow("blended", 0);
	imshow("blended", blend);

	waitKey(0);
	return 0;
	system("pause");
}
