#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;

void main()
{
	//--��Ӿ��εĲ��Һͻ���	
	/*Mat srcImg = imread("1.jpg");
	Mat dstImg = srcImg.clone();
	cvtColor(srcImg,srcImg,CV_BGR2GRAY);
	threshold(srcImg,srcImg,100,255,CV_THRESH_BINARY);//��ֵ��

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);//��������
	vector<Rect> boundRect(contours.size());	
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]);//����ÿ����������Ӿ���
		drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
		x = boundRect[i].x; //��õ�i����Ӿ��ε����Ͻǵ�x����
		y = boundRect[i].y; //��õ�i����Ӿ��ε����Ͻǵ�y����
		w = boundRect[i].width;
		h = boundRect[i].height;
		rectangle(dstImg, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 2, 8);//���Ƶ�i����Ӿ���
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/
	
	//�ָ�Ӳ������
	/*Mat srcImg = imread("1.jpg");
	Mat dstImg = srcImg.clone();
	cvtColor(srcImg,srcImg,CV_BGR2GRAY);
	threshold(srcImg,srcImg,100,255,CV_THRESH_BINARY);//��ֵ��

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(srcImg, srcImg, element);//���Ͳ���

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);//��������
	vector<Rect> boundRect(contours.size());	
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]);//����ÿ����������Ӿ���
		drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
		x = boundRect[i].x; //��õ�i����Ӿ��ε����Ͻǵ�x����
		y = boundRect[i].y; //��õ�i����Ӿ��ε����Ͻǵ�y����
		w = boundRect[i].width;
		h = boundRect[i].height;
		if (w > 30 && h > 30)//�˳�
		{
			rectangle(dstImg, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 2, 8);//���Ƶ�i����Ӿ���
		}
	}
	imshow("boundRect", dstImg);
	waitKey(0);*/

	//���Ʒָ�
	Mat srcImg = imread("car.jpg");
	Mat dstImg = srcImg.clone();
	medianBlur(srcImg, srcImg, 5);//��ֵ�˲�����
	cvtColor(srcImg,srcImg,CV_BGR2GRAY);
	threshold(srcImg,srcImg,100,255,CV_THRESH_BINARY);//��ֵ��
	imshow("Gray", srcImg);

	//Mat element = getStructuringElement(MORPH_RECT, Size(6, 6), Point(-1, -1));
	//dilate(srcImg, srcImg, element);//���Ͳ���

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	int x = 0, y = 0, w = 0, h = 0;
	findContours(srcImg, contours, hierarcy, CV_RETR_TREE, CHAIN_APPROX_NONE);//��������
	vector<Rect> boundRect(contours.size());	
	//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
	for (int i = 0;i < contours.size();i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]);//����ÿ����������Ӿ���
		//drawContours(dstImg, contours, -1, Scalar(0, 0, 255), 2, 8);
		x = boundRect[i].x; //��õ�i����Ӿ��ε����Ͻǵ�x����
		y = boundRect[i].y; //��õ�i����Ӿ��ε����Ͻǵ�y����
		w = boundRect[i].width;
		h = boundRect[i].height;
		if (w > srcImg.cols / 12 && w <srcImg.cols / 7 && h>srcImg.rows / 6 && h < srcImg.rows * 5 / 6)//�˳�
		{
			char pic_name[10];
			sprintf_s(pic_name, "%d.bmp", i);
			Mat ROI = dstImg(Rect(x, y, w, h));
			imwrite(pic_name, ROI);
			rectangle(dstImg, Point(x, y), Point(x + w, y + h), Scalar(0, 255, 0), 2, 8);//���Ƶ�i����Ӿ���
		}
	}
	imshow("boundRect", dstImg);
	waitKey(0);
}