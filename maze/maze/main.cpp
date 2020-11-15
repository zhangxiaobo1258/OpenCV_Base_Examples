/******************************************************************************************
*������Ϊ��OpenCV�½��չ��ĳ��򣬾��岽�裺
*1.�Ƚ�ͼƬ���ж�ֵ����
*2.���Ҷ�ֵ����ͼƬ�������������
*3.���Ƴ����е�һ���������������䣻
*4.������������������ͣ���ȥ�ظ���·�ߣ�
*5.�����ͺ���������и�ʴ����ʴ�Ŀ�Ƚ������Թ���·�Ŀ�ȣ�
*6.�����ͺ�������͸�ʴ����������о��Բ�ֵ�������ó���·ͼ��
*7.����·ͼ���������������
*8.�����ҵ����������Ƶ�ԭͼ�ϣ����ɵó��Թ���·��ͼ��
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
	threshold(ProcImg,ProcImg,200,255,THRESH_BINARY_INV);//����ߵ���·Ϊ��ɫ������Ҫʹ��THRESH_BINARY_INV����
	imshow("Threshold Image",ProcImg);
	vector<vector<Point>>contours;
	vector<Vec4i>h;
	findContours(ProcImg,contours,h,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	drawContours(draw, contours, 0, Scalar(255), -1);
	imshow("Contours Image",draw);
	dilate(draw,ProcImg,element,Point(-1,-1),9,BORDER_CONSTANT);//����ͼƬ�Ĳ�ͬ�޸����͵Ĵ���
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


