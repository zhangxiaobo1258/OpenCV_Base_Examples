#include "opencv2/opencv.hpp"
#include<iostream>

using namespace std;
using namespace cv;

void main()
{
	//单模板匹配
	//Mat temp = imread("temp.jpg");
	//Mat src = imread("1.jpg");
	//int width = src.cols - temp.cols + 1;
	//int height = src.rows - temp.rows + 1;
	//Mat result(height, width, CV_32FC1);
	//matchTemplate(src, temp, result, CV_TM_CCOEFF_NORMED);//化相关系数匹配最佳值1
	//imshow("result", result);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1); //归一化到0--1范围
	//double minValue, maxValue;
	//Point minLoc, maxLoc;
	//minMaxLoc(result,&minValue,&maxValue,&minLoc,&maxLoc);
	//cout << "minValue = " << minValue << endl;
	//cout << "maxVaule = " << maxValue << endl;
	//rectangle(src.clone(), maxLoc, Point(maxLoc.x + temp.cols, maxLoc.y + temp.rows), Scalar(0, 255, 0), 3, 8);

	//视频匹配
	//多模板匹配
	waitKey(0);
}