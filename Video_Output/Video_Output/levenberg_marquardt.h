#ifndef _levenberg_marquardt_h
#define _levenberg_marquardt_h

#include "opencv\cv.h"
#include "math.h"


typedef struct CvRatation_Parameter
{
	double Thata;
	double Center_X;
	double Center_Y;
}CvRatation_Parameter;

CvRatation_Parameter cvLevenberg_Marquardt(CvPoint* Object_Point_Val,CvPoint* Reference_Point_Val,int Heigth_Val,int Width_Val,int count);

#endif