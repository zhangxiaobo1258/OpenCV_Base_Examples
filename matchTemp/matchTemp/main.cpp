#include "opencv2/opencv.hpp"
#include<iostream>

using namespace std;
using namespace cv;

void main()
{
	//��ģ��ƥ��
	//Mat temp = imread("temp.jpg");
	//Mat src = imread("1.jpg");
	//int width = src.cols - temp.cols + 1;
	//int height = src.rows - temp.rows + 1;
	//Mat result(height, width, CV_32FC1);
	//matchTemplate(src, temp, result, CV_TM_CCOEFF_NORMED);//�����ϵ��ƥ�����ֵ1
	//imshow("result", result);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1); //��һ����0--1��Χ
	//double minValue, maxValue;
	//Point minLoc, maxLoc;
	//minMaxLoc(result,&minValue,&maxValue,&minLoc,&maxLoc);
	//cout << "minValue = " << minValue << endl;
	//cout << "maxVaule = " << maxValue << endl;
	//rectangle(src.clone(), maxLoc, Point(maxLoc.x + temp.cols, maxLoc.y + temp.rows), Scalar(0, 255, 0), 3, 8);

	//��Ƶƥ��
	//��ģ��ƥ��
	waitKey(0);
}