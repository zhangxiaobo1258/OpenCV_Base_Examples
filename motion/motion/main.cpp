#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\videoio\videoio.hpp"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

/*检测出运动物体，并计算出运动物体的轮廓的外接矩形*/
void  MotionDetect(Mat Ref_Img,										//参考图像
	Mat Dist_Img,													//目标图像
	double Threshold,												//二值化阈值
	InputArray ErodeKernel,											//腐蚀核尺寸 
	InputArray DilateKernel											//膨胀核尺寸
	)
{
												
	Mat Gray_Ref;
	Mat Gray_Dist;
	Mat Diff_Img;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	
	cvtColor(Ref_Img, Gray_Ref, CV_BGR2GRAY);								//参考图像灰度化处理
	cvtColor(Dist_Img, Gray_Dist, CV_BGR2GRAY);								//目标图像灰度化处理
	absdiff(Gray_Ref, Gray_Dist, Diff_Img);									//参考图像和目标图像的绝对差值
	imshow("absdiff Image", Diff_Img);
	threshold(Diff_Img, Diff_Img, Threshold, 255, THRESH_BINARY);			//对做差后的图像进行二值化
	imshow("Threshold Image", Diff_Img);
	erode(Diff_Img, Diff_Img, ErodeKernel);
	dilate(Diff_Img, Diff_Img, DilateKernel);
	//dilate(Diff_Img, Diff_Img, DilateKernel);
	imshow("Diff img", Diff_Img);
	findContours(Diff_Img, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);//查找轮廓
	vector<Rect> boundRect(contours.size());//定义外接矩形的集合
	int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
	//drawContours(Dist_Img, contours, -1, Scalar(0, 0, 255), 3, 8);
	for (int i = 0;i < contours.size();i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]);//查找每个轮廓的外接矩形
		x0 = boundRect[i].x;
		y0 = boundRect[i].y;
		w0 = boundRect[i].width;
		h0 = boundRect[i].height;
		rectangle(Dist_Img, Point(x0, y0), Point(x0 + w0, y0 + h0),Scalar(0,255,0), 2, 8);
	}
}

void main()
{

	Mat ErodeKernel = getStructuringElement(MORPH_RECT, Size(5, 5));		//定义腐蚀核大小
	Mat DilateKernel = getStructuringElement(MORPH_RECT, Size(50, 50));		//定义膨胀核的大小

	VideoCapture Video;
	double fps;
	int count=1;
	Mat Ref_Image,Dist_Image,Frame_Image;
	Video.open("2.avi");
	if ( ! Video.isOpened())
	{
		cout << "Video opened failed!" << endl;
		exit(1);
	}
	fps = Video.get(CV_CAP_PROP_FPS);//获取视频的帧数
	Video >> Frame_Image;//将第一帧数据写入参考帧中
	Frame_Image.copyTo(Ref_Image);
	while(1)
	{
		count++;
		Video >> Frame_Image;//将第二帧数据写入目标帧
		Frame_Image.copyTo(Dist_Image);
		if (Dist_Image.empty())
		{
			break;
		}
		MotionDetect(Ref_Image, Dist_Image, 30, ErodeKernel, DilateKernel);
		Frame_Image.copyTo(Ref_Image);
		imshow("视频显示", Dist_Image);
		int c = waitKey(30);
		if (c == 'q' || c == 'Q' || (c & 255) == 27)
			break;
	}
	destroyAllWindows();
	exit(1);
}
