#include "opencv2/opencv.hpp"

using namespace cv;

void main()
{
	Mat srcImg = imread("1.jpg", 0);
	Mat Grad_x,Grad_y,dstImg;
	//Canny(srcImg, dstImg, 30, 80);
	imshow("src", srcImg);

	//Sobel(srcImg, Grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	//convertScaleAbs(Grad_x,Grad_x);
	//imshow("Grad_x", Grad_x);

	//Sobel(srcImg, Grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	//convertScaleAbs(Grad_y,Grad_y);
	//imshow("Grad_y", Grad_y);

	//addWeighted(Grad_x, 0.5, Grad_y, 0.5, 1, dstImg);
	//imshow("ºÏ²¢ºóÍ¼Ïñ", dstImg);

	Laplacian(srcImg, dstImg, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(dstImg, dstImg);
	imshow("dst", dstImg);

	waitKey(0);

}