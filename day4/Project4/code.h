#ifndef CODE_H 
#define CODE_H 
#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 
using namespace cv;  
void ER( const IplImage *_src,  const int _level );
void IDWT( const IplImage *_src,  const int _level );
#endif// CODE_H 
