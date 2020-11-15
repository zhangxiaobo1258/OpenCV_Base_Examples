//#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2\calib3d.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <ctype.h>
#include "iostream"
using namespace std;
using namespace cv;

IplImage *image = 0;

int main(void)
{
	cvNamedWindow( "Video Display", CV_WINDOW_AUTOSIZE );
	CvCapture * cap=cvCaptureFromAVI("TestVideo.avi");  //获取当前路径下的视频数据
	//CvCapture * cap=cvCaptureFromCAM(0);  //获取摄像头
	CvVideoWriter * video=NULL;
	int i=30000;
	if(!cap)
		return 0;
	IplImage *Frame=cvQueryFrame(cap);//获得一帧图像;

	int n=0;
	video=cvCreateVideoWriter("TestVideo2.avi",CV_FOURCC('D', 'I', 'V', 'X') ,25,cvSize(Frame->width,Frame->height));
	n=cvWriteFrame(video,Frame);
	while(i>0)
	{
		Frame=cvQueryFrame(cap);//获得一帧图像

		n=cvWriteFrame(video,Frame);
		cvShowImage("Video Display",Frame);
		i--;
		if(cvWaitKey(1)==27)
			break;
	}
	cvReleaseCapture(&cap);
	cvReleaseVideoWriter(&video);
	cvDestroyAllWindows();
	return 0;
}