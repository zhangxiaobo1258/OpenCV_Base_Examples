#include "opencv2/opencv.hpp"

using namespace cv;

void main()
{
	//ÂþË®Ìî³äËã·¨ 
	Mat srcImg = imread("1.jpg");
	imshow("src",srcImg);
	Rect rect;
	floodFill(srcImg, Point(20, 20), Scalar(255, 0, 255), &rect, Scalar(10, 10, 10), Scalar(10, 10, 10));
	imshow("result", srcImg);
	waitKey(0);
}