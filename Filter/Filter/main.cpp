#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using  namespace cv;

void main()
{
	Mat srcImg = imread("1.jpg");
	Mat dstImg;

	//线性滤波
	//boxFilter(srcImg, dstImg, -1, Size(3, 3), Point(-1, -1), true);//方框滤波器
	//blur(srcImg, dstImg, Size(3, 3));//均值滤波
	//GaussianBlur(srcImg, dstImg, Size(3, 3), 1);//高斯滤波，用得比较多

	//非线性滤波
	//medianBlur(srcImg, dstImg, 3);//中值滤波的方法
	bilateralFilter(srcImg, dstImg, 5, 10.0, 2);//双边滤波器

	imshow("src", srcImg);
	imshow("dst", dstImg);
	waitKey(0);
}
