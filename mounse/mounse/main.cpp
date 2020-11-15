#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;


//Point pt;

Mat srcImg, dstImg;
int value = 20;
//void onMouse(int event,int x, int y,int flag,void *param)
//{
//	Mat img = *(Mat *)param;
//	//if (event == CV_EVENT_LBUTTONDOWN)
//	//{
//	//	cout << "Mouse Down" << endl;
//	//	circle(img,Point(x,y),80,Scalar(255,255,0),2,8);
//	//}	
//	//switch (event)
//	//{
//	//case CV_EVENT_LBUTTONDOWN:
//	//	pt.x = x;
//	//	pt.y = y;
//	//	break;
//	//case CV_EVENT_LBUTTONUP:
//	//	rectangle(img,pt,Point(x,y),Scalar(0,225,0),2,8);
//	//	break;
//	default:
//		break;
//	}
//}

void OnChange(int, void *)
{
	//threshold(srcImg,dstImg,value,255,THRESH_BINARY);//对图像做二值化操作
	Canny(srcImg,dstImg,value,255);
	imshow("TrackBar",dstImg);
}

void main()
{
	
	//Mat img(500,500,CV_8UC3,Scalar(255,255,255));	
	//namedWindow("Mouse",CV_WINDOW_AUTOSIZE);
	//setMouseCallback("Mouse",onMouse,&img);
	//while (1)
	//{
	//	imshow("Mouse",img);
	//	if (27==waitKey(10))
	//		break;
	//}

	srcImg = imread("1.jpg",0);//读取的是灰度图片
	namedWindow("TrackBar", CV_WINDOW_AUTOSIZE);
	createTrackbar("Threshold","TrackBar",&value,255,OnChange,0);
	//threshold(srcImg,dstImg,value,255,THRESH_BINARY);//对图像做二值化操作
	Canny(srcImg,dstImg,value,255);
	imshow("TrackBar",dstImg);	
	waitKey(0);
}
