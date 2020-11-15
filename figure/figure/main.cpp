#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

void main()
{
	Mat img(400, 400, CV_8UC3, Scalar(0, 0, 0));
	//画线
	line(img, Point(10, 10), Point(200, 200), Scalar(0, 255, 255), 2, 8);
	//画圆
	circle(img, Point(150, 150), 100, Scalar(255, 255, 0), 2, 8);
	//画填充圆
	circle(img, Point(160, 160), 100, Scalar(255, 255, 0), -1, 8);//内部填充
	//画矩阵
	rectangle(img,Point(10,10),Point(240,240),Scalar(0,255,0),2,8);
	//画填充的矩形
	rectangle(img,Rect(10,10,230,190),Scalar(0,0,255),-1,8);
	//画椭圆
	ellipse(img,Point(150,150),Size(100,50),90,0,360,Scalar(128,128,128),2,8);
	//画填充椭圆
	ellipse(img,Point(150,150),Size(100,50),45,0,360,Scalar(128,128,128),-1,8);
	//画旋转椭圆
	ellipse(img, RotatedRect(Point(100, 100), Size(100, 50), 45), Scalar(0, 255, 0), 2, 8);
	//画填充多边形
	Point ppt[] = { Point(120,50),Point(180,50),Point(210,100),Point(180,150),Point(120,150),Point(90,100) };
	const Point *pts[] = {ppt};
	int npt[] = { 6 };
	fillPoly(img,pts,npt,1,Scalar(255,255,0),8);
	//画闭合非填充多边形
	polylines(img,pts,npt,1,true,Scalar(0,255,255),2,8);
	//画非闭合非填充多边形
	polylines(img,pts,npt,1,false,Scalar(0,255,255),2,8);
	//增加文字
	putText(img,"China",Point(10,100),CV_FONT_HERSHEY_COMPLEX,3.5,Scalar(255,0,255),2,8);
	
	imshow("drawing",img);
	waitKey(0);
}