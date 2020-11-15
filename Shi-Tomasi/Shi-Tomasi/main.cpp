#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void main()
{
	Mat SrcImg = imread("1.jpg");
	imshow("src", SrcImg);
	Mat ResultImg = SrcImg.clone();
	Mat GrayImg;
	cvtColor(SrcImg, GrayImg, COLOR_BGR2GRAY);
	
	vector<Point2f> Corners;//��ż���Ľǵ�ͼ��
	goodFeaturesToTrack(GrayImg, Corners, 100,0.01,2, Mat(),3,false,0.04);//Shi-Tomasi�ǵ���
	cout << "�ǵ�������"<<Corners.size() << endl;
	for(int i=0;i<Corners.size();i++)
	{ 
		cout << "�������꣺��" << Corners[i].x << "," << Corners[i].y <<"��"<< endl;
		circle(ResultImg, Corners[i], 10, Scalar(255, 255, 0), 3, 8);
	}
	cout << "--------------------------------------" << endl;
	imshow("Result", ResultImg);

	Size winSize = Size(5, 5);
	Size zeroZone = Size(-1, -1);
	//��������Ŀ40�򾫶�0.001����һ���ﵽ
	TermCriteria criteria = TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001);
	cornerSubPix(GrayImg, Corners, winSize, zeroZone, criteria);

	for(int i=0;i<Corners.size();i++)
	{ 
		cout << "���������꣺��" << Corners[i].x << "," << Corners[i].y <<")"<< endl;
		circle(SrcImg, Corners[i], 10, Scalar(0, 255, 255), 3, 8);
	}
	imshow("Result", SrcImg);
	waitKey(0);
}