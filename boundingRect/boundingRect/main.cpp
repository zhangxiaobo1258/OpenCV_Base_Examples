#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;

void main()
{
	//--外接矩形的查找和绘制	
	/*Mat srcImg = imread("1.jpg");
	Mat dstImg = srcImg.clone();
	cvtColor(srcImg,srcImg,CV_BGR2GRAY);
	threshold(srcImg,srcImg,100,255,CV_THRESH_BINARY);//二值化

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);//查找轮廓
	vector<Rect> boundRect(contours.size());	
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]);//查找每个轮廓的外接矩形
		drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
		x = boundRect[i].x; //获得第i个外接矩形的左上角的x坐标
		y = boundRect[i].y; //获得第i个外接矩形的左上角的y坐标
		w = boundRect[i].width;
		h = boundRect[i].height;
		rectangle(dstImg, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 2, 8);//绘制第i个外接矩形
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/
	
	//分割硬币轮廓
	/*Mat srcImg = imread("1.jpg");
	Mat dstImg = srcImg.clone();
	cvtColor(srcImg,srcImg,CV_BGR2GRAY);
	threshold(srcImg,srcImg,100,255,CV_THRESH_BINARY);//二值化

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(srcImg, srcImg, element);//膨胀操作

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);//查找轮廓
	vector<Rect> boundRect(contours.size());	
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]);//查找每个轮廓的外接矩形
		drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
		x = boundRect[i].x; //获得第i个外接矩形的左上角的x坐标
		y = boundRect[i].y; //获得第i个外接矩形的左上角的y坐标
		w = boundRect[i].width;
		h = boundRect[i].height;
		if (w > 30 && h > 30)//滤除
		{
			rectangle(dstImg, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 2, 8);//绘制第i个外接矩形
		}
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/

	//车牌分割
	Mat srcImg = imread("car.jpg");
	Mat dstImg = srcImg.clone();
	medianBlur(srcImg, srcImg, 5);//中值滤波操作
	cvtColor(srcImg,srcImg,CV_BGR2GRAY);
	threshold(srcImg,srcImg,100,255,CV_THRESH_BINARY);//二值化
	imshow("Gray", srcImg);

	//Mat element = getStructuringElement(MORPH_RECT, Size(6, 6), Point(-1, -1));
	//dilate(srcImg, srcImg, element);//膨胀操作

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_TREE, CHAIN_APPROX_NONE);//查找轮廓
	vector<Rect> boundRect(contours.size());	
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]);//查找每个轮廓的外接矩形
		//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
		x = boundRect[i].x; //获得第i个外接矩形的左上角的x坐标
		y = boundRect[i].y; //获得第i个外接矩形的左上角的y坐标
		w = boundRect[i].width;
		h = boundRect[i].height;
		if (w > srcImg.cols / 12 && w <srcImg.cols / 7 && h>srcImg.rows / 6 && h < srcImg.rows * 5 / 6)//滤除
		{
			char pic_name[10];
			sprintf_s(pic_name, "%d.bmp", i);
			Mat ROI = dstImg(Rect(x, y, w, h));
			imwrite(pic_name, ROI);
			rectangle(dstImg, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 2, 8);//绘制第i个外接矩形
		}
	}
	imshow("boundRect", dstImg);
	waitKey(0);
}