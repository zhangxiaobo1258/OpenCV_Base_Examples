#ifndef GUARD_cvsurf_h
#define GUARD_cvsurf_h

#include <opencv\cv.h>
#include "levenberg_marquardt.h"
#include "calib3d.hpp"

#define CV_SURF_EXTENDED (1)

typedef struct CvSURFDescriptor
{
	int x;
	int y;
	int laplacian;
	double s;
	double dir;
	double mod;
	double vector[128];
} CvSURFDescriptor;

typedef struct CvSURFPoints
{
	int x;
	int y;
	int laplacian;
	int size;
	int octave;
	int scale;
} CvSURFPoints;

CvSeq* cvSURFDescriptor( const CvArr* _img, CvMemStorage* storage, double quality, int flags = 0 );

void cvSURFInitialize();

void cvSURFFinalize();

bool cvSURFObjectLocate( CvSeq* ImageDescriptor, CvSeq* ObjectDescriptor, CvPoint* points, int flags = 0 );

CvSeq* cvSURFFindPair( CvSeq* ImageDescriptor, CvSeq* ObjectDescriptor, CvMemStorage* storage, int flags );

IplImage* cvRotation(IplImage* Object_Image,double Angle_val);

IplImage* cvMoveImage(IplImage* Object_Image,int width,int height);

IplImage* cvRotationMoveImage(IplImage* Object_Image,CvRatation_Parameter RatationMove_val);

#endif
