#include"code.h"  
#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 
using namespace cv;
using namespace std;
int main(int argc, char *argv[])
{    
	int nLayer = 2;
	// �����ɫͼ��
	IplImage *pSrc = cvLoadImage("F:\\project\\lapla\\left.jpg", CV_LOAD_IMAGE_COLOR);
	// ����С��ͼ���С
	CvSize size = cvGetSize(pSrc);
	if ((pSrc->width >> nLayer) << nLayer != pSrc->width)
	{
		size.width = ((pSrc->width >> nLayer) + 1) << nLayer;
	}
	if ((pSrc->height >> nLayer) << nLayer != pSrc->height)
	{
		size.height = ((pSrc->height >> nLayer) + 1) << nLayer;
	}
	// ����С��ͼ��
	IplImage *pWavelet = cvCreateImage(size, IPL_DEPTH_32F, pSrc->nChannels);
	if (pWavelet)
	{
		// С��ͼ��ֵ
		cvSetImageROI(pWavelet, cvRect(0, 0, pSrc->width, pSrc->height));
		cvConvertScale(pSrc, pWavelet, 1, -128);
		cvResetImageROI(pWavelet);
		// ��ɫͼ��С���任
		IplImage *pImage = cvCreateImage(cvGetSize(pWavelet), IPL_DEPTH_32F, 1);
		if (pImage)
		{
			for (int i = 1; i <= pWavelet->nChannels; i++)
			{
				cvSetImageCOI(pWavelet, i);
				cvCopy(pWavelet, pImage, NULL);
				// ��ά��ɢС���任
				ER(pImage, nLayer);
				// ��ά��ɢС���ָ�
				IDWT(pImage,nLayer);
				cvCopy(pImage, pWavelet, NULL);
			}
			cvSetImageCOI(pWavelet, 0);
			cvReleaseImage(&pImage);
		}
		// С���任ͼ��
		cvSetImageROI(pWavelet, cvRect(0, 0, pSrc->width, pSrc->height));
		cvConvertScale(pWavelet, pSrc, 1, 128);
		cvResetImageROI(pWavelet); // ���д����е���࣬���������������õı��ϰ��
		cvReleaseImage(&pWavelet);
	}
	// ��ʾͼ��pSrc
	// ...
	cvReleaseImage(&pSrc);
	waitKey(0);  
	return 0;
}