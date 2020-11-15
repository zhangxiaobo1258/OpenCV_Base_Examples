#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#define WIN_NAME "输出图像"

Mat srcImg,dstImg;
int Contrase_value = 20;
int Bright_Vaule = 20;

void OnChange(int, void*)
{
	for (int i = 0;i < srcImg.rows;i++)
	{
		for (int j = 0;j < srcImg.cols;j++)
		{
			dstImg.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(Contrase_value*0.01*srcImg.at<Vec3b>(i, j)[0] + Bright_Vaule);
			dstImg.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(Contrase_value*0.01*srcImg.at<Vec3b>(i, j)[1] + Bright_Vaule);
			dstImg.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(Contrase_value*0.01*srcImg.at<Vec3b>(i, j)[2] + Bright_Vaule);
		}
	}
	imshow("原图",srcImg);
	imshow(WIN_NAME,dstImg);
}

void main()
{
	srcImg = imread("1.jpg");
	dstImg = Mat::zeros(srcImg.size(),srcImg.type());//将Mat的元素都置为0
	//Mat::zeros();//将Mat的元素都置为0
	//Mat::ones();//将Mat的元素都置为1

	namedWindow(WIN_NAME,CV_WINDOW_AUTOSIZE);
	createTrackbar("对比度",WIN_NAME,&Contrase_value,300,OnChange,0);
	createTrackbar("亮度",WIN_NAME,&Contrase_value,200,OnChange,0);
	OnChange(Contrase_value,0);//回调函数初始化
	OnChange(Bright_Vaule,0);//回调函数初始化
	waitKey(0);
}