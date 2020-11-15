#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

void main()
{
	Mat srcImg1 = imread("00.jpg");
	Mat srcImg2 = imread("01.jpg");
	//定义SIFT特征检测类对象
	SiftFeatureDetector  siftDetector1;
	SiftFeatureDetector  siftDetector2;
	//定义keyPoint变量
	vector<KeyPoint> KeyPoint1;
	vector<KeyPoint> KeyPoint2;
	//特征点检测
	siftDetector1.detect(srcImg1,KeyPoint1);
	siftDetector1.detect(srcImg2,KeyPoint2);
	//绘制特征点
	Mat Feature_Img1, Feature_Img2;
	drawKeypoints(srcImg1,KeyPoint1,Feature_Img1,Scalar(0,0,255));
	drawKeypoints(srcImg2,KeyPoint2,Feature_Img2,Scalar(0,0,255));
	//drawKeypoints(srcImg1, KeyPoint1, Feature_Img1, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//drawKeypoints(srcImg1, KeyPoint1, Feature_Img1, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//显示原图
	imshow("src1", srcImg1);
	imshow("src2", srcImg2);
	//显示结果
	imshow("feature1", Feature_Img1);
	imshow("feature2", Feature_Img2);

	waitKey(0);
}

