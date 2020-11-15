#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "string.h"
#include "iostream"

using namespace cv;
using namespace std;

void main()
{
	//批量读取图片(有序)
	//char filename[50];
	//char winName[50];
	//Mat srcImg;
	//int i;
	//for (i = 1;i < 100;i++)
	//{
	//	sprintf_s(filename,"%d.bmp",i);//字符串格式化操作
	//	sprintf_s(winName,"No.%d",i);
	//	srcImg=imread(filename);
	//	if (srcImg.empty())
	//	{
	//		break;
	//	}
	//	imshow("img",srcImg);
	//}

	//
	//waitKey(0);
	//destroyAllWindows();

	//视频分解为图片
//	int i=1;
//	Mat Frame;
//	char outfileName[50];
//	VideoCapture cap;
//	cap.open(0);
//	if (!cap.isOpened())//打开失败
//	{
//		return;
//	}
////	double totalFrame = cap.get(CV_CAP_PROP_FRAME_COUNT);
//	//for (i = 1;i <= totalFrame;i++)
//	while(1)
//	{
//		cap >> Frame;
//		if (Frame.empty())
//		{
//			break;
//		}
//		sprintf_s(outfileName, "H:\\%d.bmp", i);
//		imwrite(outfileName,Frame);
//		imshow("video",Frame);
//		if ('27'== waitKey(15))
//		{
//			return;
//		}
//		i++;
//	}
//	cap.release();
//	destroyAllWindows();

	//图片合成视频
	VideoWriter writer("H:\\out.avi",CV_FOURCC('X','V','I','D'),20,Size(568,320),true);
	VideoCapture cap;
	char fileName[50],winName[50];
	Mat Frame;
	int i;
	for (i = 1;i < 100;i++)
	{
		sprintf_s(fileName,"%d.bmp",i);
		//Frame=imread(fileName);  //使用图片读取的方法
		cap.open(fileName);//使用视频的方式读取图片
		if (!cap.isOpened())
		{
			break;
		}
		cap >> Frame;

		if (Frame.empty())
		{
			break;
		}
		writer << Frame;
	}
	cout << "write end!\n" << endl;
	destroyAllWindows();
}