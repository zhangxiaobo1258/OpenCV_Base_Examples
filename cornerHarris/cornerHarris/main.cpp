#include "opencv2/opencv.hpp"
#include  <iostream>

using namespace cv;
using namespace std;

void main()
{
	Mat SrcImg = imread("1.jpg");
	imshow("src", SrcImg);
	Mat ResultImg = SrcImg.clone();
	Mat GrayImg;
	cvtColor(SrcImg, GrayImg, COLOR_BGR2GRAY);
	
	Mat DstImg;
	Mat CornerImg;//´æ·Å¼ì²âºóµÄ½ÇµãÍ¼Ïñ
	cornerHarris(GrayImg, CornerImg, 2, 3, 0.05);//Harris½Çµã¼ì²â
	imshow("corner", CornerImg);
	threshold(CornerImg, DstImg, 0.005, 255, CV_THRESH_BINARY);
	imshow("dst", DstImg);

	int rowNumber = GrayImg.rows;
	int colNumber = GrayImg.cols;
	cout << rowNumber << endl;
	cout << colNumber << endl;
	cout << DstImg.type() << endl;
	for(int i=0;i<rowNumber;i++)
	{ 
		for (int j=0;j < colNumber;j++)
		{
			if (DstImg.at<float>(i, j) == 255)
			{
				circle(ResultImg, Point(j, i), 10, Scalar(255, 255, 0), 3, 8);
			}
		}
	}
	imshow("Result", ResultImg);
	waitKey(0);
}