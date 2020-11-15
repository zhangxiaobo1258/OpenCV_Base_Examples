#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <iostream>

using namespace std;

#define Center_X  10
#define Center_Y  10
#define Angle 20

int main()
{
	FILE* fp;
	double degree = Angle; 
	double angle = degree  * CV_PI / 180.; // angle in radian
	double a = sin(angle), b = cos(angle); // sine and cosine of angle

	// Load source image as you wish
	IplImage *imgSrc = cvLoadImage("hua6.jpg");
	int w_src = imgSrc->width;
	int h_src = imgSrc->height;
	cvNamedWindow ("src", 1);
	cvShowImage ("src", imgSrc);
	// Make w_dst and h_dst to fit the output image
	int w_dst = int(h_src * fabs(a) + w_src * fabs(b));
	int h_dst = int(w_src * fabs(a) + h_src * fabs(b));

	// map matrix for WarpAffine, stored in statck array
	double map[6];
	CvMat map_matrix = cvMat(2, 3, CV_64FC1, map);

	// Rotation center needed for cv2DRotationMatrix
	CvPoint2D32f pt;
	pt.x=Center_X;
	pt.y=Center_Y;
	cv2DRotationMatrix(pt, degree, 1.0, &map_matrix);

	// Adjust rotation center to dst's center,
	// otherwise you will get only part of the result
	map[2] +=(w_dst - w_src) / 2;
	map[5] +=(h_dst - h_src) / 2;


	// We need a destination image

	IplImage *imgDst = cvCreateImage(cvSize(w_dst,h_dst), 8, 3);
	cvWarpAffine(
		imgSrc, 
		imgDst,
		&map_matrix,
		CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,
		cvScalarAll(0)
		);

	// Don't forget to release imgSrc and imgDst if you no longer need them
	cvNamedWindow( "dst_big", 1 );
	cvShowImage( "dst_big", imgDst);
	cvWaitKey(0);
	///cvReleaseImage(&imgSrc);
	///cvReleaseImage(&imgDst);

	system("pause");

	return 0;
}