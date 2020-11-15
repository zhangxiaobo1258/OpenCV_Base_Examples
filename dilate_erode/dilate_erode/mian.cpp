#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	Mat srcImg = imread("1.bmp");
	Mat dstImg;
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	cout << element << endl;
	//dilate(srcImg,dstImg,element,Point(-1,-1),5);
	//erode(srcImg,dstImg,element,Point(-1,-1),1);

	//morphologyEx(srcImg, dstImg, MORPH_OPEN, element);  //形态学开运算
	//morphologyEx(srcImg, dstImg, MORPH_OPEN, element);  //形态学闭运算
	morphologyEx(srcImg, dstImg, MORPH_GRADIENT, element);  //形态学梯度运算
	
	imshow("src", srcImg);
	imshow("dst", dstImg);
	waitKey(0);
}
