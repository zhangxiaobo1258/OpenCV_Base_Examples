#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "math.h"

using  namespace cv;

//Mat imgTranslate(Mat &srcImg, int xOffset, int yOffset)
//{
//	int rows = srcImg.rows;
//	int cols = srcImg.cols;
//	Mat dstImg = Mat::zeros(srcImg.size(), srcImg.type());//创建全黑图像
//
//	for (int i = 0;i < rows;i++)
//	{
//		for (int j = 0;j < cols;j++)
//		{
//			int x = j + xOffset;
//			int y = i + yOffset;
//			if (x >= 0 && y >= 0 && x < cols&& y < rows)
//			{
//				dstImg.at<Vec3b>(y, x) = srcImg.at<Vec3b>(i, j);
//			}
//		}	
//	}
//	return dstImg;
//}
Mat imgTranslate(Mat &srcImg, int xOffset, int yOffset)
{
	int rows = srcImg.rows+abs(yOffset);
	int cols = srcImg.cols+abs(xOffset);
	Mat dstImg = Mat::zeros(rows,cols, srcImg.type());//创建全黑图像

	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			int x = j + xOffset;
			int y = i + yOffset;
			if (x >= 0 && y >= 0 && x < cols&& y < rows)
			{
				dstImg.at<Vec3b>(y, x) = srcImg.at<Vec3b>(i, j);
			}
		}	
	}
	return dstImg;
}

void main()
{
	Mat srcImg = imread("1.jpg");
	//Point2f center = Point2f(srcImg.cols / 2, srcImg.rows / 2);
	//double angle = 30;
	//double scale = 0.5;
	//Mat roateM = getRotationMatrix2D(center, angle, scale);//获得旋转矩阵
	Mat dstImg;
	//warpAffine(srcImg, dstImg, roateM, Size(1000, 800));
	//Mat dstImg = imgTranslate(srcImg, 20, 30);
	//resize(srcImg, dstImg, Size(1000, 1000));
	//transpose(srcImg, dstImg);//图像转置
	//flip(srcImg, dstImg, 0);//图像镜像

	Mat xMapImg = Mat::zeros(srcImg.size(), CV_32FC1);
	Mat yMapImg = Mat::zeros(srcImg.size(), CV_32FC1);

	for (int i = 0;i < srcImg.rows;i++)
	{
		for (int j = 0;j < srcImg.cols;j++)
		{
			xMapImg.at<float>(i, j) = j;//保持列不变
			yMapImg.at<float>(i, j) = i+5*sin(j/10.0);
		}
	}

	remap(srcImg, dstImg, xMapImg, yMapImg, CV_INTER_LINEAR);

	imshow("src",srcImg);
	imshow("dst", dstImg);


	waitKey(0);
}
