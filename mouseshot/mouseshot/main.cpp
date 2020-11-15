#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat img = imread("1.jpg");
Mat temp = img.clone();
Mat ROI;
Point pt;
bool flag;

void OnMouse(int event,int x,int y ,int flag,void* param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		//cout << "left button down!\n" << endl;
		flag = true;
		pt.x = x;
		pt.y = y;
		break;
	case CV_EVENT_MOUSEMOVE:
		//cout << "mouse move!\n" << endl;
		if (flag == true)
		{
			temp.copyTo(img);
			rectangle(img, pt, Point(x, y), Scalar(0, 255, 0), 2, 8);
		}
		
		break;
	case CV_EVENT_LBUTTONUP:	
		//cout << "left button up!\n" << endl;
		flag = true;
		ROI = temp(Rect(pt.x, pt.y, x - pt.x, y - pt.y));
		imshow("ROI",ROI);
		imwrite("ROI.jpg",ROI);
		break;
	default:
			break;
	}
}

void main()
{
	
	namedWindow("mouseshot",CV_WINDOW_AUTOSIZE);
	
	setMouseCallback("mouseshot",OnMouse,0);//设置鼠标回调函数

	while (1)
	{
		imshow("mouseshot", img);
		if (27 == waitKey(10))
			break;
	}
}