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
	grayr.copyTo(maskedl, templ); //��Ĥ��ȡ��һ��ͼ�Ƿ��ⲿ��
	grayl.copyTo(maskedr, tempr);
	addWeighted(grayl, 1, templ, -1, 0, lastl);//���ⲿ��ȡ��
	addWeighted(lastl, 1, maskedl, 1, 0, finl);//���ⲿ���滻
	addWeighted(grayr, 1, tempr, -1, 0, lastr);
	addWeighted(lastr, 1, maskedr, 1, 0, finr);
//����ROI�����λ��
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
	int lenxl = (maxxl - minxl)/2+minxl;//������εĿ�  
	int lenyl = (maxyl - minyl)/2+minyl;//������εĸ�  

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
	int lenxr = (maxxr - minxr) / 2 + minxr;//������εĿ�  
	int lenyr = (maxyr - minyr) / 2 + minyr;//������εĸ�  


//	Mat src_mask = 255 * Mat::ones(templ.rows,templ.cols, templ.depth());
//�����ں�	
	Mat normal_clonel, normal_cloner;
	Mat colorr, colorl, colort;


	cvtColor(grayr, colorr, CV_GRAY2BGR);
	cvtColor(grayl, colorl, CV_GRAY2BGR);
	cvtColor(templ, colort, CV_GRAY2BGR);

	Point centerl(lenyl, lenxl);
	Point centerr(lenyr, lenxr);
	seamlessClone(colorr, colorl, templ, centerl, normal_clonel, NORMAL_CLONE);
	seamlessClone(colorl, colorr, tempr, centerr, normal_cloner, NORMAL_CLONE);

	Mat_<Vec3f> l; normal_clonel.convertTo(l, CV_32F, 1.0 / 255.0);//Vec3f��ʾ������ͨ������ l[row][column][depth]  
	Mat_<Vec3f> r; normal_cloner.convertTo(r, CV_32F, 1.0 / 255.0);
	Mat_<float> m(l.rows, l.cols, 0.0);                 //��mȫ����ֵΪ0  
	m(Range::all(), Range::all()) = 0.5;    //ȡmȫ����&ȫ���У���ֵΪ1.0  

	Mat_<Vec3f> blend = LaplacianBlend(l, r, m);
	namedWindow("blended", 0);
	imshow("blended", blend);

	namedWindow("�����ں�", 0);
	imshow("�����ں�", normal_clonel);
	imwrite("F:\\project\\lapla\\ggll.jpg", normal_clonel);
	imwrite("F:\\project\\lapla\\ggrr.jpg", normal_cloner);
	namedWindow("a", 0);
	imshow("a", maskedl);
	namedWindow("ba", 0);
	imshow("ba", templ);
//	namedWindow("ԭͼ", 0);
//	imshow("ԭͼ", colorr);
//	namedWindow("c", 0);
//	imshow("c", templ);
//	namedWindow("right", 0);
//	imshow("right", tempr);
//	namedWindow("left", 0);
//	imshow("left", templ);
//	namedWindow("ȥ�߹�", 0);
//	imshow("ȥ�߹�", finl);
//	namedWindow("lastr", 0);
//	imshow("lastr", finr);
	waitKey(0);
	return 0;
	system("pause");
}

