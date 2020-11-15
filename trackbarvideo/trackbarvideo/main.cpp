#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;


double fps;
int count;
int value;
int framePos;


void OnChange(int, void* param)
{
	VideoCapture cap = *(VideoCapture *)param;
	cap.set(CV_CAP_PROP_POS_FRAMES,value);
}

void main()
{
	char strfps[20];
	Mat Frame;
	VideoCapture cap("1.avi");
	fps=cap.get(CV_CAP_PROP_FPS);
	count = cap.get(CV_CAP_PROP_FRAME_COUNT);
	sprintf_s(strfps,"Fps:%0.1f",fps);
	if (!cap.isOpened())
	{
		return;
	}
	namedWindow("video",CV_WINDOW_AUTOSIZE);
	createTrackbar("Frame","video",&value,count,OnChange,&cap);//使用回调函数时，一定要修改最后一个参数
	while (1)
	{
		framePos = cap.get(CV_CAP_PROP_POS_FRAMES);
		setTrackbarPos("Frame","video",framePos);
		cap >> Frame;
		if (Frame.empty())
		{
			break;
		}
		putText(Frame,strfps,Point(5,30),CV_FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,8);
		imshow("video",Frame);
		if (27==waitKey(1000/fps))
			break;
	}
	cap.release();
	destroyAllWindows();
}