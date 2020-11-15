#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <ctype.h>
#include "iostream"
using namespace std;
using namespace cv;

//IplImage *image = 0;

//int main(void)
//{
//	cvNamedWindow( "Video Display");
	//CvCapture * cap=cvCaptureFromAVI("TestVideo.avi");  //��ȡ��ǰ·���µ���Ƶ����
//	CvCapture * cap = cvCaptureFromCAM(0);  //��ȡ����ͷ
//	CvVideoWriter * video=NULL;
	//int i=30000;
//	if(!cap)
//		return 0;
//	IplImage *Frame=cvQueryFrame(cap);//���һ֡ͼ��;

//	int n=0;
//	video=cvCreateVideoWriter("TestVideo.avi",CV_FOURCC('D', 'I', 'V', 'X') ,25,cvSize(Frame->width,Frame->height));
//	n=cvWriteFrame(video,Frame);
//	_sleep(20);
//	while(1)
//	{
//		Frame=cvQueryFrame(cap);//���һ֡ͼ��
//		if (!Frame)
//			break;
//		n=cvWriteFrame(video,Frame);
//		cvShowImage("Video Display1",Frame);
		//i--;
		
//		cvWaitKey(0);//if(cvWaitKey(0)==27)
		//	break;
//	}
//	cvReleaseCapture(&cap);
//	cvReleaseVideoWriter(&video);
//	cvDestroyAllWindows();
//	return 0;
//}

int main(int argc, char** argv)
{
	double fps;
	int i = 3000;
	VideoCapture cap;
	VideoWriter video;
	Mat Frame;//���һ֡ͼ��;
	if (argc > 1)
		cap.open(argv[1]);
	else
		cap.open(0);

	if (!cap.isOpened())//�������ͷ��ʧ��
	{
		std::cerr << "Cannot read video. Try moving video file to sample directory." << std::endl;
		exit(1);
	}
	
	fps = cap.get(CV_CAP_PROP_FPS);
	video.open("TestVideo.avi", VideoWriter::fourcc('M', 'P', 'E', 'G'),30.0, cvSize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_WIDTH)));
	cap >> Frame;
	video << Frame;
	while(i>0)
	{
		cap >> Frame;//�����봫��frame
		if (Frame.empty())
			break;
		video << Frame;
		imshow("��Ƶ��ʾ", Frame);

		i--;
		int c = waitKey(30);
		if (c == 'q' || c == 'Q' || (c & 255) == 27)
			break;
	}
	return 0;
}


