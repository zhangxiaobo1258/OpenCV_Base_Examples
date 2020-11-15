/******************************************************************************************
*本程序为在OpenCV下解谜宫的程序，具体步骤：
*1.先将图片进行二值化；
*2.查找二值化后图片的最外层轮廓；
*3.绘制出其中的一个最外层轮廓并填充；
*4.对最外层轮廓进行膨胀，除去重复的路线；
*5.对膨胀后的轮廓进行腐蚀，腐蚀的宽度近似于迷宫中路的宽度；
*6.将膨胀后的轮廓和腐蚀后的轮廓进行绝对差值操作，得出线路图；
*7.对线路图查找最外层轮廓；
*8.将查找到的轮廓绘制到原图上，即可得出迷宫的路线图。
******************************************************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	Mat Img=imread("migong6.jpg");
	Mat srcImg;
	cvtColor(Img,srcImg,CV_BGR2GRAY);
	Mat ProcImg;
	srcImg.copyTo(ProcImg);
	Mat ProcImg1;
	srcImg.copyTo(ProcImg1);
	Mat draw = Mat::zeros(srcImg.size(), CV_32FC1);
	Mat diff,dstImg;
	Mat element;
	getStructuringElement(MORPH_RECT,Size(21,21),Point(-1,-1));
	imshow("Source Image",srcImg);
	threshold(ProcImg,ProcImg,200,255,THRESH_BINARY_INV);//如果走的线路为白色，则需要使用THRESH_BINARY_INV参数
	imshow("Threshold Image",ProcImg);
	vector<vector<Point>>contours;
	vector<Vec4i>h;
	findContours(ProcImg,contours,h,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	drawContours(draw, contours, 0, Scalar(255), -1);
	imshow("Contours Image",draw);
	dilate(draw,ProcImg,element,Point(-1,-1),9,BORDER_CONSTANT);//根据图片的不同修改膨胀的次数
	imshow("Dilate Image",ProcImg);
	erode(ProcImg,ProcImg1,element,Point(-1,-1),9,BORDER_CONSTANT);
	imshow("Erode Image",ProcImg1);
	absdiff(ProcImg,ProcImg1,diff);
	imshow("diff Image",diff);
	diff.convertTo(diff, CV_8UC1);
	vector<vector<Point>>contours2;
	findContours(diff,contours2,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	cout<<"diff contours is :"<<contours2.size()<<endl;
	drawContours(Img, contours2, -1, Scalar(255,255,0), -2);
	imshow("Result Image",Img);
	waitKey(0);
}


