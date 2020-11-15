#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;
using namespace std;

void main()
{
	Mat srcImg = imread("1.jpg");
	Mat CannyImg;
	Mat dstImg = srcImg.clone();
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);//����ʹ����ɫ�ռ�ת�����������ͼ��Ϊ�Ҷ�ͼ
	//Canny(srcImg, CannyImg, 50, 200, 3);
	//vector<Vec2f> lines; //����ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������
	//HoughLines(CannyImg, lines, 1, CV_PI / 180, 150, 0, 0);

	//���Ƽ�⵽������
	//for (size_t i = 0;i < lines.size();i++)
	//{
	//	float rho = lines[i][0], theta = lines[i][1];
	//	Point pt1, pt2;
	//	double a = cos(theta), b = sin(theta);
	//	double x0 = a*rho, y0 = b*rho;
	//	pt1.x = cvRound(x0+1000*(-b));
	//	pt1.y = cvRound(y0+1000*(a));

	//	pt2.x = cvRound(x0-1000*(-b));
	//	pt2.y = cvRound(y0-1000*(a));

	//	line(dstImg, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
	//}

	vector<Vec3f> circles;
	GaussianBlur(srcImg, srcImg, Size(5, 5), 2, 2);
	HoughCircles(srcImg, circles, CV_HOUGH_GRADIENT, 1.5, 30, 200, 100, 10, 200);
	for (size_t i = 0;i < circles.size();i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		//circle(dstImg, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		circle(dstImg, center, radius, Scalar(0, 255, 0), -1, 8, 0);
	}

	imshow("dst",dstImg);
	waitKey(0);

}